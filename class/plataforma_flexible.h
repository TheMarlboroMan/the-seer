#ifndef PLATAFORMA_FLEXIBLE_H
#define PLATAFORMA_FLEXIBLE_H

/*Obstáculo de tamaño definido por el editor, sólida y posiblemente letal.*/

#include "actor.h"
#include "visitante.h"

class Plataforma_flexible:public Actor
{
	private:

	unsigned int w;
	unsigned int h;
	unsigned int d;
	bool letal;
	bool solido;

	////////////////////////////////////
	//Métodos propios...
	
	private:

	public:

	Plataforma_flexible(int px, int py, int pz, int pw, int ph, int pd, bool pl, bool sl);
	~Plataforma_flexible();
	bool es_letal() const {return letal;}

	////////////////////////////////////
	//Implementación de representable...
	
	public:

	virtual bool es_representable() const {return false;}
	virtual void transformar_representacion(Bloque_transformacion_representable&) {}

	////////////////////////////////////
	//Implementación de espaciable.

	virtual bool es_colisionable() const {return true;}
	virtual bool es_solido() const {return solido;}
	virtual bool es_recibe_sombra() const {return false;}

	virtual std::string string_me_about_yourself() const {return "plataforma flexible";}

	////////////////////////////////////	
	//Implementación de actor.
	virtual void actualizar_volumen();
	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
