#ifndef ACTOR_CONTROL_DIRECCION
#define ACTOR_CONTROL_DIRECCION

/*Este actor no hace nada de nada excepto transferir el vector de movimiento de
un actor de un lado a otro... Servirá para hacer rebotar en trayectorias 
predefinidas cierto tipo de proyectiles.

Básicamente lo que tiene es una propiedad "direccion" y un método 
"transformar_vector". La dirección indica la nueva dirección a la que iría, 
de acuerdo con el enum de definciones, mientras que el método transformaría el 
vector del actor que se pasase.
*/

#include "actor.h"
#include "visitante.h"

class Actor_control_direccion:public Actor
{
	private:

	unsigned int direccion;

	public:

	/////////////////////////////
	//Métodos propios...

	Actor_control_direccion(unsigned int px, unsigned int py, unsigned int pz, unsigned int pdir);
	~Actor_control_direccion();
	Vector_3d transformar_vector(const Vector_3d v);
	bool es_destruye_proyectil() const {return direccion==Definiciones::POS_ERROR;}
	

	/////////////////////////////////////
	//Implementar espaciable...
	
	public:

	void actualizar_volumen(){establecer_volumen(1, 1, 1);}
	virtual bool es_solido() const {return false;}
	virtual bool es_recibe_sombra() const {return false;}

	/////////////////////////////////////
	//Implementar representable...

	public:

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "control_direccion";}
	virtual void transformar_representacion(Bloque_transformacion_representable& bl) {}

	/////////////////////////////////////
	//Implementar actor.

	public:

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}	
};

#endif
