#ifndef VISITANTE_TURNO_H
#define VISITANTE_TURNO_H

#include "factoria_actores.h"
#include "diccionario_info_juego.h" //Para sacar pop ups en algún caso.
#include <algorithm>

class Visitante_turno:public Visitante
{
	private:

	const std::vector<Actor*>& actores;
	const std::vector<Bloque *>& bloques;
	const std::vector<Actor_control_direccion *>& controles;
	const Cubo& cubo_sala;
	float delta;
	const unsigned int total_piezas;
	unsigned int id_enlace_activacion;
	unsigned int id_diapositiva;
	unsigned int id_info_juego;
	unsigned int id_cadena_acciones;

	//Esto tiene su razón de ser: guardamos el actor completo para que el 
	//controlador pueda localizarlo sin ids y lo pueda almacenar como clase
	//derivada. De esta forma nos ahorramos tener que crear un visitante que
	//sólo actúe sobre este tipo de actor cuando se resuelve un puzzle.

	Actor_puzzle * actor_puzzle; 

	std::vector<Actor*> nuevos;

	public:

	/////////////////////////////////////////////////
	//Métodos útiles.

	Visitante_turno(const std::vector<Actor*>& va,
		const std::vector<Bloque*>& vb,
		const std::vector<Actor_control_direccion*>& vc,
		const Cubo& c, unsigned int ptp, float pd)
		:Visitante(), actores(va), bloques(vb), controles(vc),
		cubo_sala(c), delta(pd), total_piezas(ptp),
		id_enlace_activacion(0),
		id_diapositiva(0), id_info_juego(0), id_cadena_acciones(0),
		actor_puzzle(NULL)
	{}
	~Visitante_turno(){}

	const std::vector<Actor *>& obtener_nuevos() {return nuevos;}

	unsigned int acc_id_diapositiva() const {return id_diapositiva;}
	unsigned int acc_id_info_juego() const {return id_info_juego;}
	unsigned int acc_id_enlace_activacion() const {return id_enlace_activacion;}
	unsigned int acc_id_cadena_acciones() const {return id_cadena_acciones;}
	Actor_puzzle * acc_actor_puzzle() const {return actor_puzzle;}

	void limpiar_estado() 
	{
		id_enlace_activacion=0;
		id_cadena_acciones=0;
		id_diapositiva=0;
		id_info_juego=0;
		actor_puzzle=NULL;
	}

	//////////////////////
	//Visitantes en si...

	//////////////////////
	//Visitantes en si...

	void visitar(Bloque&) {}
	void visitar(Actor&) {}
	void visitar(Entrada_sala&) {}
	void visitar(Salida_sala&) {}
	void visitar(Interruptor_repetible&) {}
	void visitar(Interruptor_no_repetible&){}
	void visitar(Interruptor_invisible_no_repetible&){}
	void visitar(Interruptor_invisible_repetible&){}
	void visitar(Cofre&) {}
	void visitar(Actor_control_direccion&) {}
	void visitar(Actor_generico&) {}
	void visitar(Plataforma_flexible&) {}
	void visitar(Escalera&) {}
	void visitar(Teletransportador&) {}
	void visitar(Actor_punto_guardado&) {}
	void visitar(Actor_recuperar_tiempo&) {}

	void visitar(Proyectil_obstaculo& actor) 
	{
		actor.turno(delta);

		actor.activar_movimiento(delta);		
		Cubo cubo=actor.como_cubo();

		if(Calculo_colisiones::cubo_en_colision_con_bordes_cubo(cubo, cubo_sala) 
		|| Calculo_colisiones::cubo_en_colision_con_bloques(cubo, bloques))
		{
			actor.marcar_para_borrar();
		}
		else if(controles.size())
		{	
			std::vector<Actor_control_direccion *>::const_iterator	ini=controles.begin(),
										fin=controles.end();

			while(ini < fin)
			{
				Actor_control_direccion& c=**ini;

				if(actor.en_colision_con(c.como_cubo()))
				{
					if(c.es_destruye_proyectil())
					{
						actor.marcar_para_borrar();
					}
					else
					{
						Vector_3d nuevo_vector=c.transformar_vector(actor.acc_vector());
						actor.establecer_vector(nuevo_vector);
					}
					break;
				}

				++ini;
			}
		}
	}

