#ifndef INTERRUPTOR_REPETIBLE
#define INTERRUPTOR_REPETIBLE

#include "interruptor.h"
#include "visitante.h"

class Interruptor_repetible:public Interruptor
{
	////////////////////////////
	// Propio

	private:

	unsigned int id_eslabon_cadena_off; //El del interruptor es para encender. Este es para apagar...
	bool encendido;

	public:

	Interruptor_repetible(unsigned int px, unsigned int py, unsigned int pz, unsigned int pid, unsigned int pid_off, unsigned int, unsigned int, bool);
	~Interruptor_repetible();

	bool es_encendido() const {return encendido;}
	unsigned int acc_id_eslabon_cadena_off() const {return id_eslabon_cadena_off;}	

	////////////////////////////
	// Espaciable

	void actualizar_volumen(){establecer_volumen(8, 8, 8);}
	virtual bool es_recibe_sombra() const {return true;}

	////////////////////////////
	// Representbale.

	virtual bool es_representable() const {return true;}
	virtual std::string string_me_about_yourself() const {return "interruptor repetible";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br);

	////////////////////////////
	// Interruptor

	virtual Datos_interaccion obtener_interaccion();

	////////////////////////////
	// Actor.

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
