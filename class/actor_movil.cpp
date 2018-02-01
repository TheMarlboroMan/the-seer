#include "actor_movil.h"


Actor_movil::Actor_movil(int px, int py, int pz):
	Actor(px, py, pz), vector(0.0, 0.0, 0.0)
{
}

Actor_movil::~Actor_movil()
{

}

void Actor_movil::activar_movimiento(float p_delta, unsigned int flags)
{
	float x=flags & Definiciones::X ? vector.x * p_delta : 0.0;
	float y=flags & Definiciones::Y ? vector.y * p_delta : 0.0;
	float z=flags & Definiciones::Z ? vector.z * p_delta : 0.0;

	//Una limitación interesante... la velocidad no puede
	//ser mayor que la dimensión del objeto, para evitarnos los 
	//scans en el prototipo.		

	if(x > acc_w()) x=acc_w();
	if(y > acc_d()) y=acc_d();
	if(z > acc_w()) z=acc_h();

	sumar_posicion(x, y, z);
}

void Actor_movil::establecer_vector(float vx, float vy, float vz)
{
	vector.x=vx;
	vector.y=vy;
	vector.z=vz;
}

void Actor_movil::establecer_eje_vector(float v, unsigned int eje)
{
	if(eje==Definiciones::X) vector.x=v;
	else if(eje==Definiciones::Y) vector.y=v;
	else if(eje==Definiciones::Z) vector.z=v;
}

void Actor_movil::invertir_vector(unsigned int pflags)
{
	if(pflags & Definiciones::X) vector.x=-vector.x;
	if(pflags & Definiciones::Y) vector.y=-vector.y;
	if(pflags & Definiciones::Z) vector.z=-vector.z;
}

//Añadir cantidades al vector de movimiento.
void Actor_movil::sumar_vector(float val, unsigned int flags)
{
	auto procesar=[](float &v, float val) //, float max)
	{
		v+=val;
/*		if(max)
		{
			if(v < 0 && v < -max) v=-max;
			else if(v > 0 && v > max) v=max;
		}
*/
	};

	if(flags & Definiciones::X) procesar(vector.x, val); //, vector_limite(Definiciones::X));
	if(flags & Definiciones::Y) procesar(vector.y, val); //, vector_limite(Definiciones::Y));
	if(flags & Definiciones::Z) procesar(vector.z, val); //, vector_limite(Definiciones::Z));

//		if(flags & Definiciones::X) procesar(vector.x, val, 64);
//		if(flags & Definiciones::Y) procesar(vector.y, val, 64);
//		if(flags & Definiciones::Z) procesar(vector.z, val, 200);
}

void Actor_movil::cancelar_vector(unsigned int pflags)
{
	if(pflags & Definiciones::X) vector.x=0.0;
	if(pflags & Definiciones::Y) vector.y=0.0;
	if(pflags & Definiciones::Z) vector.z=0.0;
}

//Frenar los vectores de movimiento.
void Actor_movil::decelerar_vector(float valor, unsigned int flags)
{
	auto procesar=[](float &v, float f)
	{
		if(v < 0 && (v+=f) > 0) v=0;
		else if(v > 0 && (v-=f) < 0) v=0;
	};

	if(flags & Definiciones::X) procesar(vector.x, valor);
	if(flags & Definiciones::Y) procesar(vector.y, valor);
	if(flags & Definiciones::Z) procesar(vector.z, valor);
}

float Actor_movil::vector_para_eje(unsigned int p_eje) const
{
	switch(p_eje)
	{
		case Definiciones::X: return vector.x; break;
		case Definiciones::Y: return vector.y; break;
		case Definiciones::Z: return vector.z; break;
		default: return 0.0;
	}
}

bool Actor_movil::recibir_datos_colision(const Datos_colision& datos_colision)
{
	float cantidad=datos_colision.obtener_cantidad_con_signo();

	if(cantidad)
	{
		//Realizamos la corrección poniendo la posición final
		//como enteros... No necesitamos más.

		float pos_final=DLibH::Herramientas::round(datos_colision.posicion_corregida);
		establecer_posicion_por_eje(pos_final, datos_colision.eje);
		cancelar_vector(datos_colision.eje);

		if(datos_colision.es_con_correccion_z()) 
		{
			establecer_posicion_por_eje(datos_colision.correccion_z, Definiciones::Z);
			if(vector.z <=0) cancelar_vector(Definiciones::Z);
		}
//std::cout<<"actor_movil recibe col "<<Definiciones::traducir_eje(datos_colision.eje)<<" "<<pos_final<<std::endl;
		
		return true;
	}
	else
	{
		if(datos_colision.es_con_correccion_z()) 
		{
//std::cout<<"el siguiente con z"<<std::endl;
			establecer_posicion_por_eje(datos_colision.correccion_z, Definiciones::Z);
			if(vector.z <=0) cancelar_vector(Definiciones::Z);
			return true;
		}
		
		return false;
	}
}
