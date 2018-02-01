#include "datos_colision.h"

Datos_colision::Datos_colision(unsigned int pe, bool p_con_solido)
	:tipo_colision(0), eje(pe), con_solido(p_con_solido), cantidad(0), 
	vector(0), posicion(0), 
	posicion_corregida(0),
	correccion_z(-1), //habilita_snap(false),
	cubo_a_original(0,0,0,0,0,0),
	cubo_a(0,0,0,0,0,0),
	cubo_b(0,0,0,0,0,0)
{

}

Datos_colision::Datos_colision(const Datos_colision& dc)
	:tipo_colision(dc.tipo_colision), eje(dc.eje), con_solido(dc.con_solido), 
	cantidad(dc.cantidad), vector(dc.vector), 
	posicion(dc.posicion), posicion_corregida(dc.posicion_corregida), 
	correccion_z(dc.correccion_z), //habilita_snap(dc.habilita_snap),
	cubo_a_original(dc.cubo_a_original), cubo_a(dc.cubo_a), cubo_b(dc.cubo_b)
{}

Datos_colision& Datos_colision::operator=(const Datos_colision& dc)
{
	tipo_colision=dc.tipo_colision;
	eje=dc.eje;
	con_solido=dc.con_solido;
	cantidad=dc.cantidad;
	vector=dc.vector;
	posicion=dc.posicion;
	posicion_corregida=dc.posicion_corregida;
	correccion_z=dc.correccion_z;
	//habilita_snap=dc.habilita_snap;
	cubo_a_original=dc.cubo_a_original;
	cubo_a=dc.cubo_a;
	cubo_b=dc.cubo_b;
	return *this;
}

float Datos_colision::obtener_cantidad_con_signo() const
{
	switch(posicion)
	{
		case Definiciones::POS_ARR:
		case Definiciones::POS_E:
		case Definiciones::POS_S:
			return -1.0 * cantidad; //Si ponemos -cantidad podemos sacar... -0 XD!.
		break;

		case Definiciones::POS_ABA:
		case Definiciones::POS_O:
		case Definiciones::POS_N:
			return cantidad;
		break;
		default: return 0.0;
	}
}

void Datos_colision::obtener_posicion_por_vector() 
{
	//TODO: Nada de esto es necesario ya... Se puede sacar de los dos cubos.

	switch(eje)
	{
		case Definiciones::X: 
			posicion=vector < 0 ? Definiciones::POS_O : Definiciones::POS_E;
		break;
		case Definiciones::Y:
			posicion=vector < 0 ? Definiciones::POS_N : Definiciones::POS_S;
		break;
		case Definiciones::Z:
			posicion=vector < 0 ? Definiciones::POS_ABA : Definiciones::POS_ARR;
		break;
	}
}
