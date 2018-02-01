#ifndef VISITANTE_SALIDA_SALA_H
#define VISITANTE_SALIDA_SALA_H

/*Este visitante se usa al iniciar_salida en una sala. Sirve para
limpiar cualquier basurilla que pueda haberse generado con los
actores.*/

class Visitante_salida_sala:public Visitante
{
	public:

	Visitante_salida_sala() {}
	~Visitante_salida_sala(){}

	//////////////////////
	//Visitantes en si...

	void visitar(Bloque&) {}
	void visitar(Actor&) {}
	void visitar(Proyectil_obstaculo&) {}
	void visitar(Entrada_sala&) {}
	void visitar(Salida_sala&) {}
	void visitar(Actor_control_direccion&) {}
	void visitar(Disparador_proyectiles&) {}
	void visitar(Actor_generico&) {}
	void visitar(Interruptor_repetible&) {}
	void visitar(Interruptor_no_repetible&){}
	void visitar(Interruptor_invisible_no_repetible&){}
	void visitar(Interruptor_invisible_repetible&){}
	void visitar(Cofre&) {}
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
	void visitar(Alternador_actor_generico& a) {a.limpiar_referencia();}
	void visitar(Actor_audio& a) {a.detener_sonido();}
};

#endif
