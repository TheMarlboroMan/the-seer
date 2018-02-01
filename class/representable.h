#ifndef REPRESENTABLE_H
#define REPRESENTABLE_H

#include "../libdan_headers/libDanSDL.h"
#include "geometria.h"


/*Representable es la interface que deben implementar aquellas cosas que quieran
estar en el espacio isométrico.

Viene acompañado del bloque_transformacion_representable, que encapsularía info
extra que pudiéramos usar, aunque ahora mismo no haga nada.
*/

extern DLibH::Log_base LOG;

class Representador;

struct Bloque_transformacion_representable
{
	private:

	DLibV::Representacion_bitmap_dinamica& rep_bmp;
	DLibV::Representacion_agrupada_dinamica& rep_agr;

	DLibV::Representacion * rep;
	
//	float delta;

	public:

	Bloque_transformacion_representable(
		DLibV::Representacion_bitmap_dinamica& pr,
		DLibV::Representacion_agrupada_dinamica& ag) //, float pd)
		:rep_bmp(pr), rep_agr(ag), rep(NULL)
	{
		rep_agr.poseer_representaciones();
	}

	~Bloque_transformacion_representable()
	{
		rep_agr.vaciar_grupo();
	}

	void establecer_recorte(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {rep->establecer_recorte(x, y, w, h);}	 
	void establecer_posicion(unsigned int x, unsigned int y, unsigned int w, unsigned int h) {rep->establecer_posicion(x, y, w, h);}
	SDL_Rect acc_posicion() const {return rep->acc_posicion();}
	void usar_bitmap() {rep=&rep_bmp;}
	void usar_grupo() 
	{
		rep_agr.vaciar_grupo();
		rep=&rep_agr;
	}

	///////////////////
	// Para grupo

	void insertar_en_grupo(DLibV::Representacion * r) {rep_agr.insertar_representacion(r);}

	///////////////////
	// Para bitmap.

	void establecer_recurso(unsigned int i)
	{
		if(!DLibV::Gestor_recursos_graficos::comprobar(i))
		{
			LOG<<"SOLICITADO RECURSO "<<i<<", NO EXISTENTE"<<std::endl;
		}
		else
		{
			rep_bmp.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(i));
		}
	}

	void establecer_recurso_sin_recortar(unsigned int i)
	{
		if(!DLibV::Gestor_recursos_graficos::comprobar(i))
		{
			LOG<<"SOLICITADO RECURSO "<<i<<", NO EXISTENTE"<<std::endl;
		}
		else
		{
			rep_bmp.establecer_recurso_sin_recortar(DLibV::Gestor_recursos_graficos::obtener(i));
		}
	}
	
	void establecer_recurso_manual(DLibV::Recurso_grafico * r)
	{
		rep_bmp.establecer_recurso(r);
	}

	friend class Representador;
};

class Ordenador_representables; //F-f-f-forward declaration...

class Representable 
{
	private:

	bool debug_ordenacion;
	int profundidad_ordenacion;
	std::vector<Representable *> otros_por_detras;
	bool visitado_ordenacion;

	unsigned int rx, ry, rz, rfx, rfy, rfz;

	public:

	enum planos {PL_FONDO=0, PL_NORMAL=1, PL_FRENTE=2};

	Representable();
	Representable(const Representable& otro);
	Representable& operator=(const Representable& otro);

	static Punto_2d obtener_coordenadas_representacion(unsigned int x, unsigned int y, unsigned int z);
	void preparar_representable();

	void establecer_debug_ordenacion(bool v) {debug_ordenacion=v;}
	bool es_debug_ordenacion() const {return debug_ordenacion;}
	unsigned int acc_profundidad_ordenacion() const {return profundidad_ordenacion;}
	
	///////////////////////////
	//Interfaz a implementar...

	/*La existencia de varios planos de representación puede reducir la 
	cantidad de iteraciones necesarias para ordenar las rejillas.*/

	virtual unsigned int obtener_plano_representable() const {return PL_NORMAL;}
	virtual unsigned int obtener_x_representable() const=0;
	virtual unsigned int obtener_y_representable() const=0;
	virtual unsigned int obtener_z_representable() const=0;
	virtual unsigned int obtener_w_representable() const=0;
	virtual unsigned int obtener_d_representable() const=0;
	virtual unsigned int obtener_h_representable() const=0;
	virtual bool es_representable() const=0;
	virtual void tell_me_about_yourself() const {std::cout<<string_me_about_yourself()<<std::endl;}
	virtual std::string string_me_about_yourself() const=0;
	virtual void transformar_representacion(Bloque_transformacion_representable&)=0;
	virtual bool rep_final_es_dentro_de_caja(int, int, const Bloque_transformacion_representable&, const SDL_Rect&) const=0;

	friend class Ordenador_representables;
};

class Ordenador_representables
{
	private:

	unsigned int indice_profundidad;
	void visitar_nodo(Representable * item);

	public:

	Ordenador_representables();
	bool operator()(const Representable* a, const Representable* b) const;
	void procesar(std::vector<Representable *>& v);
};

/*Aplicable a aquellos actores que pueden generar un overlay, como el 
cofre una vez abierto, que muestra la pieza que había dentro y tal.*/

class Representable_overlay
{
	public:

	virtual bool es_genera_overlay() const=0;
	virtual void transformar_overlay(Bloque_transformacion_representable&)=0;
};

#endif
