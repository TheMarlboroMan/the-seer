#ifndef ESPACIABLE_H
#define ESPACIABLE_H

/*Algo espaciable es algo con coordenadas y volumen: un cubo. Lo vamos a usar
para aquellas cosas que estén en el mapa y como interfaz para interactuar
con ellas.*/

#include "geometria.h"

class Datos_colision;

class Espaciable
{
	private:
	Cubo posicion;
	bool cambio_posicion;

	protected:
	bool es_cambio_posicion() const {return cambio_posicion;}
	void reiniciar_cambio_posicion() {cambio_posicion=false;}

	public:
	Espaciable(int px, int py, int pz, int pw=0, int pd=0, int ph=0);
	Espaciable(const Espaciable& otro);
	Espaciable& operator=(const Espaciable& otro);

/*	unsigned int acc_x() const {return posicion.origen.x;}
	unsigned int acc_y() const {return posicion.origen.y;}
	unsigned int acc_z() const {return posicion.origen.z;}
	unsigned int acc_w() const {return posicion.w;}
	unsigned int acc_d() const {return posicion.d;}
	unsigned int acc_h() const {return posicion.h;}*/

	float acc_x() const {return posicion.origen.x;}
	float acc_y() const {return posicion.origen.y;}
	float acc_z() const {return posicion.origen.z;}
	float acc_w() const {return posicion.w;}
	float acc_d() const {return posicion.d;}
	float acc_h() const {return posicion.h;}

	const Cubo& como_cubo() const {return posicion;}

	void sumar_posicion(float px, float py, float pz);
	void desplazar_posicion(float pv=0.0, unsigned int pflags=0);
	void establecer_posicion(const Punto&);
	void establecer_posicion(float px=-1.0, float py=-1.0, float pz=-1.0);
	void establecer_posicion_por_eje(float pv, unsigned int pflags);
	void establecer_volumen(int pw=-1, int pd=-1, int ph=-1);
	void normalizar_posicion(unsigned int eje);
	void establecer_posicion(const Espaciable& otro){posicion.origen=otro.posicion.origen;}

	//OK: Este método NO existe, de acuerdo?. Si quieres ver si un espaciable
	//colisiona con otro hay que hacerlo mediante cubos y comprobando si 
	//uno mismo es o no colisionable. Hay tantas cosas que pueden ser confusas
	//que es mejor dar control al código cliente.
	
//	virtual bool en_colision_con(const Espaciable& p_otro) const
//	{	
//		return p_otro.es_colisionable() && Cubo::en_colision_con(posicion, p_otro.posicion);
//	}

	virtual bool en_colision_con(const Cubo& cubo) const {return Cubo::en_colision_con(posicion, cubo);}
	virtual bool dentro_de(const Cubo& cubo, bool roce_es_colision=false) const {return Cubo::dentro_de(posicion, cubo, roce_es_colision);}

	//Cuando se detecta una colisión con un espaciable se llama a este método
	//con para que rellene información sobre la colisión. En esta información
	//pueden ir efectos especiales, como por ejemplo, cambiar de sala.

	////////////////////////////////
	// Interfaz a implementar...

	//Ok: esto desaparece como parte del espaciable ya que el visitante de 
	//colisiones hace este trabajo.

	//virtual void rellenar_info_colision(Datos_colision& datos_colision) const=0;

	//Indica si es sólido, esto es, si es posible detectar colisiones con él.
	//Si esto devuelve false simplemente NUNCA pasaremos la comprobación de
	//en_colision_con.

	virtual bool es_colisionable() const=0;

	//Cuando se detecta una colisión con un elemento sólido la contaríamos
	//para que se realizasen correcciones en la trayectoria... Sería posible
	//una colisión con un actor que tuviese un efecto pero realmente no hiciese
	//que el jugador se detenga: por ejemplo, triggers al tacto.

	virtual bool es_solido() const=0;

	//Indica si debe recibir la sombra del jugador o no...	

	virtual bool es_recibe_sombra() const=0;
};

#endif
