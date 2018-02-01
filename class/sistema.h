#ifndef SISTEMA_H
#define SISTEMA_H

#include "configuracion.h"
#include "input.h"
#include "controlador_intro.h"
#include "controlador.h"
#include "controlador_diapositivas.h"
#include "controlador_auxiliar.h"
#include "controlador_info_juego.h"
#include "controlador_confirmacion.h"
#include "controlador_puzzle.h"
#include "cargador_recursos.h"
#include "pantallazo_sistema.h"
#include "datos_sistema.h"
#include "tiempo_sistema.h"
#include "control_sesiones_guardadas.h"

extern DLibH::Log_base LOG;

class Sistema
{
	private:

	struct Info_linea_comandos
	{
		enum {E_MENU, E_JUEGO, E_COMPROBADOR_RAMPAS, E_COMPROBADOR_SINTAXIS};

		unsigned int id_sala;
		unsigned int id_entrada;
		unsigned int estado_inicial;

		Info_linea_comandos():id_sala(1), id_entrada(1), estado_inicial(E_MENU) {}
	};

	static const int W_PANTALLA=600;
	static const int H_PANTALLA=400;

	Datos_sistema datos_sistema;
	DLibH::Controlador_argumentos CARG;
	Configuracion configuracion;
	DLibV::Pantalla pantalla;
	DLibH::Controlador_fps_SDL control_frames;
	Input input;
	Tiempo_sistema tiempo_sistema;
	Control_sesiones_guardadas control_sesiones_guardadas;

	Cargador_recursos cargador_recursos;

	void inicializar_entorno_grafico(DLibV::Pantalla& pantalla, unsigned int w, unsigned int h);
	void configurar_entorno_grafico(DLibV::Pantalla& pantalla, unsigned int w, unsigned int h);

	void inicializar_entorno_audio();
	void configurar_entorno_audio();

	Info_linea_comandos comprobar_argumentos_linea_comandos();
	
	void procesar_reconfiguracion();

	void procesar_estado_menu(float, Controlador_intro&);
	void procesar_estado_mapa(float, Controlador_auxiliar&, Controlador&);
	void procesar_estado_puzzle(float, Controlador_puzzle&, Controlador&);
	void procesar_estado_juego(float, Controlador&);
	void procesar_estado_diapositivas(float, Controlador_diapositivas&, Pase_diapositivas&);
	void procesar_confirmacion_salida(float, Controlador_confirmacion&, Pantallazo_sistema&, Controlador_intro&);
	void procesar_estado_info_juego(float, Controlador_info_juego&, Pantallazo_sistema&);

	void preparar_menu(Controlador_intro&);
	void preparar_menu_intro(Controlador_intro&);
	void preparar_diapositiva(Controlador_diapositivas&, Pase_diapositivas&);
	void preparar_iniciar_juego(Controlador_intro&, Controlador&);

	void iniciar_loop_sistema();

	public:

	Sistema(int argc, char ** argv);

	void iniciar();
	void finalizar_log();
};

#endif
