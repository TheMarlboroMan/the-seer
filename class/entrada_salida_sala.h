#ifndef ENTRADA_SALIDA_SALA_H
#define ENTRADA_SALIDA_SALA_H

/*La salida de una sala es un obstáculo que, al chocar con él, nos manda a
otra sala, en la entrada que esté designada con el id_entrada (una sala puede
tener varias entradas y salidas.

Tiene un volumen pequeño. La colocación de las entradas y salidas tiene que ser
estratégica de forma que al chocar con una salida y llegar a una entrada no 
volvamos a chocar con la salida de turno.

La salida puede tener unas medidas variables. La entrada en un principio tiene
medidas fijas. Ya veremos si ampliamos.
*/

#include "actor.h"
#include "visitante.h"

class Entrada_salida:public Actor
{
	////////////////////////
	// Propio

	protected:
	unsigned int id_entrada;

	public:

	Entrada_salida(int px, int py, int pz, unsigned int p_entrada);
	Entrada_salida(const Entrada_salida& otro);
	Entrada_salida& operator=(const Entrada_salida& otro);
	virtual ~Entrada_salida();

	unsigned acc_id_entrada() const {return id_entrada;}
	virtual bool es_valida() const=0;

	/////////////////////
	// Representable

	public:

	virtual bool es_representable() const {return false;}
	virtual void transformar_representacion(Bloque_transformacion_representable& br) {}

	/////////////////////
	// Espaciable

	public:

	void actualizar_volumen(){establecer_volumen(4, 4, 2);}	
	virtual bool es_recibe_sombra() const {return false;}
};

class Salida_sala:public Entrada_salida
{
	///////////////////////////
	// Propio.

	private:
	unsigned int id_sala;

	unsigned int vw;
	unsigned int vh;
	unsigned int vd;

	public:

	Salida_sala(int px, int py, int pz, unsigned int pw, unsigned int ph, 
		unsigned int pd, unsigned int p_entrada, unsigned int p_sala);
	~Salida_sala();

	unsigned acc_id_sala() const {return id_sala;}
	virtual bool es_valida() const {return id_sala && id_entrada;}	

	/////////////////////////
	// Representable

	public:

	virtual std::string string_me_about_yourself() const {return "salida "+
		DLibH::Herramientas::entero_a_cadena(id_sala)+
		" -> "+
		DLibH::Herramientas::entero_a_cadena(id_entrada);}

	/////////////////////////
	// Espaciable

	public:

	virtual void actualizar_volumen(){establecer_volumen(vw, vh, vd);}	
	virtual bool es_colisionable() const {return true;}
	virtual bool es_solido() const {return false;}

	///////////////////////
	// Actor

	public:

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

class Entrada_sala:public Entrada_salida
{

	/////////////////////////////
	// Propio

	public:

	Entrada_sala(int px, int py, int pz, unsigned int p_entrada);
	Entrada_sala(const Entrada_sala& otro);
	Entrada_sala& operator=(const Entrada_sala& otro);
	~Entrada_sala();

	virtual bool es_valida() const {return id_entrada;}

	////////////////////////////
	// Espaciable

	virtual void actualizar_volumen(){establecer_volumen(1, 1, 1);}	
	virtual bool es_colisionable() const {return false;}
	virtual bool es_solido() const {return false;}

	////////////////////////////
	// Representable

	public:

	virtual std::string string_me_about_yourself() const {return "entrada "+
		DLibH::Herramientas::entero_a_cadena(id_entrada);}

	////////////////////////////
	// Actor.

	public:

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
