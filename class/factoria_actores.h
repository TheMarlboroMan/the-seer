#ifndef FACTORIA_ACTORES_H
#define FACTORIA_ACTORES_H

#include "entrada_salida_sala.h"
#include "interruptor_repetible.h"
#include "interruptor_no_repetible.h"
#include "interruptor_invisible_no_repetible.h"
#include "interruptor_invisible_repetible.h"
#include "cofre.h"
#include "proyectil_obstaculo.h"
#include "disparador_proyectiles.h"
#include "actor_control_direccion.h"
#include "actor_generico.h"
#include "alternador_actor_generico.h"
#include "actor_audio.h"
#include "plataforma_flexible.h"
#include "escalera.h"
#include "teletransportador.h"
#include "actor_diapositivas.h"
#include "interruptor_contacto.h"
#include "actor_info_juego.h"
#include "actor_puzzle.h"
#include "actor_punto_guardado.h"
#include "actor_recuperar_tiempo.h"
#include "interruptor_auto.h"

extern DLibH::Log_base LOG;

class Factoria_actores
{
	public:

	enum tipos{
		TF_ENTRADA_SALA=1,
		TF_SALIDA_SALA=2,	
		TF_INTERRUPTOR_REPETIBLE=3,
		TF_ACTOR_AUDIO=4,
		TF_ALTERNADOR_ACTOR_GENERICO=5,
		TF_COFRE=6,
		TF_INTERRUPTOR_NO_REPETIBLE=7,
		TF_INTERRUPTOR_INVISIBLE_NO_REPETIBLE=8,
		TF_PLATAFORMA_FLEXIBLE=9,
		TF_ESCALERA=10,
		TF_CONTROL_DIRECCION=11,
		TF_PROYECTIL_OBSTACULO=12,
		TF_DISPARADOR_PROYECTILES=13,
		TF_ACTOR_GENERICO=14,
		TF_ACTOR_DIAPOSITIVA=15,
		TF_INTERRUPTOR_CONTACTO=16,
		TF_TELETRANSPORTADOR=17,
		TF_ACTOR_INFO_JUEGO=18,
		TF_INTERRUPTOR_INVISIBLE_REPETIBLE=19,
		TF_ACTOR_PUZZLE=20,
		TF_ACTOR_PUNTO_GUARDADO=21,
		TF_ACTOR_RECUPERAR_TIEMPO=22,
		TF_INTERRUPTOR_AUTO=23
	};

	struct Parametros_factoria
	{
		unsigned int x;
		unsigned int y;
		unsigned int z;

		std::vector<unsigned int> params;

		Parametros_factoria(unsigned int px, unsigned int py, unsigned int pz)
			:x(px), y(py), z(pz)
		{}

		void insertar_param(unsigned int val) {params.push_back(val);}
	};

	private:

	Factoria_actores();
	~Factoria_actores();

	static void error(const std::string& cadena){LOG<<"ERROR: Factoria parametros insuficientes :: "<<cadena<<std::endl;}
	
	static Actor * fabricar_entrada_sala(const Parametros_factoria& p);
	static Actor * fabricar_salida_sala(const Parametros_factoria& p);
	static Actor * fabricar_interruptor_repetible(const Parametros_factoria& p);
	static Actor * fabricar_interruptor_no_repetible(const Parametros_factoria& p);
	static Actor * fabricar_interruptor_invisible_no_repetible(const Parametros_factoria& p);
	static Actor * fabricar_interruptor_invisible_repetible(const Parametros_factoria& p);
	static Actor * fabricar_cofre(const Parametros_factoria& p);
	static Actor * fabricar_proyectil_obstaculo(const Parametros_factoria& p);
	static Actor * fabricar_disparador_proyectiles(const Parametros_factoria& p);
	static Actor * fabricar_control_direccion(const Parametros_factoria& p);
	static Actor * fabricar_alternador_actor_generico(const Parametros_factoria& p);
	static Actor * fabricar_actor_audio(const Parametros_factoria& p);
	static Actor * fabricar_plataforma_flexible(const Parametros_factoria& p);
	static Actor * fabricar_escalera(const Parametros_factoria& p);
	static Actor * fabricar_actor_diapositiva(const Parametros_factoria& p);
	static Actor * fabricar_interruptor_contacto(const Parametros_factoria& p);
	static Actor * fabricar_teletransportador(const Parametros_factoria& p);
	static Actor * fabricar_actor_info_juego(const Parametros_factoria& p);
	static Actor * fabricar_actor_puzzle(const Parametros_factoria& p);
	static Actor * fabricar_actor_punto_guardado(const Parametros_factoria& p);
	static Actor * fabricar_actor_recuperar_tiempo(const Parametros_factoria& p);
	static Actor * fabricar_interruptor_auto(const Parametros_factoria& p);

	public:
	
	//Este es ligeramente particular: el alternador requiere un puntero al 
	//tipo Actor_generico, que no podemos upcastear desde Actor * sin 
	//static casts.
	static Actor_generico * fabricar_actor_generico(const Parametros_factoria& p);

	static Actor * fabricar(unsigned int pt, const Parametros_factoria& params);
	static Parametros_factoria generar_parametros_factoria_desde_actor(const Actor& a)
	{
		return Parametros_factoria(a.acc_x(), a.acc_y(), a.acc_z());	
	}
};

#endif
