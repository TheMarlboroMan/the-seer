#include "proyectil_obstaculo.h"

Controlador_datos_proyectiles_genericos Proyectil_obstaculo::controlador_datos;

void Proyectil_obstaculo::configurar_propiedades()
{
	switch(direccion)
	{
		case Definiciones::POS_N: establecer_vector(0, (int)-velocidad, 0); break;
		case Definiciones::POS_S: establecer_vector(0, velocidad, 0); break;
		case Definiciones::POS_E: establecer_vector(velocidad, 0, 0); break;
		case Definiciones::POS_O: establecer_vector((int)-velocidad, 0, 0); break;
	}

	const Datos_proyectil_generico& dr=controlador_datos.obtener_por_id(tipo);

	establecer_volumen(dr.acc_w(), dr.acc_h(), dr.acc_d());	
	desaparece_en_colision=dr.es_desaparece_colision();
	tipo_letal=dr.acc_tipo_letal();
}

Proyectil_obstaculo::Proyectil_obstaculo(int px, int py, int pz, 
	unsigned int pt, unsigned int pdir, unsigned int pvel):
	Actor_movil(px, py, pz), tipo(pt), direccion(pdir), velocidad(pvel),
	tiempo(0.0), desaparece_en_colision(false), tipo_letal(T_NADA)
{
	actualizar_volumen();
	configurar_propiedades();
}

Proyectil_obstaculo::~Proyectil_obstaculo()
{

}

void Proyectil_obstaculo::actualizar_volumen()
{
	//En realidad el trabajo se hace en "configurar_propiedades".
}

void Proyectil_obstaculo::transformar_representacion(Bloque_transformacion_representable& br)
{
	br.usar_bitmap();
	Punto_2d p=obtener_coordenadas_origen_representacion();

	Datos_proyectil_generico dr=controlador_datos.obtener_por_id(tipo);
	br.establecer_recurso(dr.acc_indice_recurso());

	unsigned int rw=0, rh=0;

	Frame_representacion fr;
	if(dr.es_estatico()) fr=dr.obtener_frame_unico();
	else fr=dr.obtener_para_tiempo_animacion(tiempo);

	p.x-=fr.correccion_x;
	p.y-=fr.correccion_y;
	rw=fr.w;
	rh=fr.h;

	br.establecer_recorte(fr.x, fr.y, fr.w, fr.h);
	br.establecer_posicion(p.x, p.y, rw, rh);
}
