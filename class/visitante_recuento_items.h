#ifndef VISITANTE_RECUENTO_ITEMS_H
#define VISITANTE_RECUENTO_ITEMS_H

class Visitante_recuento_items:public Visitante
{
	public:

	//////////////////////////////////////////////////
	//Este es el estado que tenemos que interpretar...

	unsigned int cofres;
	unsigned int tesoro;
	unsigned int llaves;

	/////////////////////////////////////////////////
	//Métodos útiles.

	Visitante_recuento_items()
		:Visitante(), cofres(0), tesoro(0), llaves(0) {}

	~Visitante_recuento_items(){}

	//////////////////////
	//Visitantes en si...

	void visitar(Bloque& a) {}
	void visitar(Actor& a) {}
	void visitar(Proyectil_obstaculo& a) {}
	void visitar(Entrada_sala& a) {}
	void visitar(Salida_sala& a) {}
	void visitar(Actor_control_direccion&) {}
	void visitar(Disparador_proyectiles&) {}
	void visitar(Alternador_actor_generico&) {}
	void visitar(Interruptor_repetible&) {}
	void visitar(Interruptor_no_repetible&){}
	void visitar(Interruptor_invisible_no_repetible&){}
	void visitar(Interruptor_invisible_repetible&){}
	void visitar(Actor_audio&) {}
	void visitar(Plataforma_flexible&) {}
	void visitar(Escalera&) {}
	void visitar(Actor_diapositivas&) {}
	void visitar(Interruptor_contacto&) {}
	void visitar(Teletransportador&) {}
	void visitar(Actor_punto_guardado&) {}
	void visitar(Actor_recuperar_tiempo&) {}
	void visitar(Actor_info_juego&) {}
	void visitar(Actor_puzzle&) {}
	void visitar(Interruptor_auto&) {}
	void visitar(Cofre& a) {++cofres;}
	void visitar(Actor_generico& a) 
	{
		if(a.es_bonus()) ++tesoro;
		else if(a.es_llave()) ++llaves;
	}
};

#endif
