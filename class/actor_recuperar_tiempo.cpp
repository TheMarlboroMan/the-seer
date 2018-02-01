#include "actor_recuperar_tiempo.h"

Actor_recuperar_tiempo::Actor_recuperar_tiempo(
	unsigned int px, unsigned int py, unsigned int pz, 
	unsigned int pw, unsigned int ph, unsigned int pd)
		:Interactuable(px, py, pz)
{
	establecer_volumen(pw, pd, ph);
}

Actor_recuperar_tiempo::~Actor_recuperar_tiempo()
{

}
