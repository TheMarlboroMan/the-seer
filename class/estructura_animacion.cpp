#include "estructura_animacion.h"

Estructura_animacion::Estructura_animacion(DLibV::Representacion& p_rep):
	rep(p_rep), porcentaje(0.0)
{

}

void Estructura_animacion_movimiento::generar_porcentajes()
{
	dist_x=abs(pos_x-dest_x);
	dist_y=abs(pos_y-dest_y);

	dist_tot=dist_x+dist_y;
	
	mult_porcentaje_x=DLibH::Herramientas::porcentaje(dist_x, dist_tot) / 100;
	mult_porcentaje_y=DLibH::Herramientas::porcentaje(dist_y, dist_tot) / 100;
}

void Estructura_animacion_movimiento::calcular_porcentajes()
{
	float dist_rest_x=abs(pos_x-dest_x);
	float dist_rest_y=abs(pos_y-dest_y);

	if(!dist_rest_x && !dist_rest_y)
	{
		porcentaje=100;
	}
	else
	{
		float porc_x=DLibH::Herramientas::porcentaje(dist_x-dist_rest_x, dist_x);
		float porc_y=DLibH::Herramientas::porcentaje(dist_y-dist_rest_y, dist_y);

		porcentaje=(porc_x*mult_porcentaje_x)+(porc_y*mult_porcentaje_y);
	}
}

void Estructura_animacion_movimiento::comprobacion_y_movimiento(float delta, float vector, float &pos, float dest)
{
	//Desplazamiento hacia la derecha / arriba.
	if(vector > 0 && pos < dest)
	{
		pos+=vector*delta;
		if(pos >= dest) pos=dest;
	}
	//esplazamiento hacia la izquierda / abajo.
	else if(vector < 0  && pos > dest)
	{
		pos+=vector*delta;
		if(pos <= dest) pos=dest;
	}
	//No hay desplazamiento
	else
	{

	}
}


Estructura_animacion_movimiento::Estructura_animacion_movimiento(DLibV::Representacion& p_rep, const DLibH::Vector_2d & pv, float dx, float dy):
	Estructura_animacion(p_rep),
	dest_x(dx), dest_y(dy), v(pv), 
	dist_x(0), dist_y(0), dist_tot(0)
{
	pos_x=p_rep.acc_posicion().x;
	pos_y=p_rep.acc_posicion().y;

	generar_porcentajes();
}


void Estructura_animacion_movimiento::turno(float p_delta)
{
	//Horizontal...
	comprobacion_y_movimiento(p_delta, v.x, pos_x, dest_x);
	comprobacion_y_movimiento(p_delta, v.y, pos_y, dest_y);

	rep.establecer_posicion(pos_x, pos_y);

	//Calcular porcentaje.
	calcular_porcentajes();
}

void Estructura_animacion_movimiento::forzar_finalizacion()
{
	pos_x=dest_x;
	pos_y=dest_y;

	rep.establecer_posicion(pos_x, pos_y);

	porcentaje=100;
}

Grupo_animacion::Grupo_animacion(float porc)
	:porcentaje(0), porcentaje_parcial(0), 
	porcentaje_minimo_cancelar(porc), total_animaciones(0)
{

}

Grupo_animacion::~Grupo_animacion()
{
	vaciar();
}

void Grupo_animacion::vaciar()
{
	for(Estructura_animacion * e : animaciones) delete e;	
	animaciones.clear();
	porcentaje=0;
	porcentaje_parcial=0;
	total_animaciones=0;
}
	
void Grupo_animacion::asignar_animacion(Estructura_animacion * temp)
{
	animaciones.push_back(temp);
	++total_animaciones;
}

void Grupo_animacion::turno(float p_delta)
{
	porcentaje_parcial=0;

	for(Estructura_animacion * e : animaciones)
	{
		e->turno(p_delta);

		if(e->es_completa()) porcentaje_parcial+=100;
		else porcentaje_parcial+=e->porcentaje;
	}

	porcentaje=porcentaje_parcial / total_animaciones;
}

bool Grupo_animacion::finalizar()
{
	if(this->porcentaje >= porcentaje_minimo_cancelar)
	{
		this->forzar_finalizacion();
		return true;
	}
	else
	{
		return false;
	}
}

void Grupo_animacion::forzar_finalizacion()
{
	for(Estructura_animacion * e : animaciones) e->forzar_finalizacion();
}

void Grupo_animacion::mostrar(DLibV::Pantalla & pantalla)
{
	for(Estructura_animacion * e : animaciones) e->rep.volcar(pantalla);
}
