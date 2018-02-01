#ifndef INTERRUPTOR_AUTO
#define INTERRUPTOR_AUTO

/*Cuando llega su turno comprobamos que está activo y no ha sido usado.
Si es el caso, se dispara la cadena y se marca como usado.
Cada vez que se inserta en una sala se marca como "no usado" de forma
que si es persistente, vuelva a llamar a la cadena de turno.*/

#include "actor.h"
#include "activable.h"
#include "visitante.h"

class Interruptor_auto:public Actor, public Activable
{
	private:

	unsigned int id_cadena;
	bool usado;

	////////////////////////////////////
	//Métodos propios...
	

	public:

	Interruptor_auto(int px, int py, int pz, int pid_c, int pid, bool pact, bool pusa);
	~Interruptor_auto();

	unsigned int acc_id_cadena() const {return id_cadena;}
	bool es_usado() const {return usado;}
	void usar();
	void marcar_como_no_usado() {usado=false;}

	////////////////////////////////////
	//Implementación de representable...
	
	public:

	virtual bool es_representable() const {return false;}
	virtual void transformar_representacion(Bloque_transformacion_representable&) {}

	////////////////////////////////////
	//Implementación de espaciable.

	virtual bool es_colisionable() const {return false;}
	virtual bool es_solido() const {return false;}
	virtual bool es_recibe_sombra() const {return false;}

	virtual std::string string_me_about_yourself() const {return "interruptor auto";}

	////////////////////////////////////	
	//Implementación de actor.
	virtual void actualizar_volumen();
	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
