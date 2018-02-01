#include "actor_diapositivas.h"

Actor_diapositivas::Actor_diapositivas(unsigned int px, unsigned int py, unsigned int pz, 
	unsigned int pidd, bool pa, unsigned int pid, bool prep):
	Actor(px, py, pz), Activable(pa),
	id_diapositiva(pidd), reproducido(prep)
{
	actualizar_id_localizador(pid);
}

Actor_diapositivas::~Actor_diapositivas()
{

}

/*
void Actor_diapositivas::turno(float p_delta)
{
	
}*/
