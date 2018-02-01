#ifndef VISITANTE_COLISION_H
#define VISITANTE_COLISION_H

/*Este visitante se dispara cuando un tipo de colisión tiene "llave", "bonus"
o algún efecto similar. Lo usamos para otorgar las llaves y bonus y para 
eliminar los actores recogibles.

Tiene el campo "cubo" por el funcionamiento del sistema: al detectar una 
colisión no se guarda el colisionable y se escala luego haciendo casts para
saber con qué hemos colisionado. En su lugar, se guarda el efecto de la colisión
y en función del mismo se crea o no el visitante. Al crearse el visitante, se
llama para cada uno de los actores susceptibles de generar la colisión y,
por supuesto, se compara si hay colisión con el cubo del jugador para ver
si algo ocurre o no.
*/

class Visitante_colision:public Visitante
{
	public:

	enum t_perder_vidas{
		T_NO=0, 
		T_SIEMPRE, 
		T_LAVA,
		T_AGUA,
		T_DARDO};

	//////////////////////////////////////////////////
	//Este es el estado que tenemos que interpretar...

	unsigned int id_sala;
	unsigned int id_entrada;
	unsigned int llaves;
	unsigned int bonus;
	unsigned int tanques_energia;
	unsigned int conceder_habilidad;	//Sólo concede una habilidad cada vez.
	unsigned int amuleto;
	unsigned int perder_vidas;
	unsigned int bonus_tiempo;

	Jugador& jugador;
	Escalera escalera;

	/////////////////////////////////////////////////
	//Métodos útiles.

	Visitante_colision(Jugador& j)
		:Visitante(), 
		id_sala(0), id_entrada(0), llaves(0), bonus(0), 
		tanques_energia(0), conceder_habilidad(0),
		amuleto(0),
		perder_vidas(false), bonus_tiempo(0), 
		jugador(j), escalera(0,0,0, 0,0,0, 0,0)
	{
	}

	~Visitante_colision()
	{
	}

	bool es_salida_sala() const {return id_sala && id_entrada;}
	unsigned int es_perder_vida() const {return perder_vidas;}
	
	//////////////////////
	//Visitantes en si...

	void visitar(Bloque& a) 
	{
		if(a.es_letal()) 
		{
			if(a.es_letal_general()) perder_vidas=T_SIEMPRE;
			else if(a.es_letal_lava()) perder_vidas=T_LAVA;
			else if(a.es_letal_agua()) perder_vidas=T_AGUA;
		}
	}

	void visitar(Actor&) {}
	void visitar(Teletransportador&) {}
	void visitar(Actor_punto_guardado&) {}
	void visitar(Actor_recuperar_tiempo&) {}

	void visitar(Proyectil_obstaculo& a) 
	{
		switch(a.acc_tipo_letal())
		{
			case Proyectil_obstaculo::T_DARDO: perder_vidas=T_DARDO; break;
			default: perder_vidas=T_SIEMPRE; break;
		}

		if(a.es_desaparece_en_colision()) a.marcar_para_borrar();
	}
	void visitar(Entrada_sala& a) {}
	void visitar(Salida_sala& a) 
	{
		id_sala=a.acc_id_sala();
		id_entrada=a.acc_id_entrada();
	}

	void visitar(Actor_control_direccion& a) {}
	void visitar(Disparador_proyectiles& a) {}
	void visitar(Alternador_actor_generico& a) {}

	void visitar(Actor_generico& a)
	{
		if(a.es_recogible()) a.marcar_para_borrar();
	
		unsigned int efecto=a.acc_efecto_colision();

		switch(efecto)
		{
			case Datos_colision::TC_LETAL: perder_vidas=T_SIEMPRE; break;
			case Datos_colision::TC_BONUS: ++bonus; break;
			case Datos_colision::TC_LLAVE: ++llaves; break;
			case Datos_colision::TC_BONUS_TIEMPO: ++bonus_tiempo; break;
			case Datos_colision::TC_TANQUE_ENERGIA: ++tanques_energia; break;
			case Datos_colision::TC_HABILIDAD_SUPER_SALTO:
			case Datos_colision::TC_HABILIDAD_LAVA:
			case Datos_colision::TC_HABILIDAD_AGUA:
			case Datos_colision::TC_HABILIDAD_RESISTENCIA_DARDOS:
			case Datos_colision::TC_HABILIDAD_TELETRANSPORTE:
			case Datos_colision::TC_HABILIDAD_IMPULSO:
			case Datos_colision::TC_HABILIDAD_PARAR_TIEMPO:
			case Datos_colision::TC_HABILIDAD_INSPECTOR_SECRETOS:
				conceder_habilidad=efecto;
			break;
			case Datos_colision::TC_AMULETO: amuleto=1; break;
		}
	}

	void visitar(Plataforma_flexible& a)
	{
		perder_vidas=a.es_letal() ? T_SIEMPRE : T_NO;
	}

	void visitar(Interruptor_repetible& a) {}
	void visitar(Interruptor_no_repetible& a){}
	void visitar(Interruptor_invisible_no_repetible& a){}
	void visitar(Interruptor_invisible_repetible& a){}
	void visitar(Cofre& a) {}
	void visitar(Actor_audio&) {}
	void visitar(Actor_diapositivas &) {}
	void visitar(Actor_info_juego &) {}
	void visitar(Actor_puzzle &) {}
	void visitar(Interruptor_auto &) {}

	void visitar(Escalera& a) 
	{
		//No enganchar si es desde arriba...
		float jz=jugador.como_cubo().final_para_eje(Definiciones::Z);
		float fe=a.como_cubo().final_para_eje(Definiciones::Z);

		if(a.es_activo() && jz < fe && (jugador.acc_direccion() & a.acc_orientacion()) )
		{
			escalera=a;
		}
	}

	//Este ocurre después del de colisión por lo que las
	//acciones van un tic tarde. Big deal.

	void visitar(Interruptor_contacto& a)
	{
		a.marcar_como_colision();
	}
};

#endif
