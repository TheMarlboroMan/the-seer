#ifndef INTERACTUABLE_H
#define INTERACTUABLE_H

/*Esto es la interfaz de todos aquellos actores que puedan activarse con la
tecla de interacción. Lo único que tienen que hacer es implementar el método
público que devuelve un "datos_interaccion", con el valor que sea y el id
de cadena de acciones a la que llamar.

Al ser parte de la jerarquía de Actor, no hay problema con meterlo en vectores
de actores. Eso si, al no ser parte de la jerarquía de bloques es un poco más
complicado hacer que un bloque interactue.

Para esos casos podemos tener hacks: un actor invisible que haga el mismo 
papel.
*/

#include "actor.h"

struct Datos_interaccion
{
/*
	enum tipos {
		T_NADA=0,	
		T_CADENA_ACCIONES,
		T_TEMPORIZADOR};
*/
	unsigned int id_cadena;	//Id cadena a la que se llamaría...
	//unsigned int valor;	//Valor a pasar. En realidad para los cofres, por ejemplo, lo podemos usar para otra cosa.
//	unsigned int tipo;

	Datos_interaccion(unsigned int pidc=0/*,*/ /*unsigned int pv=0,*/ /*unsigned int pt=0*/)
		:id_cadena(pidc)/*,*/ /*valor(pv),*/ /*tipo(pt)*/
	{}
};

class Interactuable: public Actor
{
	private:

	bool detectable_secreto; //Indica si el detector de secretos lo cuenta.

	protected:
	Interactuable(unsigned int px, unsigned int py, unsigned int pz):
		Actor(px, py, pz), detectable_secreto(false)
	{}

	virtual ~Interactuable(){}

	void hacer_secreto() {detectable_secreto=true;}
	void deshacer_secreto() {detectable_secreto=false;}

	public:	

	virtual Datos_interaccion interactuar()=0;

	Datos_interaccion obtener_interaccion_nula() 
	{
		return Datos_interaccion(0/*,*/ /*0,*/ /*0*/);
	}

	virtual void recibir_senal_interactuable_reactivar() {}

	bool es_detectable_secreto() const {return detectable_secreto;}
};	
#endif
