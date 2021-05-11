#include "sistema.h"

Sistema::Sistema(int argc, char ** argv)
	:CARG(argc, argv), control_frames(60)
{
	configuracion.cargar();
}

void Sistema::inicializar_entorno_grafico(DLibV::Pantalla& pantalla, unsigned int w, unsigned int h)
{
	configurar_entorno_grafico(pantalla, w, h);
	pantalla.establecer_titulo("--== THE SEER ==--");

	DLibV::Utilidades_graficas_SDL::mostrar_ocultar_cursor(false);
	DLibV::Gestor_color::establecer_formato(pantalla.obtener_formato_pixeles());
}

void Sistema::configurar_entorno_grafico(DLibV::Pantalla& pantalla, unsigned int w, unsigned int h)
{
	unsigned int flags_video=
		(configuracion.acc_modo_hardware() ? SDL_HWSURFACE : SDL_SWSURFACE) |
		(configuracion.acc_pantalla_completa() ? SDL_FULLSCREEN : 0) |
		(configuracion.acc_pantalla_doble_buffer() ? SDL_DOUBLEBUF : 0) |
		(configuracion.acc_pantalla_anyformat() ? SDL_ANYFORMAT : 0);

	pantalla.inicializar(w, h, 0, flags_video);
}

void Sistema::inicializar_entorno_audio()
{
	Audio::inicializar_entorno_audio(
		configuracion.acc_audio_ratio(),
		configuracion.acc_audio_salidas(),
		configuracion.acc_audio_buffers(),
		configuracion.acc_audio_canales());

	configurar_entorno_audio();
}

void Sistema::configurar_entorno_audio()
{
	Audio::establecer_volumen(configuracion.acc_volumen_audio());
	Audio::establecer_volumen_musica(configuracion.acc_volumen_musica());
}

void Sistema::iniciar()
{
	datos_sistema.id_idioma=configuracion.acc_idioma();

	inicializar_entorno_grafico(pantalla, W_PANTALLA, H_PANTALLA);
	inicializar_entorno_audio();

	cargador_recursos.generar_recursos_graficos();
	cargador_recursos.generar_recursos_audio();

	//Inicializar controles.
	input.configurar();

	//Inicialización controlador tiempo.
	control_frames.inicializar();

	iniciar_loop_sistema();

	LOG<<"T.Total: "<<tiempo_sistema.acc_t(Tiempo_sistema::E_SISTEMA)<<std::endl;
	LOG<<"T.Juego: "<<tiempo_sistema.acc_t(Tiempo_sistema::E_JUEGO)<<std::endl;
	LOG<<"T.Mapa: "<<tiempo_sistema.acc_t(Tiempo_sistema::E_MAPA)<<std::endl;
	LOG<<"T.Diapositivas: "<<tiempo_sistema.acc_t(Tiempo_sistema::E_DIAPOSITIVAS)<<std::endl;
	LOG<<"T.Puzzle: "<<tiempo_sistema.acc_t(Tiempo_sistema::E_PUZZLE)<<std::endl;
	LOG<<"T.Intro: "<<tiempo_sistema.acc_t(Tiempo_sistema::E_INTRO)<<std::endl;
	LOG<<"T.Otros: "<<tiempo_sistema.acc_t(Tiempo_sistema::E_OTROS)<<std::endl;
}

void Sistema::procesar_reconfiguracion()
{
	if(datos_sistema.reconfigurar & Datos_sistema::C_VIDEO) configurar_entorno_grafico(pantalla, W_PANTALLA, H_PANTALLA);
	if(datos_sistema.reconfigurar & Datos_sistema::C_AUDIO) configurar_entorno_audio();
	if(datos_sistema.reconfigurar & Datos_sistema::C_IDIOMA) datos_sistema.id_idioma=configuracion.acc_idioma();

	datos_sistema.reconfigurar=Datos_sistema::C_NADA;
}

Sistema::Info_linea_comandos Sistema::comprobar_argumentos_linea_comandos()
{
	Info_linea_comandos resultado;

	int pos_rampa=CARG.buscar("-crampas");
	int pos_sintaxis=CARG.buscar("-csintaxis");
	int pos_sala=CARG.buscar("-sala");
	int pos_entrada=CARG.buscar("-entrada");

	if(pos_rampa != -1) resultado.estado_inicial=Info_linea_comandos::E_COMPROBADOR_RAMPAS;
	else if(pos_sintaxis != -1) resultado.estado_inicial=Info_linea_comandos::E_COMPROBADOR_SINTAXIS;
	else if(pos_entrada!=-1 || pos_sala!=-1)
	{
		resultado.estado_inicial=Info_linea_comandos::E_JUEGO;
		if(pos_sala != -1) resultado.id_sala=DLibH::Herramientas::cadena_a_entero(CARG.acc_argumento(pos_sala+1));
		if(pos_entrada != -1) resultado.id_entrada=DLibH::Herramientas::cadena_a_entero(CARG.acc_argumento(pos_entrada+1));
	}

	return resultado;
}

