#include "controlador.h"

const float Controlador::Info_controles::T_HABILIDAD_PULSADA_CAMBIO=0.3;

/*A partir de un actor se calcula su representación y se centra...
Para que no haya movimiento cada vez que el actor se mueve, tenemos
que darle un cierto márgen. */

void Controlador::calcular_offsets_por_actor(const Actor& a)
{
	//Importante: los reiniciamos para que no traigan valores anteriores.
	offset_x=0;
	offset_y=0;

	//En primer lugar, tomamos el punto de origen y le sumamos
	//la mitad de cada dimensión ... 

	Punto_2d p=a.acc_coordenadas_origen_rep(); //obtener_coordenadas_origen_representacion();

//std::cout<<p.x<<","<<p.y<<std::endl;

	//Si ese punto de origen está fuera de la caja de márgen, tenemos
	//que corregirlo para que quede dentro de la caja.

	if(!DLibH::Herramientas_SDL::punto_en_caja(caja_margen_camara, p.x, p.y))
	{
		if(p.x + offset_x < caja_margen_camara.x)
		{
			offset_x=caja_margen_camara.x - p.x;
		}
		else if(p.x + offset_x > caja_margen_camara.x + caja_margen_camara.w)
		{
			offset_x=caja_margen_camara.x+caja_margen_camara.w - p.x;
		}

		if(p.y + offset_y < caja_margen_camara.y)
		{
			offset_y=caja_margen_camara.y - p.y;
		}
		else if(p.y + offset_y > caja_margen_camara.y + caja_margen_camara.h)
		{
			offset_y=caja_margen_camara.y+caja_margen_camara.h - p.y;
		}

//		debug_color_cam=DLibV::Gestor_color::color(255, 32, 32);

	}
	else
	{
//		debug_color_cam=DLibV::Gestor_color::color(255, 198, 198);
	}
}

/*Este es para centrar una sala en la pantalla. Si la sala es
demasiado grande puede que no quepa entera.*/

void Controlador::calcular_offsets_para_sala(const Sala& sala)
{
	//Obtenemos el punto central de la sala y su correspondencia
	//con un punto 2d al representarlo.

	Punto cs=sala.obtener_punto_central();
	Punto_2d centro_2d=Representable::obtener_coordenadas_representacion(cs.x, cs.y, cs.z);

	//Ese punto debería estar en el centro de la pantalla, luego...

	offset_x=(W_PANTALLA / 2) - centro_2d.x;
	offset_y=(H_PANTALLA / 2) - centro_2d.y;
}

void Controlador::dibujar_modo_juego()
{
	pantalla.rellenar(0,0,0);	

	/*Tratamiento de la cámara. Todos los actores actualizarían la caché de
	posición si procede pero antes haremos la del jugador, que es el centro
	de la cámara. */

	if(!sala_actual->es_camara_estatica()) 
	{
		jugador.actualizar_coordenadas_representacion();
		calcular_offsets_por_actor(jugador);
	}

	//Generar el vector, insertar la sala y el jugador. 
	Sala& s=*sala_actual;	//Por conveniencia..
	std::vector<Representable *> v_bruto;
	v_bruto.push_back(&jugador);
	v_bruto.push_back(&sombra);
	
	s.rellenar_vector_representables(v_bruto);

	//Podemos extraer del vector todo aquello que no esté
	//dentro de la cámara. 

	std::vector<Representable *> v;
	SDL_Rect caja_cam=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0, W_PANTALLA, H_PANTALLA); 

	DLibV::Representacion_bitmap_dinamica rep_base=DLibV::Representacion_bitmap_dinamica();
	DLibV::Representacion_agrupada_dinamica rep_ag=DLibV::Representacion_agrupada_dinamica(true);
	Bloque_transformacion_representable temp(rep_base, rep_ag);

	for(Representable * r : v_bruto)
	{
		r->transformar_representacion(temp);
		if(r->rep_final_es_dentro_de_caja(offset_x, offset_y, temp, caja_cam))
		{
			v.push_back(r);
		}
	}

	Ordenador_representables o;
	o.procesar(v);
	std::sort(v.begin(), v.end(), o);

	unsigned int total_rep=0;

	if(dibujar_planos & DP_FONDO) total_rep+=Representador::generar_vista(pantalla, camara, vector_plano_fondo, offset_x, offset_y, temp);
	if(dibujar_planos & DP_MEDIO) total_rep+=Representador::generar_vista(pantalla, camara, v, offset_x, offset_y, temp);
	if(dibujar_planos & DP_FRENTE) total_rep+=Representador::generar_vista(pantalla, camara, vector_plano_frente, offset_x, offset_y, temp);

	//Actores que pueden tener overlay...
	std::vector<Representable_overlay *> overlays=sala_actual->obtener_overlays();
	Representador::generar_overlays(pantalla, camara, overlays, offset_x, offset_y, temp);

	const Vector_3d vj=jugador.acc_vector();

	std::string coordenadas=
	DLibH::Herramientas::float_a_cadena(jugador.acc_x())+
	","+DLibH::Herramientas::float_a_cadena(jugador.acc_y())+
	","+DLibH::Herramientas::float_a_cadena(jugador.acc_z())+
	"\n"+DLibH::Herramientas::float_a_cadena(vj.x)+	
	","+DLibH::Herramientas::float_a_cadena(vj.y)+
	","+DLibH::Herramientas::float_a_cadena(vj.z);
//	" FPS:"+DLibH::Herramientas::entero_a_cadena(control_frames.acc_frames_contados());
/*
	" T:"+DLibH::Herramientas::entero_a_cadena(total_rep)+
	" "+DLibH::Herramientas::entero_a_cadena(offset_x)+
	","+DLibH::Herramientas::entero_a_cadena(offset_y);*/

	Representador::generar_texto_debug(pantalla, coordenadas);

	std::string temp_nombre_sala=DLibH::Herramientas::entero_a_cadena(sala_actual->acc_id())+" : "+txt_nombre_sala;
	Representador::generar_texto_nombre_sala(pantalla, temp_nombre_sala);

	const std::string tiempo_restante=Tiempo_juego::como_cadena(tiempo_juego.acc_segundos_restantes());
	Representador_tiempo_restante::generar_contador_tiempo(pantalla, tiempo_restante);
	
	//Estos son sólo si hay habilidades posibles...
	
	if(control_habilidades.es_habilidades_activadas())
	{
		if(control_habilidades.es_inspeccionando_secretos())
		{
			Representador::generar_icono_secreto(pantalla, sala_actual_con_secreto);
		}

		//Ahora los tanques de energia...
		std::vector<Control_energia::Info_tanque> tanques=control_energia.obtener_info_tanques();

		if(tanques.size())
		{
			unsigned int i=0;
			Representador::generar_fondo_info_tanque(pantalla);
			for(Control_energia::Info_tanque& info : tanques)
			{
				Representador::generar_info_tanque(pantalla, info.cantidad, info.usable, i);
				++i;
			}
		}

		unsigned int habilidad_actual=control_habilidades.acc_habilidad_activa();
		if(control_habilidades.es_habilidad_usable(habilidad_actual))
		{
			//TODO: Marcar de alguna forma si estamos en modo "escoger habilidad".
			Representador::generar_info_habilidad(pantalla, habilidad_actual);
		}
	}

//	DLibV::Primitiva_grafica_caja_estatica scroll(
//		caja_margen_camara, 
//		debug_color_cam);
//	scroll.establecer_alpha(128);
//	scroll.volcar(pantalla);

//	pantalla.flipar();
}

/*Sacaríamos del mapa aquellas cosas que fueran interactuables y en 
contacto con el cubo del jugador. El cubo del jugador tendría que 
ampliarse un poco, porque es muy posible que los trastos sean sólidos y 
no haya colisión (o si... yo que sé).*/

/*Este método se usa para ver si la posición del cubo está libre... Lo 
usaremos para ver si el jugador está en el aire.*/

bool Controlador::cubo_en_posicion_libre(const Cubo& c, bool verbose)
{
	if(verbose) 
	{
		std::cout<<"Cubo jugador como "<<c.origen.x<<","<<c.origen.y<<","<<c.origen.z<<","<<std::endl;
		LOG<<"Cubo jugador "<<c.origen.x<<","<<c.origen.y<<","<<c.origen.z<<","<<std::endl;
	}

	if(!Cubo::dentro_de(c, sala_actual->como_cubo(), true)) 
	{
		if(verbose) std::cout<<"Cubo fuera de sala"<<std::endl;
		return false;
	}
	else 
	{
		std::vector<Actor*>& actores=sala_actual->obtener_actores();
		if(Calculo_colisiones::cubo_en_colision_con_actores(c, actores, verbose)) 
		{
			return false;
		}
		else 
		{
			//const std::vector<Bloque *>& bloques=sala_actual->acc_bloques();
			const std::vector<Bloque *>& bloques=sala_actual->bloques_en_colision_con_cubo(c);
			if(Calculo_colisiones::cubo_en_colision_con_bloques(c, bloques, verbose)) 
			{
				return false;
			}
			else return true;
		}
	}
}

/*Esto sirve para activar todo aquello con lo que el jugador puede 
interactuar con la tecla de "usar". El visitante de interacción
guarda información en su estado: básicamente si debe ejecutar alguna
cadena de acciones y si debe restar llaves.*/

void Controlador::activar_interacciones_sala()
{
	Cubo cubo_j=jugador.obtener_cubo_interaccion();
	bool recuento=false;

	std::vector<Interactuable *> interactuables=sala_actual->obtener_interactuables();

	//El visitante guarda estado que podemos interpretar.
	Visitante_interaccion v(info_juego.acc_llaves());
	v.configurar(control_habilidades.es_habilidades_activadas());
	
	for(Interactuable * i : interactuables)
	{
		if(i->en_colision_con(cubo_j)) 
		{
			i->aceptar_visitante(v);

			unsigned int id_cadena=v.acc_cadena_acciones();
			if(id_cadena) ejecutar_cadena_acciones(id_cadena);
			recuento=true;
			v.reiniciar_estado_cadena_acciones();
		}
	}

	if(v.acc_id_pieza_puzzle()!=Visitante_interaccion::PIEZA_PUZZLE_NULA) abrir_cofre(v.acc_id_pieza_puzzle());
	if(v.acc_id_entrada_guardar_partida()) iniciar_proceso_guardado(v.acc_id_entrada_guardar_partida());
	if(v.acc_id_info_juego()) mostrar_mensaje_info_juego(v.acc_id_info_juego());
	if(v.redimir_tiempo) redimir_bonus_tiempo();
	if(recuento) recuento_secretos_sala();
}

