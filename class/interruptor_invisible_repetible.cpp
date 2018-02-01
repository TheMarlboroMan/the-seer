#include "interruptor_invisible_repetible.h"

Interruptor_invisible_repetible::Interruptor_invisible_repetible(
		unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int p_w, unsigned int p_h, unsigned int p_d,
		unsigned int pid, unsigned int pid_off, 
		unsigned int ptc, unsigned int pidn, bool penc):
		Interruptor_repetible(px, py, pz, 
			pid, pid_off,
			ptc, pidn, penc), vw(p_w), vh(p_h), vd(p_d)
{
	actualizar_volumen();
}

Interruptor_invisible_repetible::~Interruptor_invisible_repetible()
{

}
