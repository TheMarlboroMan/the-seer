#include "control_temporizador.h"

Control_temporizador::Control_temporizador()
	:tiempo(0.0), tiempo_maximo(0.0), cadena_actual(0),
	siguiente_temporizador(0)/*, encendido(false)*/ 
{
}

Control_temporizador::Control_temporizador(const Datos_temporizador& p_dt/*, bool p_encendido*/):
	tiempo(0.0), tiempo_maximo(p_dt.tiempo), 
	cadena_actual(p_dt.id_cadena), siguiente_temporizador(p_dt.siguiente_entrada)
	/*, encendido(p_encendido)*/
{
}

bool Control_temporizador::procesar_turno(float p_delta)
{
	tiempo+=p_delta;
	if(tiempo >= tiempo_maximo)	//Los temporizadores no son exactos.
	{
		tiempo=0.0;
		return true;
	}
	
	return false;
}

void Control_temporizador::actualizar_informacion(const Datos_temporizador& p_dt)
{
	cadena_actual=p_dt.id_cadena;
	tiempo_maximo=p_dt.tiempo;
	siguiente_temporizador=p_dt.siguiente_entrada;
}
