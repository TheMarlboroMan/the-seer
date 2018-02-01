#ifndef ACTOR_H
#define ACTOR_H

/*El actor ocupa el espacio "real" en la sala.

Define además la interfaz para varias cosas, como por ejemplo, cómo deben
ser almacenados en la sala.

Obliga a todos los que heredan a implementar el método para aceptar visitantes.
*/

#include "representable.h"
#include "espaciable.h"
#include "datos_colision.h"

class Visitante;

class Actor:public Representable, public Espaciable
{
	private:
	
	unsigned int id_localizador;
	bool marcado_para_borrar;
	Punto_2d coordenadas_origen_rep;
	
	protected:

	void actualizar_id_localizador(unsigned int v) {id_localizador=v;}

	public:

	Actor(int px, int py, int pz);
	virtual ~Actor();
	Actor(const Actor& otro);
	Actor& operator=(const Actor& otro);
	bool operator!=(const Actor& otro) const;

	/////////////////////
	// Representable...

	public:

	unsigned int obtener_x_representable() const {return acc_x();}
	unsigned int obtener_y_representable() const {return acc_y();}
	unsigned int obtener_z_representable() const {return acc_z();}
	unsigned int obtener_w_representable() const {return acc_w();}
	unsigned int obtener_d_representable() const {return acc_d();}
	unsigned int obtener_h_representable() const {return acc_h();}
	bool rep_final_es_dentro_de_caja(int dx, int dy, const Bloque_transformacion_representable& bt, const SDL_Rect& caja) const;

	virtual bool es_representable() const {return true;}
//	virtual void tell_me_about_yourself() const {std::cout<<string_me_about_yourself()<<std::endl;}
//	virtual std::string string_me_about_yourself() const=0;

	////////////////////
	// Espaciable...

	public:

	virtual bool es_colisionable() const {return true;}	//Por defecto son todos colisionables.
	virtual bool es_solido() const {return true;}	//Por defecto son todos sólidos.

	//////////////////
	// Interfaz propia a implementar.

	public:

	//Todos los actores deben aceptar el visitante.
	virtual void actualizar_volumen()=0;
	virtual void aceptar_visitante(Visitante& v) {}	

	///////////////////
	// Métodos propios.

	void establecer_id_localizador(unsigned int v) {id_localizador=v;}
	void marcar_para_borrar();
	unsigned int acc_id_localizador() const {return id_localizador;}
	bool es_marcado_para_borrar() const {return marcado_para_borrar;}

	Punto_2d obtener_coordenadas_origen_representacion();
	Punto_2d acc_coordenadas_origen_rep() const {return coordenadas_origen_rep;}

	void actualizar_coordenadas_representacion();
};

#endif