/*Este es el proceso de arrancar todos los controladores relacionados con el
estado de juego y el loop en si.*/

void Sistema::iniciar_loop_sistema()
{
	Info_linea_comandos ilc=comprobar_argumentos_linea_comandos();
	Pase_diapositivas pase(0);
	Pantallazo_sistema ps;

	//Controlador de juego: necesario
	Controlador c(datos_sistema, pantalla, input, W_PANTALLA, H_PANTALLA);
	if(!c.inicializar_informacion())
	{
		LOG<<"ERROR: El controlador de juego ha fallado al inicializar información"<<std::endl;
		return;
	}

	Controlador_intro ci(datos_sistema, control_sesiones_guardadas, configuracion);

	//Escoger estado inicial...
	switch(ilc.estado_inicial)
	{
		case Info_linea_comandos::E_COMPROBADOR_RAMPAS:
			c.iniciar_comprobador_rampas();
			return;
		break;
		case Info_linea_comandos::E_COMPROBADOR_SINTAXIS:
			c.iniciar_comprobador_sintaxis();
			return;
		break;
		case Info_linea_comandos::E_JUEGO:
			c.iniciar_juego(ilc.id_sala, ilc.id_entrada);
			datos_sistema.establecer_en_juego();
			ci.habilitar_volver_a_juego();
		break;
		case Info_linea_comandos::E_MENU:
			datos_sistema.establecer_en_intro();
		break;
	}

	//Resto de controles.
	Controlador_diapositivas cd(datos_sistema, pantalla, input, W_PANTALLA, H_PANTALLA);
	Controlador_info_juego cij(datos_sistema, input);
	Controlador_confirmacion cc(datos_sistema, input);
	Controlador_puzzle cp(datos_sistema, c.acc_tiempo_juego());
	Controlador_auxiliar cm(datos_sistema, pantalla,
					input,
					c.acc_control_salas(),
					c.acc_control_habilidades(),
					c.acc_info_juego(),
					c.acc_control_objetivos(),
					W_PANTALLA, H_PANTALLA);

	float delta=0;
	do
	{
		delta=control_frames.obtener_delta();
		if(delta > Definiciones::MAX_DELTA) delta=Definiciones::MAX_DELTA;

		tiempo_sistema.t(delta, Tiempo_sistema::E_SISTEMA);

		input.turno();

		if(datos_sistema.reconfigurar)
		{
			procesar_reconfiguracion();
		}

		//Un loop de cualquier subsistema NO se queda dentro
		//del loop iterando sin parar, sino que entra y sale,
		//devolviendo el control al sistema principal.
		//A cada tic del juego, es el sistema el que controla
		//el tiempo y a quién da el control.

		switch(datos_sistema.estado)
		{
			case Datos_sistema::E_PREPARANDO_MENU: preparar_menu(ci); break;
			case Datos_sistema::E_PREPARANDO_MENU_INTRO: preparar_menu_intro(ci); break;
			case Datos_sistema::E_MENU: procesar_estado_menu(delta, ci); break;
			case Datos_sistema::E_INICIAR_JUEGO: preparar_iniciar_juego(ci, c); break;
			case Datos_sistema::E_JUEGO: procesar_estado_juego(delta, c); break;
			case Datos_sistema::E_PREPARAR_DIAPOSITIVA: preparar_diapositiva(cd, pase); break;
			case Datos_sistema::E_DIAPOSITIVA: procesar_estado_diapositivas(delta, cd, pase); break;
			case Datos_sistema::E_MAPA: procesar_estado_mapa(delta, cm, c); break;
			case Datos_sistema::E_PUZZLE: procesar_estado_puzzle(delta, cp, c); break;
			case Datos_sistema::E_CONFIRMACION_SALIDA: procesar_confirmacion_salida(delta, cc, ps, ci); break;
			case Datos_sistema::E_INFO_JUEGO: procesar_estado_info_juego(delta, cij, ps); break;
			case Datos_sistema::E_ABORTANDO_EJECUCION:
			case Datos_sistema::E_ABANDONANDO_SISTEMA:
				return;
			break;
		}

		control_frames.turno();

		std::string fps="FPS:"+DLibH::Herramientas::entero_a_cadena(control_frames.acc_frames_contados());
		DLibV::Representacion_texto_fija_estatica txt(DLibV::Gestor_recursos_graficos::obtener(3), fps, 100, 20);
		txt.establecer_posicion(500, 6);
		txt.volcar(pantalla);
		pantalla.flipar();
		Audio::procesar_cola_sonido();

	}while(true);
}

void Sistema::preparar_iniciar_juego(Controlador_intro& ci, Controlador& c)
{
	datos_sistema.establecer_id_sesion_guardado(ci.acc_id_sesion_guardado());

	if(control_sesiones_guardadas.es_sesion_usada(datos_sistema.acc_id_sesion_guardado()))
	{
		if(!c.recuperar_sesion_juego()) datos_sistema.abortar_ejecucion();
	}
	else
	{
		c.iniciar_juego(1, 1);
	}

	datos_sistema.establecer_en_juego();
}

