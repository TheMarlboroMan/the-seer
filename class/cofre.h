#ifndef COFRE
#define COFRE

#include "interactuable.h"
#include "visitante.h"
#include "control_pieza_puzzle.h"

/*El cofre es interactuable y especial: cada uno de ellos
tiene un número de pieza y una posición para el puzzle final.

El número de pieza y la posición se asignan al azar cuando el 
cofre se abre.

Como consecuencia de esto, se espera que haya exactamente 15 cofres
en todo el juego. Se espera también que los 15 cofres se guarden en 
el almacenamiento persistente.
*/

class Cofre:public Interactuable, public Representable_overlay
{
	//////////////////////////////////
	// Propio.

	private:

	static bool habilitado_overlays;
	bool abierto;
	unsigned int id_pieza;

	public:

	Cofre(unsigned int px, unsigned int py, unsigned int pz, unsigned int idp, bool);
	~Cofre();

	void abrir() {abierto=true;}
	static void habilitar_overlays() {habilitado_overlays=true;}
	unsigned int acc_id_pieza() const {return id_pieza;}

	bool es_abierto() const {return abierto;} 
	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}

	//////////////////////////////////
	// Representable

	virtual bool es_representable() const {return true;}
	virtual std::string string_me_about_yourself() const {return "cofre";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br);

	//////////////////////////////////
	// Espaciable.

	public:

	void actualizar_volumen(){establecer_volumen(8, 8, 8);}
	virtual bool es_recibe_sombra() const {return true;}

	//////////////////////////////////
	// Interactuable

	public:

	virtual Datos_interaccion interactuar() {return obtener_interaccion_nula();}

	//////////////////////////////////
	// Overlay

	public:

	virtual bool es_genera_overlay() const {return es_abierto() && habilitado_overlays;}
	virtual void transformar_overlay(Bloque_transformacion_representable&);

};

#endif