void Controlador::recuento_secretos_sala()
{
	unsigned int total_secretos=0;

	std::vector<Interactuable *> interactuables=sala_actual->obtener_interactuables();
	
	for(Interactuable * i : interactuables)
	{
		if(i->es_detectable_secreto()) ++total_secretos;
	}

	sala_actual_con_secreto=total_secretos;
}

/*Las cosas que aparecen al fondo y al frente siempre tienen la misma ordenación
de modo que sólo se ordenan una vez. Eso se hace en este método.*/

void Controlador::preparar_vectores_representacion_estaticos(Sala &s)
{
	s.rellenar_vector_plano(vector_plano_frente, Representable::PL_FRENTE);
	Ordenador_representables opfr;
	opfr.procesar(vector_plano_frente);
	std::sort(vector_plano_frente.begin(), vector_plano_frente.end(), opfr);

	s.rellenar_vector_plano(vector_plano_fondo, Representable::PL_FONDO);
	Ordenador_representables opfo;
	opfo.procesar(vector_plano_fondo);
	std::sort(vector_plano_fondo.begin(), vector_plano_fondo.end(), opfo);		
}

void Controlador::loop(float delta)
{
	if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE))
	{
		datos_sistema.establecer_en_menu(true);
	}
	else 
	{
		if(input.es_tecla_down(SDLK_r)) recargar_sala_actual();
		if(input.es_tecla_down(SDLK_t)) recargar_mundo();
		if(input.es_tecla_down(SDLK_o)) modo_dios=!modo_dios;
		if(input.es_tecla_down(SDLK_y))
		{
			//Forzar una colisión con todo.
			Visitante_colision vc(jugador);
			auto actores=sala_actual->obtener_actores();
			for(Actor * a : actores) a->aceptar_visitante(vc);		
		}
		if(input.es_tecla_down(SDLK_u)) conceder_tanque_energia(1);
		if(input.es_tecla_down(SDLK_i)) 
		{
			conceder_amuleto();
			control_habilidades.conceder_habilidad(Control_habilidades::H_SUPER_SALTO); 
			control_habilidades.conceder_habilidad(Control_habilidades::H_RESISTENCIA_LAVA); 
			control_habilidades.conceder_habilidad(Control_habilidades::H_RESISTENCIA_AGUA); 
			control_habilidades.conceder_habilidad(Control_habilidades::H_RESISTENCIA_DARDO); 
			control_habilidades.conceder_habilidad(Control_habilidades::H_TELETRANSPORTE); 
			control_habilidades.conceder_habilidad(Control_habilidades::H_IMPULSO); 
			control_habilidades.conceder_habilidad(Control_habilidades::H_PARAR_TIEMPO); 
			control_habilidades.conceder_habilidad(Control_habilidades::H_INSPECTOR_SECRETOS); 
			for(int i=0; i<13; ++i) control_puzzle.recoger_pieza(i);
		}

		if(input.es_tecla_down(SDLK_PLUS)) 
		{
			iniciar_salida_sala(sala_actual->acc_id()+1, 1);
		}
		if(input.es_tecla_down(SDLK_MINUS)) 
		{
			iniciar_salida_sala(sala_actual->acc_id()-1, 1);
		}

		if(input.es_tecla_down(SDLK_1)) dibujar_planos=dibujar_planos ^ DP_FONDO;
		if(input.es_tecla_down(SDLK_2)) dibujar_planos=dibujar_planos ^ DP_MEDIO;
		if(input.es_tecla_down(SDLK_3)) dibujar_planos=dibujar_planos ^ DP_FRENTE;

		if(input.es_tecla_down(SDLK_q)) estado=E_JUEGO;
		if(input.es_tecla_down(SDLK_a)) estado=E_DEBUG_BLOQUES;

		switch(estado)
		{
			case E_JUEGO: loop_juego(delta); break;
			case E_DEBUG_BLOQUES: loop_debug_bloques(dbl); break;
		}

		//Este es el único acceso al game over de todo el programa: si
		//te liquidan en cualquier otro modo que permite que el tiempo
		//se agote lo que haría sería salir de este estado y pasar aquí.
		if(tiempo_juego.es_finalizado())
		{
			//TODO: Cambiaría a otro estado
			//datos_sistema.establecer_en_game_over();
		}
	}
	
} 

void Controlador::loop_debug_bloques(Debug_bloques& dbl)
{
	if(input.es_tecla_down(SDLK_LEFT) && dbl.x > 0) dbl.x--;
	else if(input.es_tecla_down(SDLK_RIGHT) && dbl.x < sala_actual->acc_w()-1) dbl.x++;
	else if(input.es_tecla_down(SDLK_UP) && dbl.y > 0) dbl.y--;
	else if(input.es_tecla_down(SDLK_DOWN) && dbl.y < sala_actual->acc_d()-1) dbl.y++;
	else if(input.es_tecla_down(SDLK_s) && dbl.z > 0) dbl.z--;
	else if(input.es_tecla_down(SDLK_w) && dbl.z < sala_actual->acc_h()-1) dbl.z++;
	else if(input.es_tecla_down(SDLK_z)) dbl.mostrar_plano=!dbl.mostrar_plano;

	else if(
		input.es_tecla_down(SDLK_b) ||
		input.es_tecla_down(SDLK_n) ||
		input.es_tecla_down(SDLK_m))
	{
		Bloque& b=sala_actual->obtener_bloque(dbl.x, dbl.y, dbl.z);
		if(!b.es_bloque_error())
		{
			if(input.es_tecla_down(SDLK_b)) b.establecer_plano_representacion(Representable::PL_FONDO); 
			else if(input.es_tecla_down(SDLK_n)) b.establecer_plano_representacion(Representable::PL_NORMAL);
			else if(input.es_tecla_down(SDLK_m)) b.establecer_plano_representacion(Representable::PL_FRENTE);
		}
	}

	if(input.acc_controles_sdl().recibe_eventos_teclado())
	{
		//Coger todos los bloques.
		std::vector<Representable *> bloques_a;
		std::vector<Representable *> bloques_b;
		std::vector<Representable *> bloques_c;
		std::vector<Bloque*>& original=sala_actual->acc_bloques();
		for(Bloque * pb : original)
		{
			Bloque& b=*pb;
			if(b.es_en_coordenadas(dbl.x, dbl.y, dbl.z)) b.establecer_debug_bloque(true);
			else b.establecer_debug_bloque(false);

			//Mostrar sólo una tajada... o no.

			if(!dbl.mostrar_plano || dbl.z==b.acc_matriz_z())
			{
				switch(b.obtener_plano_representable())
				{
					case Representable::PL_FONDO: bloques_a.push_back(&b); break;
					case Representable::PL_NORMAL: bloques_b.push_back(&b); break;
					case Representable::PL_FRENTE: bloques_c.push_back(&b); break;
				}
			}
		}
	
		pantalla.rellenar(0,0,0);	
//		Ordenador_representables o;
//		o.procesar(bloques);
//		std::sort(bloques.begin(), bloques.end(), o);

		DLibV::Representacion_bitmap_dinamica rep_base=DLibV::Representacion_bitmap_dinamica();
		DLibV::Representacion_agrupada_dinamica rep_ag=DLibV::Representacion_agrupada_dinamica(true);
		Bloque_transformacion_representable temp(rep_base, rep_ag);

		unsigned int total_rep=0;
		if(dibujar_planos & DP_FONDO) total_rep+=Representador::generar_vista(pantalla, camara, bloques_a, offset_x, offset_y, temp);
		if(dibujar_planos & DP_MEDIO) total_rep+=Representador::generar_vista(pantalla, camara, bloques_b, offset_x, offset_y, temp);
		if(dibujar_planos & DP_FRENTE) total_rep+=Representador::generar_vista(pantalla, camara, bloques_c, offset_x, offset_y, temp);

		std::string coordenadas=
		DLibH::Herramientas::entero_a_cadena(dbl.x)+
		","+DLibH::Herramientas::entero_a_cadena(dbl.y)+
		","+DLibH::Herramientas::entero_a_cadena(dbl.z)+
		" T:"+DLibH::Herramientas::entero_a_cadena(total_rep);

		Representador::generar_texto_debug(pantalla, coordenadas);
		Representador::generar_texto_nombre_sala(pantalla, txt_nombre_sala);

//		pantalla.flipar();
	
		for(Bloque * b : original) b->establecer_debug_bloque(false);
	}
}

/*Cambia el estado de pausa. Incluye código para controlar la pausa de los
sonidos. Si se quita la pausa no se reanudan todos los sonidos, puesto que 
algunos pueden haber sido puestos en pausa por exigencia del controlador.*/

void Controlador::pausar_despausar()
{
	pausa=!pausa;

	if(pausa)
	{
		Audio::pausar();
	}
	else 
	{
		Audio::despausar_excepto_activos();
	}
}

