#ifndef ACTOR_INFO_JUEGO_H
#define ACTOR_INFO_JUEGO_H

/*Un actor que hace que el controlador indique al sistema que
desea arrancar un info juego: el dialogo de pop up con 
texto que da un mensaje rápido. Al ser activado suelta el popup,
y lo volverá a soltar cuando se active de nuevo.

Como otros tantos controles, no es representable.*/

#include "actor.h"
#include "activable.h"
#include "visitante.h"

class Actor_info_juego:public Actor, public Activable
{
	/////////////////////
	// Propio.

	private:

	unsigned int id_info_juego;

	public:

	Actor_info_juego(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int pidd, bool pa, unsigned int pid);
	~Actor_info_juego();

	void reproducir() {desactivar();}
	unsigned int acc_id_info_juego() const {return id_info_juego;}

	////////////////////
	// Representable

	public:

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "actor_info_juego";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br) {}

	///////////////////
	// Espaciable

	public:

	void actualizar_volumen(){establecer_volumen(1, 1, 1);}
	virtual bool es_colisionable() const {return false;}
	virtual bool es_solido() const {return false;}
	virtual bool es_recibe_sombra() const {return false;}

	///////////////////
	// Actor.

	public:

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}

	///////////////////
	//Activable...
};

#endif