//Esto es "abrir el menu"
void Sistema::preparar_menu(Controlador_intro& ci)
{
	ci.restaurar_estado_menu();
	datos_sistema.establecer_en_menu_real();
	Audio::pausar(); //Pausamos todo.
}

//Esto es "iniciar el menu desde la intro".
void Sistema::preparar_menu_intro(Controlador_intro& ci)
{
	ci.establecer_en_intro();
	datos_sistema.establecer_en_menu_real();
}

void Sistema::procesar_estado_mapa(float delta, Controlador_auxiliar& cm, Controlador& c)
{
	tiempo_sistema.t(delta, Tiempo_sistema::E_MAPA);

	if(!cm.es_inicializado())
	{
		cm.inicializar(
			Tiempo_juego::como_cadena(c.acc_segundos_restantes()),
			c.obtener_id_sala_actual(),
			c.obtener_piezas_recogidas());
		Audio::pausar(); //Pausamos todo.
	}

	if(!cm.loop(delta))
	{
		cm.desmontar();
		datos_sistema.establecer_en_juego();
		Audio::despausar_excepto_activos(); //Despausamos excepto los que estuvieran pausados por parar el tiempo.
	}
}

void Sistema::procesar_estado_puzzle(float delta, Controlador_puzzle& cp, Controlador& c)
{
	tiempo_sistema.t(delta, Tiempo_sistema::E_PUZZLE);

	if(!cp.es_montado())
	{
		cp.preparar(datos_sistema.id_puzzle, c.obtener_piezas_recogidas());
	}

	if(!cp.loop(delta, pantalla, input))
	{
		c.solucionar_puzzle(cp.es_finalizado());
		cp.desmontar();
		datos_sistema.establecer_en_juego();
		Audio::despausar_canales_activos(); //Despausamos excepto los que estuvieran pausados por parar el tiempo.
	}
}


void Sistema::procesar_estado_juego(float delta, Controlador &c)
{
	tiempo_sistema.t(delta, Tiempo_sistema::E_JUEGO);
	c.loop(delta);
}

void Sistema::procesar_estado_diapositivas(float delta, Controlador_diapositivas& cd, Pase_diapositivas& pase)
{
	tiempo_sistema.t(delta, Tiempo_sistema::E_DIAPOSITIVAS);
	if(!cd.loop(delta, pase))
	{
		datos_sistema.establecer_en_juego();
	}
}

void Sistema::preparar_diapositiva(Controlador_diapositivas& cd, Pase_diapositivas& pase)
{
	try
	{
		pase=cd.obtener_y_preparar_pase(datos_sistema.id_diapositiva);
		datos_sistema.diapositiva_preparada();
		cd.reinicializar(pase);
	}
	catch(Excepcion_diapositivas& e)
	{
		LOG<<"ERROR: No se ha localizado la diapositiva "<<datos_sistema.id_diapositiva<<std::endl;
		datos_sistema.abortar_ejecucion();
	}
}

void Sistema::procesar_confirmacion_salida(float delta, Controlador_confirmacion& cc, Pantallazo_sistema& ps, Controlador_intro& ci)
{
	tiempo_sistema.t(delta, Tiempo_sistema::E_OTROS);

	if(!cc.es_montado())
	{
		Audio::pausar();
		cc.preparar();
		ps.preparar(pantalla);
	}

	ps.dibujar(pantalla);

	if(!cc.loop(delta, pantalla))
	{
		if(cc.es_abandonar_juego())
		{
			datos_sistema.abandonar_sistema();
		}
		else
		{
			datos_sistema.establecer_en_menu(false);
//			ci.establecer_en_intro();
//			Audio::despausar_excepto_activos();
		}

		cc.desmontar();
		ps.desmontar();
	}
}

void Sistema::procesar_estado_info_juego(float delta, Controlador_info_juego& cij, Pantallazo_sistema& ps)
{
	tiempo_sistema.t(delta, Tiempo_sistema::E_OTROS);

	if(!cij.es_montado())
	{
		cij.preparar(datos_sistema.id_info_juego);
		ps.preparar(pantalla);
	}

	ps.dibujar(pantalla);

	if(!cij.loop(delta, pantalla))
	{
		ps.desmontar();
		cij.desmontar();
		datos_sistema.establecer_en_juego();
		Audio::despausar_canales_activos(); //Despausamos excepto los que estuvieran pausados por parar el tiempo.
	}
}

void Sistema::procesar_estado_menu(float delta, Controlador_intro& ci)
{
	tiempo_sistema.t(delta, Tiempo_sistema::E_INTRO);
	ci.loop(delta, pantalla, input);

	if(ci.es_salir_a_sistema())
	{
		datos_sistema.confirmar_salida();
	}
	else if(ci.es_iniciar_juego())
	{
		ci.habilitar_volver_a_juego();
		datos_sistema.iniciar_juego();
	}
	else if(ci.es_volver_a_juego())
	{
		Audio::despausar_excepto_activos(); //Despausamos excepto los que estuvieran pausados por parar el tiempo.
		datos_sistema.restaurar_juego();
	}
}
