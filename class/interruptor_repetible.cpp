#include "interruptor_repetible.h" 

Interruptor_repetible::Interruptor_repetible(unsigned int px, unsigned int py, unsigned int pz, 
	unsigned int pid, unsigned int pid_off, 
	unsigned int ptc, unsigned int pidn, bool penc
)
	:Interruptor(px, py, pz, pid, ptc, pidn), 
	id_eslabon_cadena_off(pid_off),
	encendido(penc)
{
	actualizar_volumen();
}

Interruptor_repetible::~Interruptor_repetible()
{ 

}

void Interruptor_repetible::transformar_representacion(Bloque_transformacion_representable& br)
{
	br.usar_bitmap();
	br.establecer_recurso_sin_recortar(2);
	Punto_2d p=obtener_coordenadas_origen_representacion();

	if(encendido) br.establecer_recorte(0, 61, 16, 29);
	else br.establecer_recorte(0, 91, 16, 29);

	p.x-=8;
	p.y-=16;
	br.establecer_posicion(p.x, p.y, 16, 29);
}

Datos_interaccion Interruptor_repetible::obtener_interaccion()
{
	encendido=!encendido;
	/*unsigned int valor=encendido ? 1 : 0;*/
	unsigned int idc=encendido ? acc_id_eslabon_cadena() : id_eslabon_cadena_off;
	return obtener_datos_interaccion(/*valor, */idc);
}
