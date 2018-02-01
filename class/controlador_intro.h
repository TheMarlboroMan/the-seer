#ifndef CONTROLADOR_INTRO
#define CONTROLADOR_INTRO

/*Controlador de la introducción al juego. Si hay logos cuando comienza
y ese tipo de historias saldrá aquí.

En un principio nos lleva a la pantalla de intro, de donde sólo podemos pasar
al sistema de nuevo o al juego en si.*/

#include "input.h"
#include "localizador.h"
#include "datos_sistema.h"
#include "item_configuracion.h"
#include "configuracion.h"
#include "cargador_idiomas.h"
#include "herramientas_calculos.h"
#include "menu_seleccionable.h"
#include "control_sesiones_guardadas.h"
#include "estructura_animacion.h"
#include "../libdan_headers/libDanSDL.h"

class Controlador_intro
{
	private:

	enum estados{E_INTRO, E_MENU, E_SELECCION_PARTIDA, E_OPCIONES, E_INICIAR_JUEGO, E_SALIR_SISTEMA, E_VOLVER_JUEGO};
	enum config{VIDEO_PANTALLA_COMPLETA, VIDEO_HARDWARE, VOLUMEN_SONIDO, VOLUMEN_MUSICA, IDIOMA};

	//Indices de cadenas de localización.
	static const int INDICE_CADENA_PULSA_TECLA=20;
	static const int INDICE_CADENA_INICIAR_CARGAR_JUEGO=21;
	static const int INDICE_CADENA_OPCIONES=22;
	static const int INDICE_CADENA_VOLVER_JUEGO=23;

	static const int INDICE_CADENA_SELECCION_DATOS_JUEGO=30;
	static const int INDICE_CADENA_DATOS_JUEGO=31;
	static const int INDICE_CADENA_DATOS_JUEGO_LIBRE=32;

	static const int INDICE_CADENA_CONFIG_FULLSCREEN=50;
	static const int INDICE_CADENA_CONFIG_HARDWARE=51;
	static const int INDICE_CADENA_CONFIG_MUSICA=52;
	static const int INDICE_CADENA_CONFIG_SONIDO=53;
	static const int INDICE_CADENA_CONFIG_IDIOMA=54;

	static const int INDICE_CADENA_VAL_CONFIG_SI=70;
	static const int INDICE_CADENA_VAL_CONFIG_NO=71;
	static const int INDICE_CADENA_VAL_CONFIG_VOL_0=72;
	static const int INDICE_CADENA_VAL_CONFIG_VOL_1=73;
	static const int INDICE_CADENA_VAL_CONFIG_VOL_2=74;
	static const int INDICE_CADENA_VAL_CONFIG_VOL_3=75;
	static const int INDICE_CADENA_VAL_CONFIG_VOL_4=76;
	static const int INDICE_CADENA_VAL_CONFIG_VOL_5=77;

	static const std::string RUTA_LOCALIZACION;
	static const float T_PARPADEO;

	//Posiciones de textos...

	static const int X_TXT_PULSA_TECLA=440;
	static const int Y_TXT_PULSA_TECLA=380;

	static const int X_TXT_INICIAR_CARGAR_JUEGO=340;
	static const int Y_TXT_INICIAR_CARGAR_JUEGO=200;

	static const int X_MENUS=260;
	static const int Y_MENUS=240;
	static const int DESP_X_CONFIG=190;

	//Posiciones de fondo...
	static const int X_FONDO_PRINCIPAL=-90;
	static const int Y_FONDO_PRINCIPAL=0;
	static const int Y_FONDO_SECUNDARIO=54;
	static const int DESP_X_FONDO=90;
	static const int SALTO_X_FONDO=60;

	
	Datos_sistema& datos_sistema;
	Control_sesiones_guardadas& control_sesiones_guardadas;
	Configuracion& configuracion;
	
	Localizador loc;

	//Hay varios textos fijos...
	DLibV::Representacion_texto_auto_estatica txt_pulsar_tecla;
	DLibV::Representacion_texto_auto_estatica txt_iniciar_cargar_juego;

	//Un par de imágenes...
	DLibV::Representacion_bitmap_estatica fondo;
	DLibV::Representacion_bitmap_estatica frente;

	//Y varios menús...
	Menu_seleccionable menu_principal;
	Menu_seleccionable menu_partidas;
	Menu_seleccionable menu_opciones_titulos; //Las opciones se simulan usando dos menús.
	Menu_seleccionable menu_opciones_valores;

	//Un sólo grupo de animación.
	Grupo_animacion grupo_animacion;

	//Un vector de configuraciones...
	std::vector<Item_configuracion> items_configuracion;
	
	unsigned int estado;
	unsigned int id_sesion_guardado;
	bool volver_a_juego_habilitado;
	float tiempo;

	void dibujar(DLibV::Pantalla&); 
	void dibujar_estado_intro(DLibV::Pantalla&); 
	void dibujar_estado_menu(DLibV::Pantalla&); 
	void dibujar_estado_seleccion_partida(DLibV::Pantalla&); 
	void dibujar_estado_opciones(DLibV::Pantalla&);

	void procesar_input(const Input&);

	void cambiar_estado(unsigned int);
	void logica(float);
	void actualizar_alpha_fondo();

	void generar_menus();
	void generar_sesiones();
	void generar_opciones();

	void limpiar_sesiones();

	void eliminar_sesion(unsigned int);
	void cambiar_valor_opciones(unsigned int, int);

	int obtener_x_fondo_para_estado(unsigned int);

	public:

	void loop(float, DLibV::Pantalla&, const Input& input);

	bool es_iniciar_juego() const {return estado==E_INICIAR_JUEGO;}
	bool es_salir_a_sistema() const {return estado==E_SALIR_SISTEMA;}
	bool es_volver_a_juego() const {return estado==E_VOLVER_JUEGO;}
	
	void habilitar_volver_a_juego();
	void restaurar_estado_menu();	
	void establecer_en_intro() {estado=E_INTRO;}
	unsigned int acc_id_sesion_guardado() const {return id_sesion_guardado;}

	Controlador_intro(Datos_sistema&, Control_sesiones_guardadas&, Configuracion&);
};

#endif