void Controlador::loop_juego(float delta)
{
	if(input.es_input_down(Input::I_PAUSA)) 
	{
		pausar_despausar();
	}
	if(input.es_tecla_down(SDLK_l)) 
	{
		modo_paso=!modo_paso;
	}

	if(!pausa)
	{
		//El delta puede ser 0 para el mundo con algo de parar el tiempo.
		//Conservamos el delta original para las cosas que afectan al
		//jugador.
		float delta_mundo=delta;
		if(control_habilidades.es_parando_tiempo()) delta_mundo=0.0;

		if(input.es_input_down(Input::I_MAPA))
		{
			datos_sistema.establecer_en_mapa();
			return;
		}

		if(jugador.es_estado_corre_tiempo())
		{
			Vector_3d v=jugador.acc_vector();
			float valor_vector=v.x+v.y+v.z;

			tiempo_juego.turno(delta_mundo);
			control_energia.turno(delta, !valor_vector); //Si el jugador está quieto recupera más rápido.
			control_habilidades.turno(delta);
			if(control_habilidades.es_reanudar_sonido()) 
			{
				finalizar_habilidad_parar_tiempo(true);
			}
		}

		if(jugador.es_finalizada_animacion_perder_vida()) 
		{
			finalizar_perder_vida();
			return;
		}
		
		if(jugador.es_fin_habilidad_impulso()) 
		{
			calcular_estado_fin_impulso_jugador(); 	
		}

/*Primero procesamos el turno de los actores de la sala que tengan que
hacerlo. Aquí iría todo aquello que se mueva o que realice algún tipo
de acción sobre otras cosas (como por ejemplo, disparar cosas). Todo
ese código irá en el propio visitante.

Luego pasaremos los controles de temporizador... Luego de todo esto 
miraremos si estamos en una posición legal. Si al terminar ambas cosas
no estamos en una posición legal es que nos hemos atrapado en algo.
*/

		std::vector<Actor *> con_turno=sala_actual->obtener_con_turno();

		const std::vector<Actor *>& todos_actores=sala_actual->obtener_actores();
		const std::vector<Bloque *>& bloques=sala_actual->acc_bloques();
		const std::vector<Actor_control_direccion *>& controles=sala_actual->obtener_controles_direccion();
		const Cubo& cubo_sala=sala_actual->como_cubo();

		Visitante_turno v(
			todos_actores, bloques, controles, cubo_sala, 
			control_puzzle.obtener_total_piezas(), delta_mundo);

		for(Actor * a : con_turno) 
		{
			a->aceptar_visitante(v);

			if(v.acc_id_enlace_activacion())
			{
				sala_actual->activar_desactivar_por_id(v.acc_id_enlace_activacion(), Visitante_activacion::E_ACTIVAR);
			}
			else if(v.acc_id_cadena_acciones())
			{
				ejecutar_cadena_acciones(v.acc_id_cadena_acciones());
			}
			else if(v.acc_id_diapositiva())
			{
				datos_sistema.establecer_en_diapositiva(v.acc_id_diapositiva());
			}
			else if(v.acc_id_info_juego())
			{
				mostrar_mensaje_info_juego(v.acc_id_info_juego());
			}
			else if(v.acc_actor_puzzle()!=NULL)
			{
				actor_puzzle_actual=v.acc_actor_puzzle();
				iniciar_puzzle(actor_puzzle_actual->acc_id_puzzle());
			}

			//Quizás también diapositivas aquí??
			v.limpiar_estado();
		}

		/*Ahora los controles de temporizador...*/
		procesar_controles_temporizador(delta_mundo);

		const std::vector<Actor *>& nuevos=v.obtener_nuevos();
		if(nuevos.size())
		{
			for(Actor * a : nuevos) sala_actual->insertar_actor(a);	
			sala_actual->unificar_actores();
		}
	
		//Si el jugador no está en un estado en que deba comprobar 
		//colisiones (perdiendo una vida, por ejemplo) no tenemos 
		//que hacer nada porque simplemente no se estaría moviendo por
		//el espacio del juego.

		if(jugador.es_comprueba_colisiones()) 
		{
			//TODO: Esto no debería ocurrir nunca más.
			if(!cubo_en_posicion_libre(jugador.como_cubo())) 
			{
				std::cout<<"PERDER VIDA POR POSICIÓN OCUPADA main"<<std::endl;
				LOG<<"PERDER VIDA POR POSICIÓN OCUPADA main"<<std::endl;
				cubo_en_posicion_libre(jugador.como_cubo(), true);
				pausar_despausar();	
				//TODO: Si no podemos determinar bien esto, perder vida simplemente.
			}
	
			info_controles.reset_ejes();

			//Si es posible controlar al jugador vamos a recoger el input del usuario...
			if(jugador.es_controlable()) procesar_input_usuario(delta);

			//Reiniciar la sombra, puede cambiar al procesar el turno del jugador.
			sombra.establecer_estado(Sombra::E_NORMAL);
			procesar_turno_jugador(info_controles.x, info_controles.y, delta);
		}

		//Avanzar contadores de estado.
		jugador.procesar_turno(delta);

		if(sala_actual->actualizar_borrado_actores())
		{
			control_actores_persistentes.actualizar_borrado_actores(sala_actual->acc_id());
		}

		//si paras el tiempo esto no hace nada.
		Temporizador_animaciones::procesar_tiempo_animacion(delta_mundo);
	}

	//Esto está aquí para que no se nos cuele un frame de juego
	//antes de iniciar una diapositiva que está ahí cuando empieza
	//una sala. Los sonidos están fuera porque podemos haber ordenado que
	//suene un sonido a la vez que queremos mostrar un pop up.

	if(datos_sistema.es_modo_juego())
	{
		dibujar_modo_juego();
		pantalla.procesar_procesos(delta);
	}

	procesar_sonidos_activos();

	if(modo_paso)
	{
		bool paso=false;

		while(!paso)
		{
			input.acc_controles_sdl().recoger();
			if(input.acc_controles_sdl().es_tecla_down(SDLK_k)) paso=true;
			if(input.acc_controles_sdl().es_tecla_down(SDLK_l)) 
			{
				paso=true; 
				modo_paso=false;
			}
			SDL_Delay(16);
		};
	}
}

/*El input del usuario se condensa en dos variables: x e y. Los valores que 
tengan se usarán luego según el estado actual para interpretarse de una u otra
forma.

El resto de input tiene efecto inmediato.*/

void Controlador::procesar_input_usuario(float delta)
{
	//El primer punto es saber si estamos en modo de "escoger habilidad"
	//o no. Si hemos estado pulsando la tecla de habilidad X tiempo y la
	//mantenemos pulsada entramos en modo cambio.

	if(info_controles.es_modo_cambio() && control_habilidades.es_habilidades_activadas() && input.es_input_pulsado(Input::I_HABILIDAD))
	{
		if(input.es_input_down(Input::I_IZQUIERDA) || input.es_input_down(Input::I_ARRIBA)) control_habilidades.habilidad_anterior();
		else if(input.es_input_down(Input::I_DERECHA) || input.es_input_down(Input::I_ABAJO)) control_habilidades.habilidad_siguiente();
	}
	//Si no estamos en modo escoger habilidad, entramos en modo normal.
	else
	{
		//Ir sumando cantidad...
		if(input.es_input_pulsado(Input::I_HABILIDAD)) 
		{
			info_controles.t_habilidad_pulsada+=delta;
		}
		else if(input.es_input_up(Input::I_HABILIDAD))
		{			
			if(!info_controles.es_modo_cambio()) activar_habilidad_actual();
			info_controles.t_habilidad_pulsada=0.0;
		}

		if(input.es_input_down(Input::I_INTERACCION)) 
		{
			if(jugador.es_en_escalera())
			{
				soltar_jugador_escalera();
			}
			else
			{
				activar_interacciones_sala();
				intentar_descenso_escalera();
			}
		}

		//Activar o desactivar el correr.
		jugador.establecer_corriendo(input.es_input_pulsado(Input::I_CORRER));

		//Este esquema de control sólo permite moverse en una dirección a la vez.

		if(input.es_input_pulsado(Input::I_IZQUIERDA)) info_controles.x=-1;
		else if(input.es_input_pulsado(Input::I_DERECHA)) info_controles.x=1;
		else if(input.es_input_pulsado(Input::I_ARRIBA)) info_controles.y=-1;
		else if(input.es_input_pulsado(Input::I_ABAJO)) info_controles.y=1;

		/*Al realizar el salto se anula cualquier vector de movimiento
		en plano que sea distinto a la orientación del jugador. De
		este modo se impiden saltos en diagonal. Para conservarlo es
		importante controlar el salto después del movimiento... Dentro
		del jugador se anulan los vectores. Aquí se anula el input.

		Nótese que aún es posible bugear junto al iniciar una caida,
		pero eso ni importa.*/

		if(input.es_input_down(Input::I_SALTAR))
		{
			if(jugador.puede_saltar())
			{
				jugador.saltar();

				sonidos_activos.cambiar(Audio::S_SALTO, 100, true);
				unsigned int direccion=jugador.acc_direccion();
				if(direccion==Definiciones::POS_N || direccion==Definiciones::POS_S) info_controles.x=0;
				else info_controles.y=0;
			}
		}
	}
}

void Controlador::procesar_sonidos_activos()
{
	const std::vector<Sonidos_activos::Datos>& activos=sonidos_activos.obtener_datos();
	for(const Sonidos_activos::Datos& d : activos)
	{
		if(d.activo)
		{
			Audio::insertar_sonido(
				DLibA::Estructura_sonido(
				DLibA::Gestor_recursos_audio::obtener_sonido(d.id_sonido), d.volumen));
		}
	}

	sonidos_activos.reiniciar();
}

/*Detiene todos los sonidos activos e inicia el controlador de puzzle.
El id que se envíe será el id del puzzle que se abra. El segundo id
es el id del actor, que se envía para luego poderlo capturar cuando
el puzzle haya finalizado.*/

void Controlador::iniciar_puzzle(unsigned int idp)
{
	Audio::pausar_canales_activos();
	datos_sistema.establecer_en_puzzle(idp);
}

/*Se indica que un puzzle ha finalizado. El puzzle finalizado fue iniciado
por el actor almacenado en actor_puzzle_actual de forma que no hace falta
ningún visitante ni cast para poder usarlo directamente.

Si el parámetro que se pasa es true, se da el puzzle como resuelto. Al marcar el actor como resuelto se consigue
que entre en el visitante de turno una vez más para indicar la cadena que debe
ejecutar. Como parte del proceso, el actor de desactiva para siempre en el 
visitante.

Si el parámetro es false de desactiva el actor para que el puzzle no se ejecute
inmediatamente de nuevo. 

En cualquier caso se limpia el puntero al actor. 

Este método está fuera del loop principal y es llamado directamente desde el
sistema de modo que en el siguiente loop, cuando llegue el visitante de turno,
se realizarán las acciones que sean.
*/

void Controlador::solucionar_puzzle(bool resuelto)
{
	if(resuelto) actor_puzzle_actual->resolver();
	else actor_puzzle_actual->desactivar();

	actor_puzzle_actual=NULL;
}

/*Detiene todos los sonidos activos y muestra el mensaje de info
juego de turno.*/

void Controlador::mostrar_mensaje_info_juego(unsigned int id_info)
{
	Audio::pausar_canales_activos();
	datos_sistema.establecer_en_info_juego(id_info);
}


/*Intenta usar alguna de las habilidades activas.*/

