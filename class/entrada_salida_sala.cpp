#include "entrada_salida_sala.h"

Entrada_salida::Entrada_salida(int px, int py, int pz, unsigned int p_entrada):
	Actor(px, py, pz), id_entrada(p_entrada)
{
	actualizar_volumen();
}

Entrada_salida::Entrada_salida(const Entrada_salida& otro)
	:Actor(otro), id_entrada(otro.id_entrada)
{}

Entrada_salida& Entrada_salida::operator=(const Entrada_salida& otro)
{
	Actor::operator=(otro);
	id_entrada=otro.id_entrada;
	return *this;
}	

Entrada_salida::~Entrada_salida()
{

}

/////////////////////////////////////////
//////// Salida sala...

Salida_sala::Salida_sala(int px, int py, int pz, unsigned int pw, unsigned int ph, 
	unsigned int pd, unsigned int p_entrada, unsigned int p_sala):
	Entrada_salida(px, py, pz, p_entrada), id_sala(p_sala),
	vw(pw), vh(ph), vd(pd)
{
	actualizar_volumen();
}

Salida_sala::~Salida_sala()
{}

/////////////////////////////////////////
///////// Entrada sala...

Entrada_sala::Entrada_sala(int px, int py, int pz, unsigned int p_entrada):
	Entrada_salida(px, py, pz, p_entrada)
{
	actualizar_volumen();
}

Entrada_sala::Entrada_sala(const Entrada_sala& otro):Entrada_salida(otro)
{

}

Entrada_sala& Entrada_sala::operator=(const Entrada_sala& otro)
{
	Entrada_salida::operator=(otro);
	return *this;
}

Entrada_sala::~Entrada_sala()
{

}
