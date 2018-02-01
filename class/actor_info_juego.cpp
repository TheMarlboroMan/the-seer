#include "actor_info_juego.h"

Actor_info_juego::Actor_info_juego(unsigned int px, unsigned int py, unsigned int pz, 
	unsigned int pidd, bool pa, unsigned int pid):
	Actor(px, py, pz), Activable(pa),
	id_info_juego(pidd)
{
	actualizar_id_localizador(pid);
}

Actor_info_juego::~Actor_info_juego()
{

}

/*
void Actor_info_juego::turno(float p_delta)
{
	
}*/
