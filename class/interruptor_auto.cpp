#include "interruptor_auto.h"

Interruptor_auto::Interruptor_auto(int px, int py, int pz, int pid_c, int pid, bool pact, bool pusa):
	Actor(px, py, pz), Activable(pact), id_cadena(pid_c), usado(pusa)
{
	actualizar_volumen();
	actualizar_id_localizador(pid);
}

Interruptor_auto::~Interruptor_auto()
{

}

void Interruptor_auto::actualizar_volumen()
{
	establecer_volumen(0, 0, 0);
}

void Interruptor_auto::usar()
{
	usado=true;	
}
