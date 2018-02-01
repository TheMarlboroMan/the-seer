#ifndef TEMPORIZADOR_ANIMACIONES_H
#define TEMPORIZADOR_ANIMACIONES_H

class Bloque;
class Actor_generico;

class Temporizador_animaciones
{
	private:

	static float tiempo_animacion;
	static const float MAX_TIEMPO_ANIMACION;	

	public:

	static void procesar_tiempo_animacion(float p_delta)
	{
		tiempo_animacion+=p_delta;
		if(tiempo_animacion >= MAX_TIEMPO_ANIMACION) tiempo_animacion=0.0;
	}

	friend class Bloque;
	friend class Actor_generico;
	friend class Sombra;
	friend class Actor_punto_guardado;
};

#endif
