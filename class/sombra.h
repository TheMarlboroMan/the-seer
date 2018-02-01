#ifndef SOMBRA
#define SOMBRA

#include "actor.h"
#include "visitante.h"
#include "herramientas_graficas.h"
#include "temporizador_animaciones.h"

class Sombra: public Actor
{	
	/////////////////////
	// Propio.

	private:

	static const unsigned int X_SOMBRA_NORMAL=0;
	static const unsigned int Y_SOMBRA_NORMAL=0;

	static const unsigned int X_PATRON=0;
	static const unsigned int Y_PATRON=181;
	static const unsigned int W_RECORTE=15;
	static const unsigned int H_RECORTE=11;
	static const unsigned int TOTAL_PASOS_ANIMACION=4;
	
	static Uint32 COLOR_ALTERAR;
	static Uint32 COLOR_TRANSPARENTE;

	static Uint32 COLOR_LAVA_01;
	static Uint32 COLOR_LAVA_02;
	static Uint32 COLOR_LAVA_03;
	static Uint32 COLOR_LAVA_04;

	static Uint32 COLOR_AGUA_01;
	static Uint32 COLOR_AGUA_02;
	static Uint32 COLOR_AGUA_03;
	static Uint32 COLOR_AGUA_04;

	DLibV::Lienzo * recurso_alterado;
	bool visible;
	unsigned int estado;

	void inicializar_colores();
	void generar_recurso();
	void liberar_recurso();

	unsigned int calcular_x_representacion(unsigned int);

	public:

	enum estados{
		E_NORMAL,
		E_LAVA,
		E_AGUA
		};

	Sombra();
	~Sombra();

	void establecer_visibilidad(bool v) {visible=v;}
	void establecer_estado(unsigned int e) {estado=e;}

	/////////////////////
	// Espaciable

	public:

	void actualizar_volumen(){establecer_volumen(8, 8, 2);}
	virtual bool es_solido() const {return false;}
	virtual bool es_colisionable() const {return false;}
	virtual bool es_recibe_sombra() const {return false;}

	////////////////////
	// Representable

	public:

	virtual bool es_representable() const {return true;}
	virtual std::string string_me_about_yourself() const {return "sombra";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br);

	////////////////////
	// Actor.
	
	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
