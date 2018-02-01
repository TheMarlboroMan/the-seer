#include "interruptor_no_repetible.h"

Interruptor_no_repetible::Interruptor_no_repetible(unsigned int px, unsigned int py, unsigned int pz, 
	unsigned int pid_eslabon, unsigned int pid,
	unsigned int ptc, unsigned int pidn, bool pus)
	:Interruptor(px, py, pz, pid_eslabon, ptc, pidn), usado(pus)
{
	actualizar_volumen();
	actualizar_id_localizador(pid);
}

Interruptor_no_repetible::~Interruptor_no_repetible()
{

}

void Interruptor_no_repetible::transformar_representacion(Bloque_transformacion_representable& br)
{
	br.usar_bitmap();
	br.establecer_recurso_sin_recortar(2);
	Punto_2d p=obtener_coordenadas_origen_representacion();

	if(!usado) br.establecer_recorte(0, 121, 16, 29);
	else br.establecer_recorte(0, 151, 16, 29);

	p.x-=8;
	p.y-=16;
	br.establecer_posicion(p.x, p.y, 16, 29);
}

Datos_interaccion Interruptor_no_repetible::obtener_interaccion()
{
	if(usado) return obtener_interaccion_nula(); //Este viene de "Interactuable".
	else 
	{
		usado=true;
		return obtener_datos_interaccion(/*1*/);
	}
}
