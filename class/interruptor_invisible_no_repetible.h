#ifndef INTERRUPTOR_INVISIBLE_NO_REPETIBLE
#define INTERRUPTOR_INVISIBLE_NO_REPETIBLE

/*Como un interruptor no repetible pero además no es colisionable ni visible:
sirve, por ejemplo, para hacer puertas secretas y, al contrario que los
otros, quiere que le digas sus medidas.*/

#include "interruptor.h"
#include "visitante.h"

class Interruptor_invisible_no_repetible:public Interruptor
{
	/////////////////////////////
	// Propio.

	private:

	bool usado;
	bool secreto;
	unsigned int vw;
	unsigned int vh;
	unsigned int vd;

	public:

	Interruptor_invisible_no_repetible(unsigned int px, unsigned int py, unsigned int pz,
		unsigned int pw, unsigned int ph, unsigned int pd,  
		unsigned int pid, bool sec, unsigned int, unsigned int, bool);
	~Interruptor_invisible_no_repetible();

	bool es_usado() const {return usado;}
	bool es_secreto() const {return secreto;}

	/////////////////////////////
	// Espaciable

	void actualizar_volumen(){establecer_volumen(vw, vd, vh);}
	virtual bool es_solido() const {return false;}
	virtual bool es_recibe_sombra() const {return false;}

	/////////////////////////////
	// Representable

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "interruptor invisible no repetible";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br) {}

	/////////////////////////////
	// Interruptor.

	virtual Datos_interaccion obtener_interaccion();

	/////////////////////////////
	// Actor.

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