void Controlador::activar_habilidad_actual()
{
	unsigned int habilidad_actual=control_habilidades.acc_habilidad_activa();

	switch(habilidad_actual)
	{
		case Control_habilidades::H_TELETRANSPORTE:
			if(control_energia.hay_tanques_completos(Control_habilidades::COSTE_TANQUES_TELETRANSPORTE)
			&& control_habilidades.es_habilidad_usable(Control_habilidades::H_TELETRANSPORTE)
			)
			{
				activar_habilidad_teletransportador();
			}
		break;


		case Control_habilidades::H_IMPULSO:
			if(jugador.es_en_tierra()
			&& control_habilidades.es_habilidad_usable(Control_habilidades::H_IMPULSO)
			&& control_energia.hay_tanques_completos(Control_habilidades::COSTE_TANQUES_IMPULSO))
			{
				control_energia.gastar_completo(Control_habilidades::COSTE_TANQUES_IMPULSO);
				sonidos_activos.cambiar(Audio::S_IMPULSO, 100, true);
				jugador.activar_habilidad_impulso(300.0);
			}
		break;

		case Control_habilidades::H_PARAR_TIEMPO:
			if(!control_habilidades.es_parando_tiempo()
			&& control_habilidades.es_habilidad_usable(Control_habilidades::H_PARAR_TIEMPO)
			&& control_energia.hay_tanques_completos(Control_habilidades::COSTE_TANQUES_PARAR_TIEMPO))
			{
				control_energia.gastar_completo(Control_habilidades::COSTE_TANQUES_PARAR_TIEMPO);
				control_habilidades.activar_paro_tiempo();
				Audio::pausar_canales_activos();
				sonidos_activos.cambiar(Audio::S_DETENER_TIEMPO, 100, true);
				insertar_fundido_pantalla(255, 255, 255, 64, 0, Control_habilidades::SEGUNDOS_UTIL_PARAR_TIEMPO);
			}
		break;

		case Control_habilidades::H_INSPECTOR_SECRETOS:
			if(!control_habilidades.es_inspeccionando_secretos()
			&& control_habilidades.es_habilidad_usable(Control_habilidades::H_INSPECTOR_SECRETOS) 
			&& control_energia.hay_tanques_completos(Control_habilidades::COSTE_TANQUES_INSPECTOR_SECRETOS) )
			{
				control_energia.gastar_completo(Control_habilidades::COSTE_TANQUES_INSPECTOR_SECRETOS);
				control_habilidades.activar_inspector_secretos();
			}
		break;

		case Control_habilidades::H_SUPER_SALTO:
			if(jugador.puede_super_saltar()
			&& control_habilidades.es_habilidad_usable(Control_habilidades::H_SUPER_SALTO)
			&& control_energia.hay_tanques_completos(Control_habilidades::COSTE_TANQUES_SUPER_SALTO))
			{
				control_energia.gastar_completo(Control_habilidades::COSTE_TANQUES_SUPER_SALTO);
				sonidos_activos.cambiar(Audio::S_IMPULSO, 100, true);
				insertar_fundido_pantalla(160, 160, 160, 128, 0, 0.3);
				jugador.super_saltar();

			}
		break;
	}
}

void Controlador::activar_habilidad_teletransportador()
{
	const Cubo caja_j=jugador.como_cubo();
	const Teletransportador * const t=sala_actual->obtener_teletransportador_en_colision(caja_j);

	if(t)
	{
		unsigned int id_t=t->acc_id_conexion();
		const Teletransportador * const t2=sala_actual->obtener_teletransportador_por_id(id_t);
		if(t2)
		{
			control_energia.gastar_completo(Control_habilidades::COSTE_TANQUES_TELETRANSPORTE);
			Punto p=t2->obtener_destino();
			jugador.establecer_posicion(p);
			insertar_fundido_pantalla(20, 120, 220, 128, 0, 0.3);
		}
	}
}


void Controlador::insertar_fundido_pantalla(unsigned int r,
		unsigned int g, unsigned int b,
		unsigned int alpha_a, unsigned int alpha_b,
		float t)
{
	pantalla.adjuntar_proceso(
		DLibV::Proceso_superficie_fundido::generar(
			pantalla.acc_superficie(),
			r, g, b,
			alpha_a, alpha_b,
			t));
}

void Controlador::forzar_pausa_debug()
{
	bool salir=false;

	while(!salir)
	{
		input.acc_controles_sdl().recoger();
		if(input.acc_controles_sdl().es_tecla_down(SDLK_k)) salir=true;
		SDL_Delay(16);
	};
}

void Controlador::procesar_controles_temporizador(float delta)
{
	std::vector<Control_temporizador>::iterator 	ini=controles_temporizador.begin(),
							fin=controles_temporizador.end();

	while(ini < fin)
	{
		bool borrar=false;

		if((*ini).procesar_turno(delta))
		{
			LOG<<"Desde temporizador, ejecutar cadena "<<(*ini).acc_cadena_actual()<<std::endl;
			ejecutar_cadena_acciones((*ini).acc_cadena_actual()/*,(*ini).es_encendido()*/);

			if((*ini).es_fin_de_cadena()) 
			{
				LOG<<"Es fin de cadena"<<std::endl;
				borrar=true;
			}
			else 	
			{
				unsigned int id_siguiente=(*ini).acc_siguiente_temporizador();
				if(!controlador_datos_temporizador.existe_entrada_por_id(id_siguiente)) 
				{
					LOG<<"ERROR: Controlador tiempo no localizado para actualizar "<<id_siguiente<<std::endl;
					borrar=true;
				}
				else
				{
					Datos_temporizador dt=controlador_datos_temporizador.obtener_por_id(id_siguiente);
					(*ini).actualizar_informacion(dt);
				}
			}
		}

		if(borrar)
		{
			ini=controles_temporizador.erase(ini);
			fin=controles_temporizador.end();
		}
		else ++ini;
	}
}

/*Este trozo lo dejamos dedicado para el jugador... Aunque lo podríamos
descomponer aún más y reusarlo para otros. Se asume que se produce colisión
tan sólo con un bloque a la vez.

Cuando estamos en animación de perder vida o algún tipo si la hubiere no vamos
a entrar aquí.

Se hace una copia del vector original para ser usado sólo en caso de que se
cambie de habitación, para preservar la velocidad.
*/

void Controlador::procesar_turno_jugador(int x, int y, float delta)
{
	Vector_3d vector_original=jugador.acc_vector();

	if(jugador.es_en_escalera())
	{
		procesar_turno_jugador_escalera(x, y, delta);
	}
	else
	{
		procesar_turno_jugador_tierra_aire(x, y, delta);
	}

	//Nos queda únicamente procesar colisiones con efecto, valorar si el 
	//jugador está en el aire y calcular la posición de la sombra. Si no
	//hay control no vamos a recalcular un nuevo estado.

	procesar_colisiones_turno(delta, vector_original);
	if(jugador.es_afectado_por_gravedad()) controlar_accion_gravedad_en_jugador();
	obtener_posicion_sombra();
}

void Controlador::procesar_turno_jugador_tierra_aire(int x, int y, float delta)
{
	float vz_orig=jugador.acc_vector().z;

	Datos_colision col;
	Cubo cubo_sala=sala_actual->como_cubo();
	std::vector<Actor *>& actores=sala_actual->obtener_actores();

	bool con_correccion_z=false;

	//Inicio del movimiento en Z... Si se detecta algún tipo de rampa 
	//reajustaremos la altura para que el movimiento en X e Y tenga la 
	//altura correcta y nos permita subir con suavidad.
	
	//Si el jugador está usando el impulso, será su propia lógica
	//quien retire la gravedad.

	Calculo_colisiones::establecer_cubo_a_original(jugador.como_cubo());
	jugador.activar_movimiento(delta, Definiciones::Z);
	std::vector<Bloque *> bloques=sala_actual->bloques_en_colision_con_cubo(jugador.como_cubo());

	col=Calculo_colisiones::obtener_info_colision_completa(jugador, cubo_sala, bloques, actores, jugador.vector_para_eje(Definiciones::Z), Definiciones::Z, bloques_colisiones_con_efecto_en_turno, actores_colisiones_con_efecto_en_turno);
	if(col.es_considerable()) 
	{
		jugador.recibir_datos_colision(col);
		con_correccion_z=con_correccion_z || col.es_con_correccion_z();
		if(vz_orig < 0.0) evaluar_colision_jugador_con_suelo(vz_orig);
	}

	/*Ahora se procesan Y y X. Mucho ojo con intentar ahorrar en todo esto:
	el sistema está tan interconectado que si dejamos de comprobar un 
	eje y estamos en una rampa el comportamiento puede no ser el deseado.
	Por ejemplo, si no recibimos el input de movimiento las animaciones
	no funcionarían ni se restarían los vectores. Si no activamos el 
	movimiento... No pasaría nada, pero realmente eso no hace mucho. 
	Obtener la info de colisión completa es obligatorio: aunque estés 
	quieto es lo que calcula las correcciones en las rampas. Y bueno,
	básicamente todo está muy conectado.*/

	jugador.recibir_input_movimiento(x, delta, Definiciones::X);
	Calculo_colisiones::establecer_cubo_a_original(jugador.como_cubo());	
	jugador.activar_movimiento(delta, Definiciones::X);
	Vector_3d v=jugador.acc_vector();
	if(v.x)
	{
		bloques=sala_actual->bloques_en_colision_con_cubo(jugador.como_cubo());
		col=Calculo_colisiones::obtener_info_colision_completa(jugador, cubo_sala, bloques, actores, v.x, Definiciones::X, bloques_colisiones_con_efecto_en_turno, actores_colisiones_con_efecto_en_turno);
		if(col.es_considerable()) 
		{
			jugador.recibir_datos_colision(col);
			con_correccion_z=con_correccion_z || col.es_con_correccion_z();
//LOG<<"X: "<<jugador.como_cubo().origen.x<<","<<jugador.como_cubo().origen.z<<" "<<col.es_con_correccion_z()<<std::endl;
		}	
	}

	jugador.recibir_input_movimiento(y, delta, Definiciones::Y);
	Calculo_colisiones::establecer_cubo_a_original(jugador.como_cubo());	
	jugador.activar_movimiento(delta, Definiciones::Y);
	v=jugador.acc_vector();
	if(v.y)
	{
		bloques=sala_actual->bloques_en_colision_con_cubo(jugador.como_cubo());
		col=Calculo_colisiones::obtener_info_colision_completa(jugador, cubo_sala, bloques, actores, v.y, Definiciones::Y, bloques_colisiones_con_efecto_en_turno, actores_colisiones_con_efecto_en_turno);
		if(col.es_considerable()) 
		{
			jugador.recibir_datos_colision(col);
			con_correccion_z=con_correccion_z || col.es_con_correccion_z();
//LOG<<"Y: "<<jugador.como_cubo().origen.x<<","<<jugador.como_cubo().origen.z<<" "<<col.es_con_correccion_z()<<std::endl;
		}
	}

	//Si ha habido correccion Z podemos querer lanzar una última
	//comprobación para prevenir posiciones ilegales. Esto está aquí porque
	//al subir una rampa con un muro al final a veces hay posiciones
	//ilegales... Hemos intentado repararlo en el código de colisión pero
	//hay demasiados casos. Lo que haremos es sacar todos los bloques y
	//si estamos dentro de una rampa, ajustar.
	//El tema del impulso está por si acaso: cuando vas con el impulso
	//puedes subir por una rampa que tenga al final una pared. Al no tener
	//gravedad no cuenta que tienes corrección z y puedes acabar en una 
	//posición ilegal.
	 
	if(con_correccion_z || jugador.es_habilidad_impulso())
	{
		Cubo cj=jugador.como_cubo();
		bloques=sala_actual->bloques_en_colision_con_cubo(cj);
	
		for(Bloque * pbl : bloques)
		{
			Bloque& bl=*pbl;
		//	Cubo cb=bl.como_cubo();

			if(bl.es_colisionable() && bl.es_tipo_rampa() && !bl.cubo_libre_en_rampa(cj, true))
			{
				int z=bl.calcular_z_encima(cj);

				jugador.establecer_posicion_por_eje(z, Definiciones::Z);
				//jugador.establecer_en_tierra(); //Esto evita que vaya "resbalando" al subir a una rampa saltando. Not really.
				break;
			} 
		}
	}
}


