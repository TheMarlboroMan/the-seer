#ifndef INTERRUPTOR_H
#define INTERRUPTOR_H

/*La clase base para todos los interruptores: lleva el eslabón de la cadena y 
poco más. El resto tienen que implementar las particularidades.*/

#include "interactuable.h"

class Interruptor:public Interactuable
{
	///////////////////////////////
	// Propio.

	private:

	unsigned int id_eslabon_cadena;
	unsigned int tipo_condicion;	//El interruptor puede estar sujeto a una condición estática. Las condiciones estarían en el visitante de interacción.
	unsigned int id_eslabon_cadena_no_condicion;

	bool permitido;

	/*Cuando por una condición errónea no es posible activar el interruptor,
	todas las clases derivadas pueden usar este código. Lo que hace es 
	sencillo: si hay una cadena errónea devuelve esa. Si no, 
	devuelve la interacción nula.*/

	Datos_interaccion obtener_datos_interaccion_condicion_erronea()
	{
		if(id_eslabon_cadena_no_condicion)
		{
			return obtener_datos_interaccion(id_eslabon_cadena_no_condicion);
		}
		else
		{
			return obtener_interaccion_nula();		
		}
	}

	protected:

	/*Este es el que deben sobreescribir las clases que hereden de esta.*/

	virtual Datos_interaccion obtener_interaccion()=0;


	public:

	Interruptor(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int pid, unsigned int ptc, unsigned int pid_no)
		:Interactuable(px, py, pz), id_eslabon_cadena(pid),
		tipo_condicion(ptc), id_eslabon_cadena_no_condicion(pid_no),
		permitido(false)
	{}

	virtual ~Interruptor()
	{}

	unsigned int acc_id_eslabon_cadena() const {return id_eslabon_cadena;}
	unsigned int acc_tipo_condicion() const {return tipo_condicion;}
	unsigned int acc_id_eslabon_cadena_no_condicion() const {return id_eslabon_cadena_no_condicion;}

	//////////////////////
	// Interactuable...

	protected:

	//Este está aquí para que las clases que hereden la llamen para obtener
	//un resultado a la hora de interactuar.
	Datos_interaccion obtener_datos_interaccion(/*unsigned int valor,*/ int pid_eslabon_cadena=-1)
	{
		unsigned int ped=pid_eslabon_cadena==-1 ? id_eslabon_cadena : pid_eslabon_cadena; 
		return Datos_interaccion(ped/*, valor*/);
	}

	public:

	Datos_interaccion interactuar() /*Final final final final*/
	{
		if(permitido) return obtener_interaccion();
		else return obtener_datos_interaccion_condicion_erronea();
	}

	void permitir(bool p) {permitido=p;}
};

#endif
