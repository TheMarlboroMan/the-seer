#include "actor_generico.h"

void Actor_generico::actualizar_volumen() 
{
	Datos_actor_generico dr=controlador_datos.obtener_por_id(tipo_actor_generico);
	establecer_volumen(dr.acc_w(),dr.acc_h(),dr.acc_d());
}

void Actor_generico::actualizar_propiedades()
{
	Datos_actor_generico dr=controlador_datos.obtener_por_id(tipo_actor_generico);
	flags=dr.acc_flags();
	efecto_colision=dr.acc_efecto_colision();
}

Actor_generico::Actor_generico(int px, int py, int pz, int pt):
	Actor(px, py, pz), tipo_actor_generico(pt), flags(0),
	efecto_colision(0)
{
	actualizar_volumen(); 
	actualizar_propiedades();
}

Actor_generico::~Actor_generico() 
{

}

void Actor_generico::transformar_representacion(Bloque_transformacion_representable& br)
{
	br.usar_bitmap();

	//Obtener coordenadas de origen.
	Punto_2d p=obtener_coordenadas_origen_representacion();

	Datos_actor_generico dr=controlador_datos.obtener_por_id(tipo_actor_generico);
	br.establecer_recurso(dr.acc_indice_recurso());

	unsigned int rw=0, rh=0;

	Frame_representacion fr;
	if(dr.es_estatico()) fr=dr.obtener_frame_unico();
	else fr=dr.obtener_para_tiempo_animacion(Temporizador_animaciones::tiempo_animacion);

	p.x-=fr.correccion_x;
	p.y-=fr.correccion_y;
	rw=fr.w;
	rh=fr.h;

	br.establecer_recorte(fr.x, fr.y, fr.w, fr.h);
	br.establecer_posicion(p.x, p.y, rw, rh);
}

Controlador_datos_actores_genericos Actor_generico::controlador_datos;
