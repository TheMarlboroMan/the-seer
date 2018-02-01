#include "audio.h"

DLibA::Controlador_audio_SDL * Audio::controlador_audio=NULL;
DLibA::Cola_sonido Audio::cola_sonido;
std::vector<unsigned int> Audio::canales_detenidos;

bool Audio::inicializar_entorno_audio(int pratio, int psalidas, int pbuffers, int pcanales)
{
	DLibA::Controlador_audio_SDL::configurar_ratio(pratio);
	DLibA::Controlador_audio_SDL::configurar_salidas(psalidas);
	DLibA::Controlador_audio_SDL::configurar_buffers(pbuffers);
	DLibA::Controlador_audio_SDL::configurar_canales_audio(pcanales);

	controlador_audio=DLibA::Controlador_audio_SDL::obtener();

	if(!controlador_audio->iniciar())
	{
		controlador_audio=NULL;
		return false;
	}
	else
	{
		return true;
	}
}

DLibA::Canal_audio Audio::obtener_canal_libre()
{	
	if(!controlador_audio)
	{
		throw Excepcion_audio("Error al solicitar canal: el sistema de audio no ha sido inicializado");
	}
	else
	{
		return controlador_audio->obtener_canal_libre();
	}
}

/*Pausa todos los canales.*/

void Audio::pausar()
{
	if(!controlador_audio)
	{
		throw Excepcion_audio("Error al pausar: el sistema de audio no ha sido inicializado");
	}
	else
	{
		controlador_audio->pausar_sonido();
	}
}

/*Pausa sólo aquellos canales que ahora mismo no estén libres. Guarda los 
índices de los canales para poderlos recuperar más adelante.*/

void Audio::pausar_canales_activos()
{
	if(!controlador_audio)
	{
		throw Excepcion_audio("Error al pausar: el sistema de audio no ha sido inicializado");
	}
	else
	{
		unsigned int canales=controlador_audio->acc_canales_audio();
		unsigned int i=0;

		while(i < canales)
		{
			DLibA::Canal_audio c=controlador_audio->obtener_canal(i);
			if(c.es_reproduciendo())
			{
				c.pausar();
				canales_detenidos.push_back(i);				
			}
			++i;
		}
	}
}

/*Todos los sonidos vuelven a reproducirse...*/

void Audio::despausar()
{
	if(!controlador_audio)
	{
		throw Excepcion_audio("Error al despausar: el sistema de audio no ha sido inicializado");
	}
	else
	{
		controlador_audio->reanudar_sonido();
	}

	canales_detenidos.clear();
}

/*Los sonidos en los canales activos que se pausaron vuelven a reproducirse. El
buffer de canales guardados se vacía para permitir usar el sistema de nuevo.*/

void Audio::despausar_canales_activos()
{
	if(!controlador_audio)
	{
		throw Excepcion_audio("Error al pausar: el sistema de audio no ha sido inicializado");
	}
	else
	{
		for(unsigned int i : canales_detenidos)
		{
			DLibA::Canal_audio c=controlador_audio->obtener_canal(i);
			c.reanudar();
		}
	}
	
	canales_detenidos.clear();
}

/*Reanuda los sonidos pausados que no fueran guardados mediante pausar_activos.*/

void Audio::despausar_excepto_activos()
{
	if(!controlador_audio)
	{
		throw Excepcion_audio("Error al pausar: el sistema de audio no ha sido inicializado");
	}
	else
	{
		unsigned int canales=controlador_audio->acc_canales_audio();
		unsigned int i=0;

		if(!canales_detenidos.size())
		{
			controlador_audio->reanudar_sonido();
		}
		else while(i < canales)
		{
			if(std::find(canales_detenidos.begin(),
					canales_detenidos.end(),
					i)==canales_detenidos.end())
			{
				DLibA::Canal_audio c=controlador_audio->obtener_canal(i);
				c.reanudar();
			}

			++i;
		}
	}
}
