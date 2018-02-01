#ifndef VISITANTE_INSERCION_SALA_H
#define VISITANTE_INSERCION_SALA_H

#include "visitante.h"
#include "actor_generico.h"
#include "interruptor_repetible.h"
#include "interruptor_no_repetible.h"
#include "interruptor_invisible_no_repetible.h"
#include "interruptor_invisible_repetible.h"
#include "cofre.h"
#include "proyectil_obstaculo.h"
#include "entrada_salida_sala.h"
#include "actor_control_direccion.h"
#include "disparador_proyectiles.h"
#include "alternador_actor_generico.h"
#include "actor_audio.h"
#include "actor_diapositivas.h"
#include "actor_info_juego.h"
#include "plataforma_flexible.h"
#include "escalera.h"
#include "teletransportador.h"
#include "interruptor_contacto.h"
#include "actor_puzzle.h"
#include "actor_punto_guardado.h"
#include "actor_recuperar_tiempo.h"
#include "interruptor_auto.h"

class Visitante_insercion_sala: public Visitante
{
	private:

	std::vector<Actor *>& con_turno;
	std::vector<Entrada_sala*>& entradas;
	std::vector<Interactuable *>& interactuables;
	std::vector<Actor_control_direccion *>& controles_direccion;
	std::vector<Actor_generico *>& recogibles;
	std::vector<Escalera *>& escaleras;
	std::vector<Representable_overlay *>& overlays;
	std::vector<Teletransportador *>& teletransportadores;

	bool sumar_gema;

	public:
	Visitante_insercion_sala(std::vector<Actor *>& t, 
			std::vector<Entrada_sala *>& e, 
			std::vector<Interactuable *>& i, 
			std::vector<Actor_control_direccion *>& acd,
			std::vector<Actor_generico *>& r,
			std::vector<Escalera *>& esc,
			std::vector<Representable_overlay *>& over,
			std::vector<Teletransportador *>& teles
);
	~Visitante_insercion_sala();

	bool es_sumar_gema() const {return sumar_gema;}
	
	void visitar(Bloque&) {}
	void visitar(Actor&) {}
	void visitar(Salida_sala&) {}
	void visitar(Plataforma_flexible&) {}

	void visitar(Actor_generico& a);
	void visitar(Interruptor_repetible& a);
	void visitar(Interruptor_no_repetible& a);
	void visitar(Interruptor_invisible_no_repetible& a);
	void visitar(Interruptor_invisible_repetible& a);
	void visitar(Cofre& a);
	void visitar(Proyectil_obstaculo& a);
	void visitar(Entrada_sala& a);
	void visitar(Actor_control_direccion& a);
	void visitar(Disparador_proyectiles& a);
	void visitar(Alternador_actor_generico& a);
	void visitar(Actor_audio& a);
	void visitar(Escalera& a);
	void visitar(Actor_diapositivas& a);
	void visitar(Actor_info_juego& a);
	void visitar(Interruptor_contacto& a);
	void visitar(Teletransportador& a);
	void visitar(Actor_puzzle& a);
	void visitar(Actor_punto_guardado& a);
	void visitar(Actor_recuperar_tiempo& a);
	void visitar(Interruptor_auto& a);
};	

#endif
