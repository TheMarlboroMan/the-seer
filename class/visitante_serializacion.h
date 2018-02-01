#ifndef VISITANTE_SERIALIZACION_H
#define VISITANTE_SERIALIZACION_H

#include <fstream>
#include <string>
#include "visitante.h"
#include "factoria_actores.h"

class Visitante_serializacion:public Visitante
{
	private:

	std::ofstream& archivo;
	std::string separador;

	public:

	Visitante_serializacion(
		std::ofstream& a, 
		const std::string& s):
		archivo(a),
		separador(s)
	{}

	//Estos se ignoran...
	void visitar(Entrada_sala&) {}
	void visitar(Salida_sala&) {}
	void visitar(Bloque&) {}
	void visitar(Actor&) {}
	void visitar(Proyectil_obstaculo&) {}
	void visitar(Plataforma_flexible&) {}
	void visitar(Actor_control_direccion&) {}
	void visitar(Teletransportador&) {}
	void visitar(Actor_punto_guardado&) {}
	void visitar(Actor_recuperar_tiempo&) {}

	//Estos no.

	void visitar(Interruptor_invisible_no_repetible&);
	void visitar(Interruptor_invisible_repetible&);
	void visitar(Actor_audio&);
	void visitar(Cofre&);
	void visitar(Disparador_proyectiles&);
	void visitar(Actor_generico&);
	void visitar(Alternador_actor_generico&);
	void visitar(Escalera&);
	void visitar(Actor_diapositivas&);
	void visitar(Interruptor_contacto&);
	void visitar(Actor_info_juego&);
	void visitar(Actor_puzzle&);
	void visitar(Interruptor_repetible&);
	void visitar(Interruptor_no_repetible&);
	void visitar(Interruptor_auto&);
};

#endif
