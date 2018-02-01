#ifndef VISITANTE_INTERACCION_H
#define VISITANTE_INTERACCION_H

#include "interactuable.h"
#include "diccionario_info_juego.h"

class Visitante_interaccion:public Visitante
{
	private:

	struct Info_condiciones
	{
		bool amuleto;

		Info_condiciones(bool pamul):
			amuleto(pamul)
		{}
	};

	enum condiciones{
		C_NADA=0, //Sin condicion...
		C_AMULETO=1 //Necesario amuleto.
	};

	bool comprobar_condicion(unsigned int tc)
	{
		switch(tc)
		{
			case C_NADA: return true; break;
			case C_AMULETO: return info_condiciones.amuleto; break;
			default: return false; break;
		}
	}

	Datos_interaccion datos;

	public:

	static const int PIEZA_PUZZLE_NULA=-1;

	//////////////////////////////////////////////////
	//Este es el estado que tenemos que interpretar...

	Info_condiciones info_condiciones;
	unsigned int llaves;
	int id_pieza_puzzle;
	unsigned int id_entrada_guardar_partida;
	unsigned int id_info_juego;
	bool redimir_tiempo; //Indica si es un altar para obtener tiempo a cambio de bonus.

	/////////////////////////////////////////////////
	//Métodos útiles.

	Visitante_interaccion(unsigned int pl)
		:Visitante(), info_condiciones(false), llaves(pl),
		id_pieza_puzzle(PIEZA_PUZZLE_NULA),
		id_entrada_guardar_partida(0),
		id_info_juego(0), redimir_tiempo(false) 
	{}

	~Visitante_interaccion(){}

	unsigned int acc_cadena_acciones() {return datos.id_cadena;}
	int acc_id_pieza_puzzle() const {return id_pieza_puzzle;}
	int acc_id_entrada_guardar_partida() const {return id_entrada_guardar_partida;}
	unsigned int acc_id_info_juego() const {return id_info_juego;}

	void reiniciar_estado_cadena_acciones()
	{
		datos=Datos_interaccion();			
	}

	void configurar(bool pamul)
	{
		info_condiciones=Info_condiciones(pamul);
	}

	//////////////////////
	//Visitantes en si...

	void visitar(Bloque& a) {}
	void visitar(Actor& a) {}
	void visitar(Proyectil_obstaculo& a) {}
	void visitar(Entrada_sala& a) {}
	void visitar(Salida_sala& a) {}
	void visitar(Actor_control_direccion&) {}
	void visitar(Disparador_proyectiles&) {}
	void visitar(Actor_generico&) {}
	void visitar(Alternador_actor_generico&) {}
	void visitar(Actor_audio&) {}
	void visitar(Plataforma_flexible&) {}
	void visitar(Escalera&) {}
	void visitar(Actor_diapositivas&) {}
	void visitar(Interruptor_contacto&) {}
	void visitar(Teletransportador&) {}
	void visitar(Actor_info_juego&) {}
	void visitar(Actor_puzzle&) {}
	void visitar(Interruptor_auto&) {}

	//Estos hacen algo.
	void visitar(Interruptor_repetible& a) 
	{
		a.permitir(comprobar_condicion(a.acc_tipo_condicion()));
		datos=a.interactuar();
	}

	void visitar(Interruptor_no_repetible& a)
	{
		a.permitir(comprobar_condicion(a.acc_tipo_condicion()));
		datos=a.interactuar();
	}

	void visitar(Interruptor_invisible_no_repetible& a)
	{
		a.permitir(comprobar_condicion(a.acc_tipo_condicion()));
		datos=a.interactuar();
	}

	void visitar(Interruptor_invisible_repetible& a)
	{
		a.permitir(comprobar_condicion(a.acc_tipo_condicion()));
		datos=a.interactuar();
	}

	void visitar(Cofre& a) 
	{
		datos=a.interactuar();

		if(!llaves)
		{
			id_info_juego=Diccionario_info_juego::D_NECESARIA_LLAVE;
		}
		else
		{
			if(!a.es_abierto())
			{
				a.abrir();
				id_pieza_puzzle=a.acc_id_pieza();
			}
		}
	}

	void visitar(Actor_punto_guardado& a) 
	{
		datos=a.interactuar();
		id_entrada_guardar_partida=a.acc_id_entrada();
	}

	void visitar(Actor_recuperar_tiempo& a) 
	{
		datos=a.interactuar();
		redimir_tiempo=true;
	}
};

#endif
