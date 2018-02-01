#ifndef INTERRUPTOR_INVISIBLE_REPETIBLE
#define INTERRUPTOR_INVISIBLE_REPETIBLE

/*Realmente no hemos preparado del todo la jerarquía para esto:
este es un interruptor repetible, pero no es visible y tiene un
volumen definido por el usuario. De alguna manera es también
interruptor_invisible_no_repetible, pero nos ahorramos la herencia
en diamante duplicando código y punto.*/

#include "interruptor_repetible.h"

class Interruptor_invisible_repetible:public Interruptor_repetible
{
	////////////////////////////
	// Propio

	private:

	unsigned int vw;
	unsigned int vh;
	unsigned int vd;

	public:

	Interruptor_invisible_repetible(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int p_w, unsigned int p_h, unsigned int p_d,
		unsigned int pid, unsigned int pid_off, unsigned int, unsigned int, bool);
	~Interruptor_invisible_repetible();

	////////////////////////////
	// Espaciable

	void actualizar_volumen(){establecer_volumen(vw, vd, vh);}
	virtual bool es_solido() const {return false;}
	virtual bool es_recibe_sombra() const {return false;}

	////////////////////////////
	// Representbale.

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "interruptor invisible repetible";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br) {}

	////////////////////////////
	// Interruptor

	//Nada... usa el de la clase base.

	////////////////////////////
	// Actor.

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