void Controlador::procesar_turno_jugador_escalera(int x, int y, float delta)
{
	jugador.recibir_input_movimiento(y, delta, Definiciones::Z);

	//Limitar movimiento en escalera y tratar los
	//casos especiales de salida de las mismas.

	float jz=jugador.acc_z();
	float fjz=jz+jugador.acc_h();
	float ez=escalera_actual.acc_z();
	float fez=ez+escalera_actual.acc_h();		

	if(jz <= ez && y > 0)
	{
		if(escalera_actual.es_base_invertir())
		{
			bajar_jugador_escalera();
		}
		else
		{
			escalera_actual.hacer_nula();
			jugador.establecer_en_aire();
		}
	}
	else if(y < 0)
	{
		if(escalera_actual.es_tope_escalable())
		{
			if(jz >= fez) trepar_jugador_escalera();			
		}
		else //No permitir subir más...
		{
			if(fjz >= fez)	
			{
				jugador.establecer_posicion_por_eje(floor(fez - jugador.acc_h()), Definiciones::Z);
				jugador.cancelar_vector(Definiciones::Z);
			}
		}
	}

	Calculo_colisiones::establecer_cubo_a_original(jugador.como_cubo());
	jugador.activar_movimiento(delta, Definiciones::Z);

	Cubo cubo_sala=sala_actual->como_cubo();
	std::vector<Bloque *> bloques=sala_actual->bloques_en_colision_con_cubo(jugador.como_cubo());
	std::vector<Actor *>& actores=sala_actual->obtener_actores();

	Datos_colision col=Calculo_colisiones::obtener_info_colision_completa(jugador, cubo_sala, bloques, actores, jugador.vector_para_eje(Definiciones::Z), Definiciones::Z, bloques_colisiones_con_efecto_en_turno, actores_colisiones_con_efecto_en_turno);
	if(col.es_considerable()) 
	{
		jugador.recibir_datos_colision(col);
	}
}

/*Cuando el jugador ha terminado de usar la habilidad de impulso puede estar
en tierra o en el aire - si hubiera chocado con una escalera o algo letal se
cambiaría sólo de estado -. Aquí calculamos en qué estado queda. Al cambiar
el estado se finaliza de forma natural el estado de usar el impulso.
Esto se salta el snap de rampas alegremente, pero bueno.
*/

void Controlador::calcular_estado_fin_impulso_jugador()
{
	Cubo cubo_j=jugador.como_cubo();
	cubo_j.origen.z-=1;
	bool aire=cubo_en_posicion_libre(cubo_j);

	if(aire)
	{
		jugador.establecer_en_aire();
	}
	else 
	{
		jugador.establecer_en_tierra();
	}
}

//Cuando hay colisión en el eje Z y el vector Z apunta hacia
//el suelo, ejecutamos estas correcciones.

void Controlador::evaluar_colision_jugador_con_suelo(float vz_orig)
{
	sonidos_activos.cambiar(Audio::S_CAIDA, 100, true);

	//Si ha tocado tierra pega un sonido y controlamos una gran caida...

	if(vz_orig <= -180.00)
	{
		jugador.establecer_pausa_caida();
		sonidos_activos.cambiar(Audio::S_CAIDA_LARGA, 100, true);
	}
	else
	{
		jugador.establecer_en_tierra();
	}
}

//Control de caso borderline: bajar una rampa caminando para que haga snap
//y no te coloque en el aire, control de que la tierra no ha desaparecido bajo
//tus pies y ese tipo de cosas.

void Controlador::controlar_accion_gravedad_en_jugador()
{
	bool original_aire=jugador.es_en_aire();

	Cubo cubo_j=jugador.como_cubo();
	cubo_j.origen.z-=1;

	bool aire=cubo_en_posicion_libre(cubo_j);
	Vector_3d v=jugador.acc_vector();

	//Ahora estoy en el aire...
	if(aire)
	{
		if(!original_aire)
		{
			if(!v.z)
			{
				//Si de repente perdemos pie vemos si
				//estamos bajando una rampa...

				if(!calcular_snap())
				{
					jugador.establecer_en_caida();
				}
				else
				{
					jugador.establecer_en_tierra();
				}
			}
			else
			{
/*				//Saltando cerca de una rampa o al borde de plataforma...
				if(v.z > 0.0 && calcular_snap())
				{
					jugador.establecer_en_tierra();
				}
				//Cayendo o saltando normal...
				else
				{
*/
					jugador.establecer_en_aire();
/*
				}
*/
			}
		}
		else
		{
			jugador.establecer_en_aire();
		}
	}
	//Estoy en tierra... Esto impide que al saltar sobre una rampa que sube
	//se quede como en un tobogán :D.
	else
	{
		if(!jugador.es_en_pausa_caida() && v.z <= 0.0) jugador.establecer_en_tierra();
	}
	//Ahora no estoy en el aire o estoy menos de un pixel por encima...
	//No estoy muy seguro de que alguna de estas cosas sea ya relevante.
/*	else 		
	{
		if(!original_aire)
		{
			//Caso borderline de muchos FPS de forma q aún estamos en el aire 
			//tras un tic después de saltar...
	
			if(v.z > 0)
			{
				//No hagas nada, déjalo como está.
			}
			else if(!jugador.es_en_pausa_caida())
			{
		 		jugador.establecer_en_tierra();
			}
		}
	}
*/
}

/*OJO: El snap no considera cosas que no sean bloques
por lo que puede hacer cosas sobre actores.*/

bool Controlador::calcular_snap()
{
	Cubo cubo_j=jugador.obtener_cubo_snap();
	std::vector<Bloque *> bloques=sala_actual->bloques_en_colision_con_cubo(cubo_j);

	if(!bloques.size()) 
	{
		return false;
	}
	else
	{
		int t=-1;
		for(Bloque * blp : bloques)
		{
			int tt=blp->calcular_z_encima(cubo_j);
			if(tt > t) t=tt;
		}

		bool resultado=jugador.recibir_snap(t);
		return resultado;
	}
}

/*Este método obtiene la posición Z de la sombra. Como no tenemos 
métodos de colisión con scan tendremos que hacerlo a mano en dos fases:
en la primera fase comprobaremos contra los bloques que están debajo 
del jugador en cada una de las esquinas. Buscaremos la parte más alta.
	
En la siguiente fase comprobaremos si hay algún actor bajo el jugador,
porque la sombra podría estar por encima.*/

/*TODO: Si no estamos en el aire esto sería relativamente optimizable a
comprobar los bloques que tengamos justo debajo. Si hay alguno pues la 
sombra está ahí. Si no hay bloque entoces estamos encima de un actor.
Si estamos en el aire usaríamos este otro método.*/ 

void Controlador::obtener_posicion_sombra()
{
	Cubo c=jugador.como_cubo();
	unsigned int x;
	unsigned int y;
	int z=c.origen.z;	//Este tiene signo porque iremos restando.
	unsigned int z_sombra=0;

	unsigned int ori_x=c.origen.x;
	unsigned int max_x=ori_x+c.w;
	unsigned int ori_y=c.origen.y;
	unsigned int max_y=ori_y+c.d;
//	unsigned int ori_z=DLibH::Herramientas::round(c.origen.z);
	unsigned int ori_z=ceil(c.origen.z);

	
	//Vamos bajando: el primer bloque que encontremos vale.
	//Debemos comprobar cada una de las cuatro esquinas del jugador.
	//Detalles importantes: el <= es para que puedas comprobar hasta
	//el último punto, pero si estas contra un bloque puede ser 
	//contraproducente porque puede sacar la sombra encima. Por eso
	//se comprueba también que la sombra no puede estar sobre el
	//origen del jugador y se añade el -1, que no parece tener mal
	//efecto.

	while(z >= 0)
	{
		for(x=ori_x; x <= max_x; x+=c.w-1)
		{
			for(y=ori_y; y <= max_y; y+=c.d-1)
			{
				Bloque& bl=sala_actual->obtener_bloque_por_coordenadas_real(x, y, z);

				if(bl.es_colisionable() && bl.es_representable() && !bl.es_bloque_error())	//Un bloque no representable no recibe sombra.
				{
					unsigned int t=bl.calcular_z_encima(c);

					if(bl.es_recibe_sombra() && t > z_sombra && t <= ori_z) 
					{
						z_sombra=t;
					}
				}
			}
		}

		z-=Bloque::H_BLOQUE;
	}

	/*Ok, ahora con los actores... Es posible que estemos encima
	de un actor que esté más alto que el primer bloque de turno. 
	Haremos un cubo desde la altura real del jugador hasta la sombra
	calculada de momento y si hay actores dentro, trabajaremos un 
	poco más... El timo de esto es que si vamos a tener actores 
	apilados encima unos de otros puede haber errores porque la
	info de colisión devuelve sólo uno de ellos. Lo hacemos a mano*/

	Cubo cubo_sombra=jugador.como_cubo();
	cubo_sombra.origen.z=z_sombra;
	cubo_sombra.h=c.origen.z - z_sombra;

	std::vector<Actor *> actores=sala_actual->obtener_actores();
	for(Actor * a : actores)
	{
		Actor& otro=*a;
		if(otro.es_solido() && otro.en_colision_con(cubo_sombra))
		{	
			Cubo co=otro.como_cubo();
			unsigned int t=co.origen.z + co.h;
			if(otro.es_recibe_sombra() && t > z_sombra) z_sombra=t;
		}
	}

	sombra.establecer_visibilidad(z_sombra!=0);
	sombra.establecer_posicion(c.origen.x, c.origen.y, z_sombra);
}

/*Todas las colisiones que se han acumulado para el jugador en el turno
se trabajan ahora aquí... También se vacía el vector de colisiones con
efecto. Algunas colisiones hacen que entre en efecto el visitante de colisiones.

El vector que se pasa es el vector de movimiento que tenía el jugador antes de
empezar el turno. Está ahí para el caso de cambio de habitación, que no se
pierda la velocidad (la velocidad se pierde, por ejemplo, al chocar con los
bordes de la sala).
*/

