#include "actor.h"

Actor::Actor(int px, int py, int pz):
	Representable(),
	Espaciable(px, py, pz), id_localizador(0), marcado_para_borrar(false),
	coordenadas_origen_rep(0,0)
{
}

Actor::~Actor()
{
}

Actor::Actor(const Actor& otro)
	:Representable(otro), Espaciable(otro), 
	coordenadas_origen_rep(otro.coordenadas_origen_rep)
{}

Actor& Actor::operator=(const Actor& otro)
{
	Representable::operator=(otro);
	Espaciable::operator=(otro);
	coordenadas_origen_rep=otro.coordenadas_origen_rep;
	return *this;
}

/*Tratamiento de la caché de la posición en pantalla con la
finalidad de acelerar el proceso de representación al no 
computar en cada paso.*/

Punto_2d Actor::obtener_coordenadas_origen_representacion()
{
	//Al estar sometido a gravedad el jugador pasará por aquí
	//a cada turno.

	if(es_cambio_posicion())
	{
		actualizar_coordenadas_representacion();
	}

	return coordenadas_origen_rep;
}

/*Actualiza la posición en pantalla cacheada. Es público porque en el caso de
centrar la cámara querremos asegurarnos de que todo está bien centrado.*/

void Actor::actualizar_coordenadas_representacion()
{
	coordenadas_origen_rep=obtener_coordenadas_representacion(
		obtener_x_representable(),
		obtener_y_representable(),
		obtener_z_representable());
	reiniciar_cambio_posicion();
}

bool Actor::operator!=(const Actor& otro) const
{
	return this != &otro;
}

bool Actor::rep_final_es_dentro_de_caja(int dx, int dy, const Bloque_transformacion_representable& bt, const SDL_Rect& caja) const
{
	SDL_Rect bloque=bt.acc_posicion();
	bloque.x+=dx;
	bloque.y+=dy;
	return DLibH::Herramientas_SDL::rectangulos_superpuestos(bloque, caja); 
}

void Actor::marcar_para_borrar()
{
	marcado_para_borrar=true;
}
