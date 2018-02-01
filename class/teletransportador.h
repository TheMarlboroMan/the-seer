#ifndef TELETRANSPORTADOR_H
#define TELETRANSPORTADOR_H

/*El teletransportador es un actor que puede activarse y coloca 
al jugador en otro teletransportador con un índice relacionado.
Al medir lo mismo que el jugador, su x, y y z valen para el destino.

El destino, por supuesto, está en la misma sala. De lo contrario sería
un par de entradas y salidas.

Ojo: El teletransportador no es representable. El diseño de los
niveles el que debe darles sentido.
*/

#include "actor.h"
#include "visitante.h"

class Teletransportador:public Actor
{
	////////////////////////
	// Propio

	private:

	static const int W=16;
	static const int D=16;
	static const int H=1;

	protected:
	unsigned int id_teletransportador;
	unsigned int id_conexion;

	public:

	Teletransportador(int px, int py, int pz, unsigned int p_id, unsigned int p_idc);

	unsigned acc_id_teletransportador() const {return id_teletransportador;}
	unsigned acc_id_conexion() const {return id_conexion;}
	Punto obtener_destino() const;

	/////////////////////
	// Representable

	public:

	virtual std::string string_me_about_yourself() const {return "teletransportador "+
		DLibH::Herramientas::entero_a_cadena(id_teletransportador)+
		" -> "+
		DLibH::Herramientas::entero_a_cadena(id_conexion);}
	virtual bool es_representable() const {return false;}
	virtual void transformar_representacion(Bloque_transformacion_representable& br) {}

	/////////////////////
	// Espaciable

	public:

	void actualizar_volumen(){establecer_volumen(W, D, H);}
	virtual bool es_recibe_sombra() const {return false;}

	virtual bool es_colisionable() const {return false;}
	virtual bool es_solido() const {return false;}

	///////////////////////
	// Actor

	public:

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
