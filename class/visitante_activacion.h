#ifndef VISITANTE_ACTIVACION_H
#define VISITANTE_ACTIVACION_H

#include "visitante.h"

#include "actor_diapositivas.h"
#include "disparador_proyectiles.h"
#include "alternador_actor_generico.h"
#include "actor_audio.h"
#include "plataforma_flexible.h"
#include "escalera.h"
#include "interruptor_contacto.h"
#include "interruptor_auto.h"


/*Este visitante se usa para encender y apagar cosas. Su estado indica
si debe encender o apagar.*/

class Visitante_activacion:public Visitante
{
	/////////////////////
	// Enums...
	
	public:

	enum estados {E_DESACTIVAR, E_ACTIVAR};

	////////////////////
	//Estado y métodos.

	private:

	unsigned int estado;

	void procesar(Activable &a)
	{
		switch(estado)
		{
			case E_DESACTIVAR: a.desactivar(); break;
			case E_ACTIVAR: a.activar(); break;
		}
	}

	public:

	/////////////////////////////////////////////////
	//Métodos útiles.

	Visitante_activacion(unsigned int e)
		:Visitante(), estado(e)
	{}

	~Visitante_activacion()
	{}

	//////////////////////
	//Visitantes en si...

	void visitar(Bloque&) {}
	void visitar(Actor&) {}
	void visitar(Proyectil_obstaculo&) {}
	void visitar(Entrada_sala&) {}
	void visitar(Salida_sala&) {}
	void visitar(Actor_control_direccion&) {}
	void visitar(Actor_generico&) {}
	void visitar(Interruptor_repetible&) {}
	void visitar(Interruptor_no_repetible&){}
	void visitar(Interruptor_invisible_no_repetible&){}
	void visitar(Interruptor_invisible_repetible&){}
	void visitar(Cofre&) {}
	void visitar(Teletransportador&) {}
	void visitar(Actor_punto_guardado&) {}
	void visitar(Actor_recuperar_tiempo&) {}

	void visitar(Actor_puzzle& a) {procesar(a);}
	void visitar(Actor_info_juego& a) {procesar(a);}
	void visitar(Actor_diapositivas& a) {procesar(a);}
	void visitar(Disparador_proyectiles& a) {procesar(a);}
	void visitar(Alternador_actor_generico& a) {procesar(a);}
	void visitar(Actor_audio& a) {procesar(a);}
	void visitar(Plataforma_flexible&) {}
	void visitar(Escalera& a) {procesar(a);}
	void visitar(Interruptor_contacto& a) {procesar(a);}
	void visitar(Interruptor_auto& a) {procesar(a);}
};

#endif
