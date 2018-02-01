#include "interruptor_invisible_no_repetible.h"

Interruptor_invisible_no_repetible::Interruptor_invisible_no_repetible(unsigned int px, unsigned int py, unsigned int pz,
	unsigned int pw, unsigned int ph, unsigned int pd,  
	unsigned int pid, bool sec, unsigned int pt_c,
	unsigned int pid_ecn, bool pus)
	:Interruptor(px, py, pz, pid, pt_c, pid_ecn), usado(pus), vw(pw), vh(ph), vd(pd)
{
	actualizar_volumen();
	if(sec) hacer_secreto();
}

Interruptor_invisible_no_repetible::~Interruptor_invisible_no_repetible()
{}

Datos_interaccion Interruptor_invisible_no_repetible::obtener_interaccion()
{
	if(usado) return obtener_interaccion_nula(); //Este viene de "Interactuable".
	else
	{
		usado=true;
		deshacer_secreto(); //Sea secreto o no.
		return obtener_datos_interaccion();
	}
}
