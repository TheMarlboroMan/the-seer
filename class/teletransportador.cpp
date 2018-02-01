#include "teletransportador.h"

Teletransportador::Teletransportador(int px, int py, int pz, unsigned int p_id, unsigned int p_idc)
	:Actor(px, py, pz), 
	id_teletransportador(p_id), id_conexion(p_idc)
{
	actualizar_volumen();
}

Punto Teletransportador::obtener_destino() const
{
	Cubo c=como_cubo();
	return Punto(c.origen.x + (W/4), c.origen.y+(W/4), c.origen.z);
}
