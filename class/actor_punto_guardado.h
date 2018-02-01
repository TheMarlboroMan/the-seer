#ifndef ACTOR_PUNTO_GUARDADO_H
#define ACTOR_PUNTO_GUARDADO_H

/*El punto de guardado es interactuable y guarda el id de salida que se
usará cuando se guarde la partida al activarlo. Tiene poca cosa más.*/

#include "interactuable.h"
#include "visitante.h"
#include "temporizador_animaciones.h"

class Actor_punto_guardado:public Interactuable
{
	//////////////////////////////////
	// Propio.

	private:

	static const int W=5;
	static const int H=20;
	static const int D=5;

	static const int W_REP=8;
	static const int H_REP=29;
	static const int X_REP=238;
	static const int Y_REP=33;

	static const float T_LOOP_ANIM;
	static const int PASOS_ANIMACION=7;
	static float DURACION_PASO_ANIM;

	unsigned int id_entrada;

	public:

	Actor_punto_guardado(unsigned int px, unsigned int py, unsigned int pz, unsigned int ids);
	~Actor_punto_guardado();

	unsigned int acc_id_entrada() const {return id_entrada;}

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}

	//////////////////////////////////
	// Representable

	virtual bool es_representable() const {return true;}
	virtual std::string string_me_about_yourself() const {return "punto_guardado";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br);

	//////////////////////////////////
	// Espaciable.

	public:

	void actualizar_volumen(){establecer_volumen(W, D, H);}
	virtual bool es_recibe_sombra() const {return true;}

	//////////////////////////////////
	// Interactuable

	public:

	virtual Datos_interaccion interactuar() {return obtener_interaccion_nula();}
};

#endif
