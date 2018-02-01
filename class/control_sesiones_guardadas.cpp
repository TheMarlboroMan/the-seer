#include "control_sesiones_guardadas.h"


Control_sesiones_guardadas::Control_sesiones_guardadas()
{
	iniciar();
}

void Control_sesiones_guardadas::refrescar()
{
	sesiones.clear();
	iniciar();
}
/*
const Control_sesiones_guardadas::Info_sesion_guardada& Control_sesiones_guardadas::obtener_sesion(unsigned int i) const
{
	return sesiones.at(i);
}
*/

void Control_sesiones_guardadas::iniciar()
{
	unsigned int i=0;

	while(i < MAX_SESIONES)
	{
		Deserializador d(i);
		bool sesion_correcta=false;

		unsigned int segundos=0;
		std::string nombre("");
		if(d.es_valido())
		{
			d.iniciar();
			if(!d.es_error())
			{
				if(d.localizar_seccion(Definiciones_serializador_deserializador::M_TIEMPO_JUEGO))
				{
					Tiempo_juego t;
					d.deserializar(t);
					
					if(!d.es_error())
					{
						segundos=t.acc_segundos_restantes();
						nombre=d.acc_nombre();
						sesion_correcta=true;
					}					
				}
			}
		}

		d.finalizar();

		if(!sesion_correcta) sesiones.push_back(Info_sesion_guardada(i));
		else sesiones.push_back(Info_sesion_guardada(i, nombre, segundos));
		++i;
	}
}

bool Control_sesiones_guardadas::es_sesion_usada(unsigned int i) const
{
	try
	{
		const Info_sesion_guardada& isg=sesiones.at(i);
		return !isg.es_libre();
	}
	catch(std::out_of_range& e)
	{}

	return false;
}

void Control_sesiones_guardadas::eliminar_sesion(unsigned int i)
{
	try
	{
		Info_sesion_guardada& isg=sesiones.at(i);
		const std::string ruta=Definiciones_serializador_deserializador::construir_ruta_archivo(isg.acc_indice());
		remove(ruta.c_str());
		isg.anular();
	}
	catch(std::out_of_range& e)
	{}


}
