#ifndef VISITANTE_H
#define VISITANTE_H

/*El visitante define la base para un sistema de despachamiento doble. Nos
servirá para evitar algún que otro static_cast.*/

class Bloque;
class Actor;
class Interruptor_repetible;
class Interruptor_no_repetible;
class Interruptor_invisible_no_repetible;
class Interruptor_invisible_repetible;
class Cofre;
class Entrada_sala;
class Salida_sala;
class Actor_control_direccion;
class Proyectil_obstaculo;
class Disparador_proyectiles;
class Actor_generico;
class Alternador_actor_generico;
class Actor_audio;
class Plataforma_flexible;
class Escalera;
class Actor_diapositivas;
class Interruptor_contacto;
class Teletransportador;
class Actor_info_juego;
class Actor_puzzle;
class Actor_punto_guardado;
class Actor_recuperar_tiempo;
class Interruptor_auto;

class Visitante
{
	public:

	Visitante() {}
	virtual ~Visitante() {}

	virtual void visitar(Bloque&)=0;
	virtual void visitar(Actor&)=0;
	virtual void visitar(Interruptor_repetible&)=0;
	virtual void visitar(Interruptor_no_repetible&)=0;
	virtual void visitar(Interruptor_invisible_no_repetible&)=0;
	virtual void visitar(Interruptor_invisible_repetible&)=0;
	virtual void visitar(Cofre&)=0;																
	virtual void visitar(Entrada_sala&)=0;
	virtual void visitar(Salida_sala&)=0;
	virtual void visitar(Actor_control_direccion&)=0;
	virtual void visitar(Proyectil_obstaculo&)=0;
	virtual void visitar(Disparador_proyectiles&)=0;
	virtual void visitar(Actor_generico&)=0;
	virtual void visitar(Alternador_actor_generico&)=0;
	virtual void visitar(Actor_audio&)=0;
	virtual void visitar(Plataforma_flexible&)=0;
	virtual void visitar(Escalera&)=0;
	virtual void visitar(Actor_diapositivas&)=0;
	virtual void visitar(Interruptor_contacto&)=0;
	virtual void visitar(Teletransportador&)=0;
	virtual void visitar(Actor_info_juego&)=0;
	virtual void visitar(Actor_puzzle&)=0;
	virtual void visitar(Actor_punto_guardado&)=0;
	virtual void visitar(Actor_recuperar_tiempo&)=0;
	virtual void visitar(Interruptor_auto&)=0;
};
/*
class Visitante_const
{
	public:

	Visitante_const() {}
	virtual ~Visitante_const() {}

	virtual void visitar(const Bloque&)=0;
	virtual void visitar(const Actor&)=0;
	virtual void visitar(const Interruptor_repetible&)=0;
	virtual void visitar(const Interruptor_no_repetible&)=0;
	virtual void visitar(const Interruptor_invisible_no_repetible&)=0;
	virtual void visitar(const Interruptor_invisible_repetible&)=0;
	virtual void visitar(const Cofre&)=0;																
	virtual void visitar(const Entrada_sala&)=0;
	virtual void visitar(const Salida_sala&)=0;
	virtual void visitar(const Actor_control_direccion&)=0;
	virtual void visitar(const Proyectil_obstaculo&)=0;
	virtual void visitar(const Disparador_proyectiles&)=0;
	virtual void visitar(const Actor_generico&)=0;
	virtual void visitar(const Alternador_actor_generico&)=0;
	virtual void visitar(const Actor_audio&)=0;
	virtual void visitar(const Plataforma_flexible&)=0;
	virtual void visitar(const Escalera&)=0;
	virtual void visitar(const Actor_diapositivas&)=0;
	virtual void visitar(const Interruptor_contacto&)=0;
	virtual void visitar(const Teletransportador&)=0;
	virtual void visitar(const Actor_info_juego&)=0;
	virtual void visitar(const Actor_puzzle&)=0;
	virtual void visitar(const Actor_punto_guardado&)=0;
	virtual void visitar(const Actor_recuperar_tiempo&)=0;
	virtual void visitar(const Interruptor_auto&)=0;
};
*/
#endif