void Controlador::procesar_colisiones_turno(float delta, const Vector_3d& v_original)
{
	if(actores_colisiones_con_efecto_en_turno.size() || bloques_colisiones_con_efecto_en_turno.size())
	{
		//Lo primero: una comprobación en varios ejes puede darnos
		//resultados repetidos, como al caer sobre un item: 
		//empezamos por purgar.

		std::sort(actores_colisiones_con_efecto_en_turno.begin(), actores_colisiones_con_efecto_en_turno.end());
		actores_colisiones_con_efecto_en_turno.erase(
			std::unique(
				actores_colisiones_con_efecto_en_turno.begin(), 
				actores_colisiones_con_efecto_en_turno.end()), 
				actores_colisiones_con_efecto_en_turno.end());

		//Creamos el visitante de colisión y hacemos 
		//que visite a todos los afectados. Cuando el
		//actor acepte el visitante hará lo que tenga que hacer, como
		//sumar llaves y borrarse.

		//En el proceso no comprobamos si hay colisión o no porque
		//es posible que ya se haya corregido. Si está ahí es porque
		//hubo colisión.

		Visitante_colision vc(jugador);
		
		for(Actor * a : actores_colisiones_con_efecto_en_turno) a->aceptar_visitante(vc);		
		for(Bloque * b : bloques_colisiones_con_efecto_en_turno) b->aceptar_visitante(vc);		

		bloques_colisiones_con_efecto_en_turno.clear();
		actores_colisiones_con_efecto_en_turno.clear();

		if(vc.es_perder_vida()) 
		{
			switch(vc.es_perder_vida())
			{
				case Visitante_colision::T_SIEMPRE: 
					perder_vida(); break;
				break;
				case Visitante_colision::T_LAVA: 
					if(!evaluar_resistencia(delta, Control_habilidades::H_RESISTENCIA_LAVA)) perder_vida();
					else sombra.establecer_estado(Sombra::E_LAVA);
				break;
				case Visitante_colision::T_AGUA: 
					if(!evaluar_resistencia(delta, Control_habilidades::H_RESISTENCIA_AGUA)) perder_vida();
					else sombra.establecer_estado(Sombra::E_AGUA);
				break;

				case Visitante_colision::T_DARDO:
					if(!evaluar_resistencia(delta, Control_habilidades::H_RESISTENCIA_DARDO)) perder_vida();
					else sonidos_activos.cambiar(Audio::S_PROTECCION_LAVA_AGUA, 100, true);
				break;
			}
		}

		//Aquí no hay else... Algunas entradas pueden estar bloqueadas por accesos letales
		//de modo que se procesaría la letalidad y si no te mata entraría aquí.

		if(vc.es_salida_sala()) 
		{
			//Nótese como quitamos de enmedio la Z.
			jugador.establecer_vector(v_original.x, v_original.y, 0.0);
			iniciar_salida_sala(vc.id_sala, vc.id_entrada);
		}
		else
		{
			if(!vc.escalera.es_nula()) vincular_escalera(vc.escalera);
			if(vc.llaves) recoger_llaves(vc.llaves);
			if(vc.bonus_tiempo) recoger_bonus_tiempo(vc.bonus_tiempo);
			if(vc.bonus) recoger_tesoro(vc.bonus);
			if(vc.tanques_energia) conceder_tanque_energia(vc.tanques_energia);
			if(vc.conceder_habilidad) conceder_habilidad(vc.conceder_habilidad);
			if(vc.amuleto) conceder_amuleto();
		}		
	}
}

/*Evalua la resistencia a los elementos que causan pérdida de vida al colisionar
con un bloque letal. Si hay resistencia efectúa el gasto de energía. En caso
contrario devuelve false, que causa la pérdida de una vida en el trozo de
código pertinente.*/

