#include "actor_control_direccion.h"

Actor_control_direccion::Actor_control_direccion(unsigned int px, unsigned int py, unsigned int pz, unsigned int pdir)
	:Actor(px, py, pz), direccion(pdir)
{
	actualizar_volumen();
}

Actor_control_direccion::~Actor_control_direccion() 
{

}

/*La transformación es una chorrada: se suman todos los vectores y
el resultado se pone en un vector sólo con la dirección de turno.*/

Vector_3d Actor_control_direccion::transformar_vector(const Vector_3d v)
{
	Vector_3d resultado;

	float suma=abs(v.x+v.y+v.z);

	switch(direccion)
	{
		case Definiciones::POS_N: resultado.y=-suma; break;
		case Definiciones::POS_S: resultado.y=suma; break;
		case Definiciones::POS_E: resultado.x=suma; break;
		case Definiciones::POS_O: resultado.x=-suma; break;
		case Definiciones::POS_ARR: resultado.z=suma; break;
		case Definiciones::POS_ABA: resultado.z=-suma; break;
	}

	return resultado;
}
