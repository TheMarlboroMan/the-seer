#ifndef INTERRUPTOR_CONTACTO
#define INTERRUPTOR_CONTACTO

/*Al tocarlo se dispara una cadena y al dejar de tocarlo,
otra. Se define como repetible o no repetible en función
de si es posible*/

#include "actor.h"
#include "activable.h"
#include "visitante.h"

class Interruptor_contacto:public Actor, public Activable
{
	private:

	enum estados{
		E_OUT,
		E_RECIEN_OUT,
		E_RECIEN_IN,
		E_IN};

	unsigned int w;
	unsigned int h;
	unsigned int d;
	unsigned int id_cadena_in;
	unsigned int id_cadena_out;
	bool reusable;

	unsigned short int estado;
	mutable unsigned int cuenta_in;
	mutable unsigned int cuenta_out;

	bool colision_este_turno;

	////////////////////////////////////
	//Métodos propios...
	

	public:

	Interruptor_contacto(int px, int py, int pz, int pw, int ph, int pd, 
		unsigned int pidc_in, unsigned int pidc_out, bool pr, bool pa, 
		unsigned int pid, unsigned int, unsigned int);
	~Interruptor_contacto();

	void marcar_como_colision();
	void actualizar_estado(/*bool*/);

	unsigned int obtener_id_activo() const;
	
	unsigned int acc_cuenta_in() const {return cuenta_in;}
	unsigned int acc_cuenta_out() const {return cuenta_out;}
	unsigned int acc_id_cadena_in() const {return id_cadena_in;}
	unsigned int acc_id_cadena_out() const {return id_cadena_out;}
	bool es_repetible() const {return reusable;}

	////////////////////////////////////
	//Implementación de representable...
	
	public:

	virtual bool es_representable() const {return false;}
	virtual void transformar_representacion(Bloque_transformacion_representable&) {}

	////////////////////////////////////
	//Implementación de espaciable.

	virtual bool es_colisionable() const {return true;}
	virtual bool es_solido() const {return false;}
	virtual bool es_recibe_sombra() const {return false;}

	virtual std::string string_me_about_yourself() const {return "interruptor contacto";}

	////////////////////////////////////	
	//Implementación de actor.
	virtual void actualizar_volumen();
	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
