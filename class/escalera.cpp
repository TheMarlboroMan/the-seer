#include "escalera.h"

Escalera::Escalera(unsigned int px, unsigned int py, unsigned int pz, 
	unsigned int ph, unsigned int por, unsigned int pflags, 
	bool pact, unsigned int pid):
	Actor(px, py, pz), Activable(pact), h(ph), orientacion(por), 
	flags(pflags)
{
	actualizar_volumen();
	actualizar_id_localizador(pid);
}

Escalera::~Escalera()
{

}

Punto Escalera::obtener_posicion_trepar() const
{
	Cubo esc=como_cubo();

	unsigned int x=esc.origen.x;
	unsigned int y=esc.origen.y;
	
	const unsigned int sn=Definiciones::POS_S | Definiciones::POS_N;
	const unsigned int eo=Definiciones::POS_E | Definiciones::POS_N;

	switch(orientacion)
	{
		case Definiciones::POS_S:
		case Definiciones::POS_N:
		case sn:
			++y;
		break;

		case Definiciones::POS_E:
		case Definiciones::POS_O:
		case eo:
			++x;
		break;
	}
	return Punto(x, y, esc.origen.z+esc.h);
}

//Ori se refiere a la dirección del jugador cuando choca.
Punto Escalera::obtener_snap(const Cubo& c, unsigned int ori) const
{
	Punto resultado(0,0,c.origen.z);
	Cubo esc=como_cubo();

	switch(ori)
	{
		case Definiciones::POS_S:
			resultado.x=esc.origen.x;
			resultado.y=esc.origen.y+esc.d;
		break;

		case Definiciones::POS_N:
			resultado.x=esc.origen.x;
			resultado.y=esc.origen.y-c.d;
		break;

		case Definiciones::POS_E:
			resultado.x=esc.origen.x+esc.w;
			resultado.y=esc.origen.y;
		break;

		case Definiciones::POS_O:
			resultado.x=esc.origen.x-c.w;
			resultado.y=esc.origen.y;
		break;
	}

	return resultado;
}

void Escalera::actualizar_volumen()
{
	unsigned int w=0;
	unsigned int d=0;
	
	const unsigned int sn=Definiciones::POS_S | Definiciones::POS_N;
	const unsigned int eo=Definiciones::POS_E | Definiciones::POS_O;

	switch(orientacion)
	{
		case Definiciones::POS_S:
		case Definiciones::POS_N:
		case sn:
			w=DIM_CARA;
			d=DIM_CANTO;
		break;

		case Definiciones::POS_E:
		case Definiciones::POS_O:
		case eo:
//std::cout<<orientacion<<std::endl;
			d=DIM_CARA;
			w=DIM_CANTO;
		break;
	}

	establecer_volumen(w, d, h);
}
