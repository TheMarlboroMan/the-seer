#include "actor_puzzle.h"

Actor_puzzle::Actor_puzzle(
	unsigned int px, unsigned int py, unsigned int pz, 
	unsigned int pidp, unsigned int pidc, unsigned int pid,
	unsigned int pest):
	Actor(px, py, pz), Activable(false),
	id_puzzle(pidp), id_cadena(pidc), estado(pest)
{
	actualizar_id_localizador(pid);
}

Actor_puzzle::~Actor_puzzle()
{

}