	void visitar(Disparador_proyectiles& actor) 
	{
		actor.turno(delta);
		if(actor.debe_disparar())
		{	
			nuevos.push_back(actor.generar_proyectil());

			//Si tiene vinculo lo guardamos en el
			//estado... Se lanzaría un visitante de activación para activarlo.
			id_enlace_activacion=actor.acc_id_enlace_activacion();
		}
	}

	
	void visitar(Alternador_actor_generico& actor) 
	{
		if(!actor.es_actor_recibido())
		{	
			Factoria_actores::Parametros_factoria params=Factoria_actores::generar_parametros_factoria_desde_actor(actor);
			params.insertar_param(actor.acc_tipo_actor());

			Actor_generico * nuevo=Factoria_actores::fabricar_actor_generico(params);
			if(nuevo)
			{
				actor.asignar_referencia(nuevo);
				nuevos.push_back(nuevo);
				actor.actualizar_estado_actor();
			}
		}

		actor.turno(delta);

		if(actor.es_recien_cambiado_estado())
		{
			//Controlar si es recien encendido o apagado. Si es el
			//caso se lanzaría un visitante de activación.

			id_enlace_activacion=actor.es_estado_on() ?
				actor.acc_id_enlace_activacion() :
				actor.acc_id_enlace_desactivacion();
		}
	}

	void visitar(Actor_audio& actor) 
	{
		actor.turno(delta);
	}

	void visitar(Actor_diapositivas& a)
	{
		if(a.es_activo() && !a.es_reproducido())
		{
			id_diapositiva=a.acc_id_diapositiva();
			a.reproducir();
		}
	}

	void visitar(Actor_info_juego& a)
	{
		if(a.es_activo())
		{
			id_info_juego=a.acc_id_info_juego();
			a.reproducir();
		}
	}

	//Este ocurre primero. El de colisión ocurre después.

	void visitar(Interruptor_contacto& a)
	{
		//El propio código de la clase filtra casos
		//en los que esto no sea válido.
		a.actualizar_estado();
		id_cadena_acciones=a.obtener_id_activo();
			
//		std::cout<<id_enlace_activacion<<" as id"<<std::endl;	
	}

	void visitar(Actor_puzzle& a)
	{
		//Si ha sido activado por un interruptor...
		if(a.es_activo())
		{
			//Si no tenemos ninguna pieza sacamos un info juego específico.
			if(!total_piezas)
			{
				id_info_juego=Diccionario_info_juego::D_NECESARIA_PIEZA;
				a.desactivar();
			}
			else
			{
				//Si el puzzle está activo y resuelto es que 
				//acaba de ser marcado como resuelto. Escogemos
				//la cadena de acciones y lo desactivamos para
				//siempre. El puzzle sólo es "activable" cuando
				//está sin resolver, de modo que ya nunca entrará
				//aquí.
			
				if(a.es_resuelto())
				{
					a.desactivar();
					id_cadena_acciones=a.acc_id_cadena();
				}
				//Si no está resuelto es que lo acabamos de activar
				//de modo que lo almacenamos para el controlador.
				else
				{
					actor_puzzle=&a;			
				}
			}
		}
	}

	//Si es activo llama a la cadena y se desactiva. Eso es todo. Se puede
	//volver a activar y repetiría de nuevo el proceso.

	void visitar(Interruptor_auto& a)
	{
		if(a.es_activo() && !a.es_usado())
		{
			id_cadena_acciones=a.acc_id_cadena();
			a.usar();
		}
	}
};

#endif