bool Controlador::evaluar_resistencia(float delta, unsigned int tipo)
{
	if(tipo==Control_habilidades::H_RESISTENCIA_LAVA ||
		tipo==Control_habilidades::H_RESISTENCIA_AGUA ||
		tipo==Control_habilidades::H_RESISTENCIA_DARDO)
	{
		auto obtener_consumo=[this](unsigned int tipo) -> float
		{
			switch(tipo)
			{
				case Control_habilidades::H_RESISTENCIA_LAVA: return Control_habilidades::CONSUMO_SEGUNDO_RESISTENCIA_LAVA; break;
				case Control_habilidades::H_RESISTENCIA_AGUA: return Control_habilidades::CONSUMO_SEGUNDO_RESISTENCIA_AGUA; break;
				case Control_habilidades::H_RESISTENCIA_DARDO: return Control_habilidades::CONSUMO_SEGUNDO_RESISTENCIA_DARDO; break;
				default: return 0.0; break;
			}
		};

		auto es_activa=[this](unsigned int tipo) -> bool
		{
			switch(tipo)
			{
				case Control_habilidades::H_RESISTENCIA_LAVA: return control_habilidades.es_resistencia_lava(); break;
				case Control_habilidades::H_RESISTENCIA_AGUA: return control_habilidades.es_resistencia_agua(); break;
				case Control_habilidades::H_RESISTENCIA_DARDO: return control_habilidades.es_resistencia_dardo(); break;
				default: return false; break;
			}
		};

		auto generar_color_destello=[this](unsigned int tipo, int &r, int &g, int &b)
		{
			switch(tipo)
			{
				case Control_habilidades::H_RESISTENCIA_LAVA: r=226; g=110; b=13; break;
				case Control_habilidades::H_RESISTENCIA_AGUA: r=20; g=120; b=220; break;
				case Control_habilidades::H_RESISTENCIA_DARDO: r=20; g=220; b=20; break;
			}
		};

		auto generar_sonido=[this](unsigned int tipo) -> int
		{
			switch(tipo)
			{
				case Control_habilidades::H_RESISTENCIA_LAVA: 
				case Control_habilidades::H_RESISTENCIA_AGUA: 
					return Audio::S_PROTECCION_LAVA_AGUA;
				break;
				case Control_habilidades::H_RESISTENCIA_DARDO: 	
				default:
					return 0; 
				break;
			}
		};

		float consumo=obtener_consumo(tipo);
		bool habilidad_activa=es_activa(tipo);

		//Comprobar si la habilidad está seleccionada y es usable...
		if(!habilidad_activa || !control_habilidades.es_habilidad_usable(tipo))
		{
			return false;
		}
		else
		{
			//Si ya hemos empezado a consumir un tanque...		
			if(control_energia.es_consumiendo())
			{
				return true;
			}
			else
			{	if(control_energia.hay_tanques_completos(1))
				{
					control_energia.iniciar_gasto(consumo);

					//Escoger color y sonido
					int r=0, g=0, b=0, sonido=0;
			
					generar_color_destello(tipo, r, g, b);
					sonido=generar_sonido(tipo);
					if(sonido) sonidos_activos.cambiar(sonido, 100, true);

					insertar_fundido_pantalla(r, g, b, 128, 0, 0.3);
					return true;
				}
				else
				{	
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}
}


void Controlador::trepar_jugador_escalera()
{
	Punto p=escalera_actual.obtener_posicion_trepar();
	jugador.establecer_posicion(p); 	
	jugador.establecer_bajar_escalera();
	escalera_actual.hacer_nula();
}

void Controlador::bajar_jugador_escalera()
{
	escalera_actual.hacer_nula();
	jugador.establecer_bajar_escalera();
	jugador.girar_180();
}

void Controlador::soltar_jugador_escalera()
{
	escalera_actual.hacer_nula();
	jugador.establecer_en_aire();
}

void Controlador::vincular_escalera(const Escalera& e)
{
	Cubo cj=jugador.como_cubo();
	escalera_actual=e;
	jugador.establecer_subir_escalera();
	jugador.establecer_posicion(e.obtener_snap(cj, Definiciones::obtener_posicion_opuesta(jugador.acc_direccion())));
}

void Controlador::intentar_descenso_escalera()
{
	//El cubo del jugador un poco más bajo...
	Cubo cj=jugador.obtener_cubo_descenso_escalera();
	unsigned int dir_actual=jugador.acc_direccion();
	unsigned int dir_opuesta=Definiciones::obtener_posicion_opuesta(dir_actual);

	std::vector<Escalera *> escaleras=sala_actual->obtener_escaleras();
	
	for(Escalera * i : escaleras)
	{
		if(i->en_colision_con(cj) && i->acc_orientacion() & dir_opuesta) 
		{
			Cubo c=cj;
			c.origen=i->obtener_snap(cj, dir_actual);
			if(cubo_en_posicion_libre(c))
			{
				jugador.girar_180();
				vincular_escalera(*i);
				break;
			}	
		}
	}
}

void Controlador::abrir_cofre(unsigned int id_pc)
{
	control_objetivos.contar(Control_objetivos::T_COFRE, 1);
	tiempo_juego.sumar_segundos(Info_juego::SEGUNDOS_POR_COFRE);
	mostrar_mensaje_info_juego(Diccionario_info_juego::D_COFRE);
	sonidos_activos.cambiar(Audio::S_COFRE, 100, true);
	control_puzzle.recoger_pieza(id_pc);
	info_juego.restar_llave();
	
	LOG<<"Recogida pieza puzzle "<<id_pc<<std::endl;

	//TODO: Esto debe ocurrir cuando terminas el juego con todos los cofres, no aquí.
	if(control_objetivos.obtener_total(Control_objetivos::T_COFRE)==
	control_objetivos.obtener_contados(Control_objetivos::T_COFRE))
	{
		Cofre::habilitar_overlays();
	}
}

void Controlador::conceder_amuleto()
{
	control_habilidades.activar_habilidades();

	mostrar_mensaje_info_juego(Diccionario_info_juego::D_AMULETO);

	sonidos_activos.cambiar(Audio::S_COFRE, 100, true);
}

void Controlador::conceder_tanque_energia(unsigned int c)
{
	unsigned int i=0;
	while(i < c)
	{
		control_energia.nuevo_tanque();
		++i;
	}
	
	mostrar_mensaje_info_juego(Diccionario_info_juego::D_TANQUE_ENERGIA);

	sonidos_activos.cambiar(Audio::S_COFRE, 100, true);
}

/*Traduce entre los valores de datos_colision para una habilidad
y las habilidades en si. Lo hacemos con switch case aunque si los valores 
siguen iguales bastaría con una operación aritmética.*/

void Controlador::conceder_habilidad(unsigned int c)
{
	Audio::pausar_canales_activos();

	int id_habilidad=-1;

	switch(c)
	{
		case Datos_colision::TC_HABILIDAD_SUPER_SALTO: 
			control_habilidades.conceder_habilidad(Control_habilidades::H_SUPER_SALTO); 
			id_habilidad=Diccionario_info_juego::D_HABILIDAD_SUPER_SALTO;
		break;

		case Datos_colision::TC_HABILIDAD_LAVA: 
			control_habilidades.conceder_habilidad(Control_habilidades::H_RESISTENCIA_LAVA); 
			id_habilidad=Diccionario_info_juego::D_HABILIDAD_LAVA;
		break;

		case Datos_colision::TC_HABILIDAD_AGUA: 
			control_habilidades.conceder_habilidad(Control_habilidades::H_RESISTENCIA_AGUA); 
			id_habilidad=Diccionario_info_juego::D_HABILIDAD_AGUA;
		break;

		case Datos_colision::TC_HABILIDAD_RESISTENCIA_DARDOS: 
			control_habilidades.conceder_habilidad(Control_habilidades::H_RESISTENCIA_DARDO); 
			id_habilidad=Diccionario_info_juego::D_HABILIDAD_DARDO;
		break;

		case Datos_colision::TC_HABILIDAD_TELETRANSPORTE: 
			control_habilidades.conceder_habilidad(Control_habilidades::H_TELETRANSPORTE); 
			id_habilidad=Diccionario_info_juego::D_HABILIDAD_TELETRANSPORTE;
		break;

		case Datos_colision::TC_HABILIDAD_IMPULSO: 
			control_habilidades.conceder_habilidad(Control_habilidades::H_IMPULSO); 
			id_habilidad=Diccionario_info_juego::D_HABILIDAD_IMPULSO;
		break;

		case Datos_colision::TC_HABILIDAD_PARAR_TIEMPO: 
			control_habilidades.conceder_habilidad(Control_habilidades::H_PARAR_TIEMPO); 
			id_habilidad=Diccionario_info_juego::D_HABILIDAD_TIEMPO;
		break;

		case Datos_colision::TC_HABILIDAD_INSPECTOR_SECRETOS: 
			control_habilidades.conceder_habilidad(Control_habilidades::H_INSPECTOR_SECRETOS); 
			id_habilidad=Diccionario_info_juego::D_HABILIDAD_SECRETOS;
		break;
	}	

	if(id_habilidad!=-1)
	{
		mostrar_mensaje_info_juego(id_habilidad);

		sonidos_activos.cambiar(Audio::S_COFRE, 100, true);
	}

}

void Controlador::recoger_llaves(unsigned int pc)
{
	info_juego.conceder_llaves(pc);

	if(!control_objetivos.es_mensaje_mostrado(Control_objetivos::M_LLAVE))
	{
		control_objetivos.marcar_mensaje_mostrado(Control_objetivos::M_LLAVE);
		mostrar_mensaje_info_juego(Diccionario_info_juego::D_LLAVE);
	}

	sonidos_activos.cambiar(Audio::S_ITEM, 100, true);
}

void Controlador::recoger_bonus_tiempo(unsigned int pc)
{
	info_juego.conceder_bonus_tiempo(pc);

	if(!control_objetivos.es_mensaje_mostrado(Control_objetivos::M_BONUS_TIEMPO))
	{
		control_objetivos.marcar_mensaje_mostrado(Control_objetivos::M_BONUS_TIEMPO);
		mostrar_mensaje_info_juego(Diccionario_info_juego::D_BONUS_TIEMPO);
	}

	sonidos_activos.cambiar(Audio::S_ITEM, 100, true);
}

void Controlador::recoger_tesoro(unsigned int pc)
{
	unsigned int i=0;

	while(i < pc)
	{
		control_objetivos.contar(Control_objetivos::T_TESORO, 1);
		++i;
		tiempo_juego.sumar_segundos(Info_juego::SEGUNDOS_POR_TESORO);
		sala_actual->recoger_gema();
	}

	if(!control_objetivos.es_mensaje_mostrado(Control_objetivos::M_TESORO))
	{
		control_objetivos.marcar_mensaje_mostrado(Control_objetivos::M_TESORO);
		mostrar_mensaje_info_juego(Diccionario_info_juego::D_TESORO);
	}

	sonidos_activos.cambiar(Audio::S_ITEM, 100, true);

	actualizar_estado_gemas_sala_actual();
}

void Controlador::redimir_bonus_tiempo()
{
	unsigned int bonus=info_juego.acc_bonus_tiempo();

	if(bonus)
	{
		info_juego.vaciar_bonus_tiempo();
		tiempo_juego.sumar_segundos(bonus * Info_juego::SEGUNDOS_POR_TIEMPO);
		mostrar_mensaje_info_juego(Diccionario_info_juego::D_BONUS_TIEMPO_USADO);
		sonidos_activos.cambiar(Audio::S_COFRE, 100, true);
	}
	else
	{
		mostrar_mensaje_info_juego(Diccionario_info_juego::D_SIN_BONUS_TIEMPO);
	}
}

/*Ejecuta la cadena de acciones que se pasa como parámetro y es el único punto
de entrada para esta acción. Si la cadena de acciones es de la sala actual
se comprobará que al final el jugador está en posición válida. En caso contrario
se contará como una vida perdida.*/

void Controlador::ejecutar_cadena_acciones(unsigned int pid_eslabon/*, bool encendido*/)
{
	if(controlador_cadenas_acciones.existe_entrada_por_id(pid_eslabon))
	{
		LOG<<"Ejecutando cadena acciones "<<pid_eslabon<<std::endl;

		//Obtener el eslabon...
		unsigned int id_eslabon=pid_eslabon;
		unsigned int id_sala=sala_actual->acc_id();
		bool comprobar_posicion_final=false;

		while(id_eslabon)
		{
			Cadena_acciones e=controlador_cadenas_acciones.obtener_por_id(id_eslabon);
			unsigned int id_dato=e.id_dato_persistente;

			LOG<<"Eslabon "<<id_eslabon<<" :: id dato "<<id_dato<<std::endl;

//hola Nete!!
			if(controlador_datos_persistentes.existe_entrada_por_id(id_dato))
			{
				/*unsigned int nuevo_valor=encendido ? e.valor_encendido : e.valor_apagado;*/
				controlador_datos_persistentes.actualizar_valor(id_dato, e.valor/*nuevo_valor*/);
				Dato_persistente dp=controlador_datos_persistentes.obtener_por_id(id_dato);

				//Aplicar si es de sala...
				if(dp.id_sala==id_sala) 
				{
					LOG<<"Dato persistente es de sala"<<std::endl;
					sala_actual->recibir_bloque_datos_persistentes(dp);
					comprobar_posicion_final=true;
				}
				else if(dp.es_global())
				{
					LOG<<"Dato persistente Es global"<<std::endl;
					if(dp.es_inicio_temporizador())	
					{
						LOG<<"Es inicio temporizador"<<std::endl;
						iniciar_temporizador(dp/*, encendido*/);
					}
				}
			}
			else LOG<<"No existe entrada datos persistentes "<<id_dato<<std::endl;

			id_eslabon=e.siguiente_eslabon;
		}

		LOG<<"Finalizando cadena"<<std::endl;

		if(comprobar_posicion_final)
		{
			Cubo cubo_j=jugador.como_cubo();

			if(!cubo_en_posicion_libre(cubo_j))
			{
				LOG<<"Perder vida por posición ocupada tras cadena de acciones."<<std::endl;
				perder_vida();
			}
		}
	}
	else
	{
		LOG<<"No existe el eslabón "<<pid_eslabon<<" para iniciar la cadena"<<std::endl;
	}
}

/*El id de elemento será el id de temporizador a iniciar... Se crea el 
temporizador y se mete en el vector. En el siguiente turno hará su
movimiento.*/

void Controlador::iniciar_temporizador(const Dato_persistente& dp/*, bool encendido*/)
{
	if(controlador_datos_temporizador.existe_entrada_por_id(dp.id_elemento))
	{
		Datos_temporizador dt=controlador_datos_temporizador.obtener_por_id(dp.id_elemento);
		controles_temporizador.push_back(Control_temporizador(dt/*, encendido*/));
	}
	else LOG<<"No se encuentra temporizador "<<dp.id_elemento<<std::endl;
}

/*Carga una nueva sala de disco e la reinicia. Al reiniciarla se coloca al 
jugador en la entrada indicada. Si el proceso tiene éxito la sala actual 
anterior se destruye.*/

bool Controlador::iniciar_salida_sala(unsigned int id_sala, unsigned int id_entrada)
{		
	//Comprobar existencia de la sala...
	Sala * s=obtener_sala_por_id(id_sala);

	if(!s) LOG<<"ERROR: La sala "<<id_sala<<" no existe"<<std::endl;
	else
	{
		Entrada_sala e=s->obtener_entrada_sala_por_id(id_entrada);	

		if(!e.es_valida()) 
		{
			LOG<<"ERROR: La entrada "<<id_entrada<<" de la sala "<<id_sala<<" no existe: borrando sala"<<std::endl;
			delete s;
		}
		else
		{
			controles_temporizador.clear();

			//Visitante de salida de sala...
			if(sala_actual) sala_actual->iniciar_salida();

			//Cargar sala nueva.
			iniciar_sala(*s);

			//Colocar al jugador en la salida.
			entrada_actual=e;
			jugador.establecer_posicion(entrada_actual);
			jugador.establecer_en_tierra();

			//Control de estados de la sala.
			control_salas.descubrir_sala(id_sala);
			actualizar_estado_gemas_sala_actual();
			recuento_secretos_sala();

			return true;
		}
	}	

	return false;				
}	

/*Devuelve la sala con el id con los datos del archivo, sin datos persistentes.
Esto no manipula la sala actual. La sala cargada incluye sólo los bloques y 
actores estáticos. Como parte de preparar una sala se imbuirán los actores 
persistentes. Ninguno de ellos sería propiedad de la sala.*/

Sala * Controlador::obtener_sala_por_id(unsigned int pid)
{
	Sala * resultado=new Sala(0,0,0,0, false);

	if(!cargador_datos.cargar_sala_desde_id(*resultado, pid))
	{
		delete resultado;
		resultado=NULL;
	}

	return resultado;
}

/*Volvería a cargar la información de la sala desde el disco...*/

void Controlador::recargar_sala_actual()
{
	unsigned int id_entrada=entrada_actual.acc_id_entrada();
	unsigned int id_sala=sala_actual->acc_id();	
//	if(sala_actual) sala_actual->iniciar_salida();
	iniciar_salida_sala(id_sala, id_entrada);
}

/*Vuelve a cargar toda la información e intenta iniciar de nuevo la que era
sala actual, que para entonces se habrá destruido.*/

void Controlador::recargar_mundo()
{
	unsigned int id_sala=sala_actual->acc_id();
	unsigned int id_entrada=entrada_actual.acc_id_entrada();

	if(sala_actual) 
	{
		sala_actual->iniciar_salida();
		delete sala_actual;
		sala_actual=NULL;
	}

	inicializar_informacion();
	iniciar_juego(id_sala, id_entrada);
}

//Cuando te quitan una vida...

void Controlador::perder_vida()
{
	if(modo_dios) return;
	info_juego.perder_vida();	

	sonidos_activos.cambiar(Audio::S_PERDER_VIDA, 100, true);

	//Desactivar los sonidos de caida larga.
	sonidos_activos.cambiar(Audio::S_CAIDA, 100, false);
	sonidos_activos.cambiar(Audio::S_CAIDA_LARGA, 100, false);

	jugador.establecer_perdiendo_vida();

}

void Controlador::finalizar_perder_vida()
{
	if(control_habilidades.es_parando_tiempo())finalizar_habilidad_parar_tiempo(false);
	tiempo_juego.sumar_segundos(Info_juego::SEGUNDOS_POR_VIDA);
	control_habilidades.desactivar_todo();
	control_energia.rellenar();
	
	recargar_sala_actual();
	jugador.establecer_en_tierra();
}

void Controlador::finalizar_habilidad_parar_tiempo(bool con_sonido)
{
	Audio::despausar_canales_activos();	
	if(con_sonido)
	{
		sonidos_activos.cambiar(Audio::S_REANUDAR_TIEMPO, 100, true);
	}	
}

/*Destruye la sala actual y establece la que se pasa como nueva sala actual.
Como parte del proceso, se actualiza la información de mostrado estático y
se imbuye la información persistente en la nueva sala.
Es la puerta de acceso a iniciar cualquier sala.*/

void Controlador::iniciar_sala(Sala& nueva)
{
	//Destruir la actual si existe.
	if(sala_actual && sala_actual!=&nueva) 
	{
		delete sala_actual;
		sala_actual=NULL;
	}

	sala_actual=&nueva;
	preparar_vectores_representacion_estaticos(*sala_actual);

	try
	{
		std::vector<Actor *>& actores_p=control_actores_persistentes.obtener_actores_persistentes_por_id(sala_actual->acc_id());
		for(Actor * a : actores_p) sala_actual->insertar_actor(a, true);
	}
	catch(std::out_of_range& e) {} //Esto no tiene mayor significación: la sala no tiene actores persistentes.

	sala_actual->unificar_actores();
	sala_actual->actualizar_datos_persistentes(controlador_datos_persistentes);

	if(sala_actual->es_camara_estatica()) calcular_offsets_para_sala(*sala_actual);

	txt_nombre_sala=loc_nombres_salas.obtener(sala_actual->acc_id());
}

void Controlador::iniciar_juego(unsigned int id_sala, unsigned int id_salida)
{
	sala_actual=NULL;
	cargar_informacion_juego();
	iniciar_salida_sala(id_sala, id_salida);
}

/*Carga información de actores y bloques... Todo esto debe existir antes de
intentar cargar o precargar las salas, que lo usarán.
*/

void Controlador::cargar_informacion_general()
{
	Bloque::asignar_datos_representacion(cargador_datos.generar_datos_representacion_bloques());
	Controlador_patrones_disparo::asignar_datos_patrones_disparo(cargador_datos.generar_datos_patrones_disparo());	
	Actor_generico::asignar_datos_representacion(cargador_datos.generar_datos_actores_genericos());
	Proyectil_obstaculo::asignar_datos_proyectiles(cargador_datos.generar_datos_proyectiles_genericos());
	Control_pieza_puzzle::preparar_datos();
	cargador_datos.localizar_archivos_salas();

	loc_nombres_salas=Localizador("data/localizacion/salas");
	loc_nombres_salas.inicializar(0);
}

/*Carga la información para una nueva sesión de juego... Para cargar una 
partida el proceso podría ser igual, pero seguido de sobreescribir la 
información actualizada. */

void Controlador::cargar_informacion_juego()
{
	//Reiniciar controles de información.
	tiempo_juego.establecer_segundos_restantes(Info_juego::SEGUNDOS_INICIO);
	control_salas.reiniciar();
	control_objetivos.reiniciar();
	control_puzzle.reiniciar(); //El Control_pieza_puzzle debe estar inicializado.
	control_energia.reiniciar();
	control_habilidades.reiniciar();
	control_actores_persistentes.reiniciar();

	//Importar información del cargador.
	controlador_datos_persistentes=cargador_datos.generar_datos_persistentes();
	controlador_cadenas_acciones=cargador_datos.generar_cadenas_acciones();
	controlador_datos_temporizador=cargador_datos.generar_datos_temporizador();

	//Cargar información necesaria...
	cargador_datos.precargar_actores_persistentes(control_actores_persistentes);
	control_actores_persistentes.efectuar_recuento_items(control_objetivos);
	cargador_datos.cargar_control_salas(control_salas);
}

/*Cuando se usa un punto de guardado llegamos a este punto, evaluaremos
el resultado del guardado y mostraremos un info juego al respecto.*/

void Controlador::iniciar_proceso_guardado(unsigned int id_entrada)
{
	unsigned int id_mensaje=guardar_sesion_juego(id_entrada) ?
		Diccionario_info_juego::D_GUARDADO_SESION_OK :
		Diccionario_info_juego::D_GUARDADO_SESION_ERROR;

	mostrar_mensaje_info_juego(id_mensaje);
}

/*Para guardar una sesión de juego. El orden de llamada de los métodos del
serializador es absolutamente determinante!.

El parámetro se refiere al id de entrada que se va a guardar.
*/

bool Controlador::guardar_sesion_juego(unsigned int id_entrada)
{
	unsigned int id_sesion=datos_sistema.acc_id_sesion_guardado();
	Serializador::realizar_copia_seguridad(id_sesion);

	Serializador s(id_sesion);

	if(!s.es_valido()) 
	{
		return false;
	}
	else
	{
		unsigned int id_sala=sala_actual->acc_id();
		std::string nombre_sala=loc_nombres_salas.obtener(id_sala);

		s.iniciar(id_sala, id_entrada, nombre_sala);
		s.serializar(info_juego);
		s.serializar(tiempo_juego);
		s.serializar(control_salas);
		s.serializar(control_objetivos);
		s.serializar(control_puzzle);
		s.serializar(control_energia);
		s.serializar(control_habilidades);
		s.serializar(controlador_datos_persistentes);
		s.serializar(control_actores_persistentes); 
		s.finalizar();
		
		return true;
	}
}

bool Controlador::recuperar_sesion_juego()
{
	//Reiniciar controles de funcionamiento...
	sala_actual=NULL;
	sonidos_activos.reiniciar();
	controles_temporizador.clear();
	escalera_actual=Escalera(0,0,0,0,0,0,0,0);
	actor_puzzle_actual=NULL;
	jugador.establecer_en_tierra();

	//Reiniciar controles de información.
	control_salas.reiniciar();
	control_objetivos.reiniciar();
	control_puzzle.reiniciar();
	control_energia.reiniciar();
	control_habilidades.reiniciar();
	control_actores_persistentes.reiniciar();

	//Importar información del cargador.
	controlador_datos_persistentes=cargador_datos.generar_datos_persistentes();
	controlador_cadenas_acciones=cargador_datos.generar_cadenas_acciones();
	controlador_datos_temporizador=cargador_datos.generar_datos_temporizador();
	cargador_datos.cargar_control_salas(control_salas);

	Deserializador d(datos_sistema.acc_id_sesion_guardado());

	if(!d.es_valido())
	{
		LOG<<"ERROR: El deserializador no tiene archivo abierto"<<std::endl;
		return false;
	}
	else
	{
		d.iniciar();
		d.deserializar(info_juego);
		d.deserializar(tiempo_juego);
		d.deserializar(control_salas);
		d.deserializar(control_objetivos);
		d.deserializar(control_puzzle);
		d.deserializar(control_energia);
		d.deserializar(control_habilidades);
		d.deserializar(controlador_datos_persistentes);
		d.deserializar(control_actores_persistentes);
		d.finalizar();

		if(d.es_error())
		{
			return false;
		}
		else
		{
			unsigned int id_sala=d.acc_id_sala();
			unsigned int id_entrada=d.acc_id_entrada();

			iniciar_salida_sala(id_sala, id_entrada);
			return true;
		}
	}
}

bool Controlador::inicializar_informacion()
{
	if(!cargador_datos.inicializar())
	{
		LOG<<"ERROR: No se ha inicializado el cargador de datos"<<std::endl;	
		return false;
	}
	else
	{
		cargar_informacion_general();
		return true;
	}
}

Controlador::Controlador(
	Datos_sistema& ds,
	DLibV::Pantalla& pp, 
	/*const*/ Input& i,
	int w, int h):
	datos_sistema(ds),
	pantalla(pp), 
	input(i),
	W_PANTALLA(w), H_PANTALLA(h),
	camara(0, 0, W_PANTALLA, H_PANTALLA),
	caja_margen_camara(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_MARGEN_SCROLL, Y_CAJA_MARGEN_SCROLL, W_CAJA_MARGEN_SCROLL, H_CAJA_MARGEN_SCROLL)),
	jugador(16,16,16),	//Una posición por defecto.
	sala_actual(NULL),
	entrada_actual(0,0,0,0),
	escalera_actual(0,0,0,0,0,0,0,0),
	actor_puzzle_actual(NULL),
	sala_actual_con_secreto(false),
	estado(E_JUEGO),
	pausa(false), modo_paso(false), modo_dios(false),
	dibujar_planos(DP_FONDO | DP_MEDIO | DP_FRENTE), offset_x(0.0), offset_y(0.0)
{

}

Controlador::~Controlador()
{}

void Controlador::iniciar_comprobador_rampas()
{
	Comprobador_rampas c;
	if(!c.comprobar())
	{
		std::cout<<"Error: imposible obtener fichero verificacion de rampas"<<std::endl;
	}
	else
	{
		Sala * s=NULL;
		unsigned int i=1;

		do
		{
			s=obtener_sala_por_id(i);
			if(!s) break;
			else
			{
				c.procesar_sala(*s);
				delete s;
				s=NULL;
				++i;
			}
		}
		while(true);

		std::cout<<"Proceso finalizado"<<std::endl;
	}
}

/*Inicia el proceso de comprobar la sintaxis de todos los bloques y actores
de la sala. Inicialmente este proceso se llevaba a cabo al inicializar, pero 
hemos preferido separarlo para no consumir tanto al arrancar.*/

void Controlador::iniciar_comprobador_sintaxis()
{
	if(!cargador_datos.inicializar())
	{
		LOG<<"ERROR: No se ha inicializado el cargador de datos"<<std::endl;	
	}
	else
	{
		cargador_datos.ejecutar_comprobacion_sintaxis_salas();
	}
}


void Controlador::actualizar_estado_gemas_sala_actual()
{
	//Marcamos si quedan o no tesoros en la sala.	
	unsigned int id_sala=sala_actual->acc_id();
	unsigned int cuenta_gemas=sala_actual->acc_cuenta_gemas();
	control_salas.marcar_gemas(id_sala, cuenta_gemas);
}
