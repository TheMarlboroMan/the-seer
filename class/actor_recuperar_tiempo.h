#ifndef ACTOR_RECUPERAR_TIEMPO_H
#define ACTOR_RECUPERAR_TIEMPO_H

/*Actúa como un interruptor que invoca en el controlador el tema de recuperar
el tiempo. No tiene representación, en su lugar podemos construir la que 
queramos.*/

#include "interactuable.h"
#include "visitante.h"

class Actor_recuperar_tiempo:public Interactuable
{
	//////////////////////////////////
	// Propio.

	private:

	public:

	Actor_recuperar_tiempo(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int pw, unsigned int ph, unsigned int pd);
	~Actor_recuperar_tiempo();

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}

	//////////////////////////////////
	// Representable

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "actor_recuperar_tiempo ";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br) {}

	//////////////////////////////////
	// Espaciable.

	public:

	void actualizar_volumen(){/*Fuck this crap*/}
	virtual bool es_recibe_sombra() const {return false;}
	virtual bool es_solido() const {return false;}

	//////////////////////////////////
	// Interactuable

	public:

	virtual Datos_interaccion interactuar() {return obtener_interaccion_nula();}
};

#endif
