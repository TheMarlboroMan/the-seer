#ifndef ALTERNADOR_ACTOR_GENERICO_H
#define ALTERNADOR_ACTOR_GENERICO_H

/*Este actor tiene una referencia a un actor genérico que se crea fuera de su
ámbito y se asigna posteriormente. A partir de ese momento puede usar los 
patrones para encenderlo y apagarlo, con lo que tenemos obstáculos que se
encienden y apagan...

Como genera un nuevo actor y guarda una referencia hemos ideado un visitante 
para limpiar la referencia al actor cuando salimos de una sala. La propia sala
hace delete.
*/

#include "actor.h"
#include "activable.h"
#include "visitante.h"
#include "actor_generico.h"
#include "datos_legibles.h"

class Alternador_actor_generico:public Actor, public Activable
{
	/////////////////////
	// Propio.

	private:


	enum estados {E_APAGADO=0, E_ENCENDIDO=1};

	Actor_generico * ref_actor; //No será propietario, pero guardará referencia.
		
	unsigned int tipo_actor;
	unsigned int patron;
	unsigned int id_enlace_activacion;
	unsigned int id_enlace_desactivacion;
	unsigned int paso_actual;

	unsigned int total_pasos;
	int estado_paso_actual; //Tiene signo. Se inicia como 100, que sería no hacer nada.
	float tiempo_paso_actual;
	float tiempo_max_paso;

	bool activo_anterior; //Lleva el tracking del estado en el turno anterior.
		
	void cargar_patron_pasos();
	void cargar_info_paso();

	public:

	Alternador_actor_generico(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int pt, unsigned int pp, unsigned int pid_enl_on, 
		unsigned int pid_enl_off, bool pa, unsigned int pact,
		unsigned int pid);
	~Alternador_actor_generico();

	bool es_actor_recibido() const {return ref_actor != NULL;}

	void turno(float p_delta);
	void asignar_referencia(Actor_generico * a) {ref_actor=a;}
	void limpiar_referencia() {ref_actor=NULL;}
	void actualizar_estado_actor();

	bool es_recien_cambiado_estado() const 
	{
		return activo_anterior!=estado_paso_actual; //es_activo();
	}

	bool es_estado_on() {return estado_paso_actual==E_ENCENDIDO;}
	//bool es_estado_off() {return estado_paso_actual==E_APAGADO;}

	void activar() 
	{
		Activable::activar();
		actualizar_estado_actor();
	}

	void desactivar() 
	{
		Activable::desactivar();
		actualizar_estado_actor();
	}

	unsigned int acc_tipo_actor() const {return tipo_actor;}
	unsigned int acc_id_enlace_activacion() const {return id_enlace_activacion;}
	unsigned int acc_id_enlace_desactivacion() const {return id_enlace_desactivacion;}
	unsigned int acc_patron() const {return patron;}
	unsigned int acc_paso_actual() const {return paso_actual;}



	////////////////////
	// Representable

	public:

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "alternador actor generico";}
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
};

#endif
