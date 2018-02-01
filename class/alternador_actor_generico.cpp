#include "alternador_actor_generico.h"

Alternador_actor_generico::Alternador_actor_generico(
	unsigned int px, unsigned int py, unsigned int pz, 
	unsigned int pt, unsigned int pp, 
	unsigned int pid_enl_on, unsigned int pid_enl_off, 
	bool pa, unsigned int pact, unsigned int pid):
	Actor(px, py, pz), Activable(pa), ref_actor(NULL), 
	tipo_actor(pt), patron(pp), 
	id_enlace_activacion(pid_enl_on), 
	id_enlace_desactivacion(pid_enl_off), 
	paso_actual(pact), total_pasos(0), 
	estado_paso_actual(E_APAGADO), tiempo_paso_actual(0.0), 
	tiempo_max_paso(0.0), activo_anterior(!pa)
{
	actualizar_volumen();
	actualizar_id_localizador(pid);
	cargar_patron_pasos();
}
	
Alternador_actor_generico::~Alternador_actor_generico()
{
	ref_actor=NULL;
}

void Alternador_actor_generico::cargar_patron_pasos()
{
	const Datos_patron_disparo& dp=Controlador_patrones_disparo::controlador_patrones_disparo.obtener_por_id(patron);
	total_pasos=dp.obtener_total_pasos();
	cargar_info_paso();
}

void Alternador_actor_generico::cargar_info_paso()
{
	const Datos_patron_disparo& dp=Controlador_patrones_disparo::controlador_patrones_disparo.obtener_por_id(patron);
	const Info_patron_disparo& d=dp.obtener_paso(paso_actual);
	estado_paso_actual=d.estado;
	tiempo_max_paso=float(d.pausa_ms) / 1000.0;
	if(ref_actor) actualizar_estado_actor();
}

void Alternador_actor_generico::actualizar_estado_actor()
{
	if(!ref_actor) 
	{
		return;
	}
	else
	{
		if(!es_activo()) 
		{
			ref_actor->hacer_ausente();
		}
		else
		{
			if(estado_paso_actual==E_ENCENDIDO) 
			{
				ref_actor->hacer_presente();
			}
			else 
			{
				ref_actor->hacer_ausente();
			}
		}
	}
}

void Alternador_actor_generico::turno(float p_delta)
{
//std::cout<<es_activo()<<std::endl;
	activo_anterior=estado_paso_actual==E_ENCENDIDO; //es_activo();
	if(!es_activo()) return;

	tiempo_paso_actual+=p_delta;

	if(tiempo_paso_actual > tiempo_max_paso)
	{
		tiempo_paso_actual=0.0;

		++paso_actual;
		if(paso_actual >= total_pasos) paso_actual=0;
		cargar_info_paso();
	}
}
