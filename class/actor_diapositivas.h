#ifndef ACTOR_DIAPOSITIVAS_H
#define ACTOR_DIAPOSITIVAS_H

/*Un actor que hace que el controlador indique al sistema que
desea arrancar una diapositiva.*/

#include "actor.h"
#include "activable.h"
#include "visitante.h"

class Actor_diapositivas:public Actor, public Activable
{
	/////////////////////
	// Propio.

	private:

	unsigned int id_diapositiva;
	bool reproducido;

	public:

	Actor_diapositivas(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int pidd, bool pa, unsigned int pid, bool prep);
	~Actor_diapositivas();

	void reproducir() {reproducido=true;}
	unsigned int acc_id_diapositiva() const {return id_diapositiva;}
	bool es_reproducido() const {return reproducido;}

	////////////////////
	// Representable

	public:

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "actor_diapositiva";}

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
