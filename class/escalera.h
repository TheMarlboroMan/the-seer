#ifndef ESCALERA_H
#define ESCALERA_H

#include "actor.h"
#include "activable.h"
#include "visitante.h"

class Escalera:public Actor, public Activable
{
	///Propiedades

	public:

	enum flags {F_TOPE_ESCALABLE=1, F_BASE_INVERTIR=2};

	private:

	static const unsigned int DIM_CARA=8;
	static const unsigned int DIM_CANTO=2;

	unsigned int h;
	unsigned int orientacion; //La cara escalable.
	unsigned int flags;
	
	public:

	/////////////////////////////
	//Métodos propios...

	Escalera(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int ph, unsigned int por, unsigned int pflags, 
		bool pact, unsigned int pid);
	~Escalera();

	bool es_base_invertir() const {return flags & F_BASE_INVERTIR;}
	bool es_tope_escalable() const {return flags & F_TOPE_ESCALABLE;}

	bool es_nula() const {return !h;}
	void hacer_nula() {h=0;}
	
	Punto obtener_posicion_trepar() const;
	Punto obtener_snap(const Cubo&, unsigned int ori) const;
	unsigned int acc_orientacion() const {return orientacion;}
	unsigned int acc_flags() const {return flags;}

	/////////////////////////////////////
	//Implementar espaciable...
	
	public:

	void actualizar_volumen();
	virtual bool es_colisionable() const {return es_activo() && !es_nula();}
	virtual bool es_solido() const {return es_activo() && !es_nula();}
	virtual bool es_recibe_sombra() const {return false;}

	/////////////////////////////////////
	//Implementar representable...

	public:

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "escalera";}	
	virtual void transformar_representacion(Bloque_transformacion_representable& bl) {}

	/////////////////////////////////////
	//Implementar actor.

	public:

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}	
};

#endif
