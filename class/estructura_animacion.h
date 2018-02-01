#ifndef ESTRUCTURA_ANIMACION_H
#define ESTRUCTURA_ANIMACION_H

/*Esto es para controlar animaciones de representaciones en movimiento, 
básicamente cosas del menú principal que vamos a mover de un sitio a otro,
a hacer transparente o cosas así...*/

#include <vector>
#include "../libdan_headers/video/pantalla/pantalla.h"
#include "../libdan_headers/video/representacion/representacion.h"
#include "../libdan_headers/herramientas/herramientas/herramientas.h"
#include "../libdan_headers/herramientas/vector_2d/vector_2d.h"

//Clase base para todas ellas.

class Estructura_animacion
{
	private:

	virtual void generar_porcentajes()=0;	//Su finalidad es calcular los datos necesarios para los porcentajes.
	virtual void calcular_porcentajes()=0;  //Esto es para calcular los porcentajes después de cada turno.

	protected:

	DLibV::Representacion& rep;
	float porcentaje;

	public:

	virtual void turno(float)=0; 
	virtual void forzar_finalizacion()=0;

	Estructura_animacion(DLibV::Representacion&);
	virtual ~Estructura_animacion(){}
	bool es_completa() {return this->porcentaje>=100;}

	friend class Grupo_animacion;
};

//Esta controla movimientos.

class Estructura_animacion_movimiento:public Estructura_animacion
{
	public:

	float pos_x;
	float pos_y;
	float dest_x;
	float dest_y;
	DLibH::Vector_2d v;

	float mult_porcentaje_x;
	float mult_porcentaje_y;
	float dist_x;
	float dist_y;
	float dist_tot;

	private:

	void generar_porcentajes();
	void calcular_porcentajes();
	void comprobacion_y_movimiento(float delta, float vector, float &pos, float dest);

	public:

	Estructura_animacion_movimiento(DLibV::Representacion&, const DLibH::Vector_2d&, float, float);

	void turno(float p_delta);
	void forzar_finalizacion();
};

//Esto aglutina varias animaciones para que no haya que poner tánto código.
//Posee y destruye las animaciones que se le pasan.

class Grupo_animacion
{
	private:

	std::vector<Estructura_animacion*> animaciones; 

	float porcentaje;
	float porcentaje_parcial;
	float porcentaje_minimo_cancelar;
	unsigned int total_animaciones;

	public:

	float acc_porcentaje() const {return porcentaje;}

	Grupo_animacion(float porc=0);
	~Grupo_animacion();
	void asignar_animacion(Estructura_animacion*);
	void turno(float p_delta);

	bool es_finalizado() const {return porcentaje == 100;}
	bool tiene_animaciones() const {return animaciones.size();}

	bool finalizar();
	void forzar_finalizacion();
	void mostrar(DLibV::Pantalla&);
	void vaciar();
};

#endif
