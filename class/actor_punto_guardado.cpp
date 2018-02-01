#include "actor_punto_guardado.h"

const float Actor_punto_guardado::T_LOOP_ANIM=1.4;
float 	Actor_punto_guardado::DURACION_PASO_ANIM=0.0;

Actor_punto_guardado::Actor_punto_guardado(
	unsigned int px, unsigned int py, unsigned int pz, unsigned int ids)
		:Interactuable(px, py, pz), id_entrada(ids)
{
	DURACION_PASO_ANIM=T_LOOP_ANIM / PASOS_ANIMACION;
	actualizar_volumen();
}

Actor_punto_guardado::~Actor_punto_guardado()
{

}

void Actor_punto_guardado::transformar_representacion(Bloque_transformacion_representable& br)
{
	br.usar_bitmap();
	br.establecer_recurso_sin_recortar(2);
	Punto_2d p=obtener_coordenadas_origen_representacion();

	//Obtenemos un valor entre 0 y T_LOOP_ANIM.
	float t=fmod(Temporizador_animaciones::tiempo_animacion, T_LOOP_ANIM);
	unsigned int paso=t / DURACION_PASO_ANIM;
	if(paso > 3) paso=7-paso; //Con esto "damos la vuelta".

	int y=Y_REP + (paso * H_REP);

	br.establecer_recorte(X_REP, y, W_REP, H_REP);
	p.x-=4;
	p.y-=18;
	br.establecer_posicion(p.x, p.y, W_REP, H_REP);
}
