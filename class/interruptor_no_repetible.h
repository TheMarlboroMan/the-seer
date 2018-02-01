#ifndef INTERRUPTOR_NO_REPETIBLE
#define INTERRUPTOR_NO_REPETIBLE

#include "interruptor.h"
#include "visitante.h"

class Interruptor_no_repetible:public Interruptor
{
	/////////////////////////////////
	// Propio...

	private:

	bool usado;

	public:

	Interruptor_no_repetible(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int pid_eslabon, unsigned int pid,
		unsigned int ptc, unsigned int pidn, bool);
	~Interruptor_no_repetible();

	bool es_usado() const {return usado;}

	//////////////////////////////////
	// Representable.

	virtual bool es_representable() const {return true;}
	virtual std::string string_me_about_yourself() const {return "interruptor no repetible";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br);

	//////////////////////////////////
	// Espaciable

	void actualizar_volumen(){establecer_volumen(8, 8, 8);}
	virtual bool es_recibe_sombra() const {return true;}

	//////////////////////////////////
	// Interactuable.

	virtual void recibir_senal_interactuable_reactivar() {usado=false;}

	//////////////////////////////////
	//Interruptor
	virtual Datos_interaccion obtener_interaccion();

	////////////////////////////////////
	// Actor.

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
