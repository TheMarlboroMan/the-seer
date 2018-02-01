#include "actor_audio.h"

Actor_audio::Actor_audio(unsigned int px, unsigned int py, unsigned int pz, 
	unsigned int pt, unsigned int pson, unsigned int pvol, 
	unsigned int ppan, unsigned int pp1, unsigned int pp2, bool pa,
	unsigned int pid):
	Actor(px, py, pz), Activable(pa),
	canal(), tipo_reproduccion(pt), id_sonido(pson), volumen(pvol),
	pan(ppan), param1(pp1), param2(pp2), tiempo_actual(0.0),
	tiempo_objetivo(0.0), iniciada_reproduccion(false)
{
	actualizar_volumen();
	actualizar_id_localizador(pid);

	calcular_tiempos();
}

Actor_audio::~Actor_audio()
{
	liberar_canal();
}

void Actor_audio::calcular_tiempos()
{
	float tiempo_pre_aleatorio=(float)param1 / 1000;
	
	tiempo_actual=0.0;

	switch(tipo_reproduccion)
	{
		case T_REPETIR_ALEATORIO:
		{
			float tiempo_post_aleatorio=param2 ? ( (float) DLibH::Herramientas::obtener_aleatorio_tramo(0, param2) / 1000) : 0.0;
			tiempo_objetivo=tiempo_pre_aleatorio + tiempo_post_aleatorio;
		}
		break;

		case T_REPETIR_INTERVALO:
			tiempo_objetivo=tiempo_pre_aleatorio;
		break;
	}
}

void Actor_audio::liberar_canal()
{
	if(canal.es_vinculado())
	{
		canal.desactivar_monitoreado();
		canal.forzar_parada();
		canal.desvincular();
	}

	iniciada_reproduccion=false;
}

void Actor_audio::iniciar_reproduccion()
{
	if(!canal.es_vinculado()) 
	{
		try
		{
			canal=Audio::obtener_canal_libre();
			canal.activar_monitoreado();
		}
		catch(const Excepcion_audio& e)
		{
			LOG<<e.mensaje<<std::endl;
			return;
		}
		catch(const DLibA::Excepcion_controlador_audio& e)
		{
			LOG<<"WARNING: Imposible obtener canal de audio para actor de audio."<<std::endl;
			return;
		}
	}

	int repeticiones=0;

	switch(tipo_reproduccion)
	{
		case T_UNICO: repeticiones=param1; break;
		case T_REPETIR: repeticiones=-1; break;
		case T_REPETIR_ALEATORIO:
		case T_REPETIR_INTERVALO: 
			repeticiones=0; break;
	}
	
	int pan_d=(pan / 2);		
	int pan_i=255-pan_d;

	canal.iniciar_reproduccion(
		DLibA::Estructura_sonido(
				DLibA::Gestor_recursos_audio::obtener_sonido(id_sonido), 
			volumen, repeticiones, pan_i, pan_d));
//		Audio::acc_volumen_general_sonido());

	iniciada_reproduccion=true;
}

void Actor_audio::turno(float p_delta)
{
	if(!es_activo()) 
	{

	}
	else
	{
		switch(tipo_reproduccion)
		{
			case T_UNICO:
			case T_REPETIR:
				if(!iniciada_reproduccion)
				{
					iniciar_reproduccion();
				}
				else if(canal.es_vinculado() && !canal.es_reproduciendo())
				{
					liberar_canal();
					desactivar();
				}
			break;

			case T_REPETIR_ALEATORIO:
			case T_REPETIR_INTERVALO:
				tiempo_actual+=p_delta;

				if(tiempo_actual >= tiempo_objetivo)
				{
					if(!iniciada_reproduccion)
					{
						iniciar_reproduccion();
					}
					else if(canal.es_vinculado() && !canal.es_reproduciendo())
					{
						liberar_canal();
						calcular_tiempos();
					}
				}
			break;

			default: return; break;
		}
	}
}
