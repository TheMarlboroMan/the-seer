#ifndef ACTOR_PUZZLE_H
#define ACTOR_PUZZLE_H

/*El actor puzzle es un controlador: en si no hace nada, salvo ser activado
y enviar al visitante de turno la señal de que hay que mostrar un puzzle
concreto.

Guarda en si mismo la información acerca del puzzle a iniciar y qué
cadena de acciones debe iniciarse cuando el puzzle se haya resuelto. 
Como los puzzles sólo pueden resolverse una vez guardará el estado
de resolución del mismo (por eso deberíamos colocarlo siempre con los
actores persistentes).
.*/

#include "actor.h"
#include "activable.h"
#include "visitante.h"

class Actor_puzzle:public Actor, public Activable
{
	/////////////////////
	// Propio.

	private:

	enum estados {E_PENDIENTE=0, E_RESUELTO=1};

	unsigned int id_puzzle;
	unsigned int id_cadena;
	unsigned int estado;

	public:

	Actor_puzzle(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int pidp, unsigned int pidc, unsigned int pid, unsigned int);
	~Actor_puzzle();

	void resolver() {estado=E_RESUELTO;}

	unsigned int acc_id_puzzle() const {return id_puzzle;}
	unsigned int acc_id_cadena() const {return id_cadena;}
	bool es_resuelto() const {return estado==E_RESUELTO;}

	////////////////////
	// Representable

	public:

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "actor_puzzle";}
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

	//Para que el puzzle sea activable debe estar pendiente. Esto nos
	//asegura que por mucho que usemos el interruptor, no se arranque
	//el puzzle una vez ha sido finalizado.
	virtual void activar() {if(estado==E_PENDIENTE) Activable::activar();}
};

#endif
