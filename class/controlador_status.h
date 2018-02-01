#ifndef CONTROLADOR_STATUS_H
#define CONTROLADOR_STATUS_H

/*Subsidiario del controlador auxiliar, muestra la información 
en pantalla de los objetivos cumplidos y el inventario...*/

#include <string>
#include <vector>
#include "localizador.h"
#include "info_juego.h"
#include "datos_sistema.h"
#include "control_objetivos.h"
#include "control_salas.h"
#include "control_pieza_puzzle.h"
#include "../libdan_headers/libDanSDL.h"
#include "control_habilidades.h" //Sólo para usar el enum de las habilidades. No lo vamos a listar como dependencia.

class Controlador_status
{
	//Enumeraciones y constantes...
	private:

	static const std::string RUTA_LOCALIZACION;

	static Uint32 COLOR_FONDO_INFO;
	static const int W_INFO=200;
	static const int H_INFO=400;
	static const int X_INFO=0;
	static const int Y_INFO=0;

	static const int X_SIMBOLOS=32;
	static const int X_TEXTOS=64;
	static const int MARGEN_Y_TEXTO=5;
	static const int Y_INICIO_SIMBOLOS=16;
	static const int SALTO_Y_SIMBOLOS=24;

	//Para las estadísticas.

	static const int W_SIMBOLO=16;
	static const int H_SIMBOLO=22;

	static const int X_SIMBOLO_TIEMPO=16;
	static const int Y_SIMBOLO_TIEMPO=0;

	static const int X_SIMBOLO_SALAS=32;
	static const int Y_SIMBOLO_SALAS=0;

	static const int X_SIMBOLO_SECRETOS=48;
	static const int Y_SIMBOLO_SECRETOS=0;

	static const int X_SIMBOLO_COFRES=0;
	static const int Y_SIMBOLO_COFRES=0;

	static const int X_SIMBOLO_LLAVES=80;
	static const int Y_SIMBOLO_LLAVES=0;

	static const int X_SIMBOLO_BONUS_TIEMPO=96;
	static const int Y_SIMBOLO_BONUS_TIEMPO=0;

	static const int X_SIMBOLO_TESORO=64;
	static const int Y_SIMBOLO_TESORO=0;

	static const int INDICE_LOCALIZACION_AMULETO=95;
	static const int INDICE_LOCALIZACION_PUZZLE=96;

	//Para las habilidades

	static const int X_TITULO_SECCION_HABILIDAD=16;
	static const int Y_TITULO_SECCION_HABILIDAD=294;

	static const int X_RECORTE_HABILIDAD=0;
	static const int Y_RECORTE_HABILIDAD=48;
	static const int W_RECORTE_HABILIDAD=16;
	static const int H_RECORTE_HABILIDAD=16;
	static const int X_ICONO_HABILIDAD=16;
	static const int Y_ICONO_HABILIDAD=310;
	
	static const int X_TITULO_HABILIDAD=40;
	static const int Y_TITULO_HABILIDAD=314;

	static const int X_TEXTO_HABILIDAD=16;
	static const int Y_TEXTO_HABILIDAD=340;

	//Para las piezas...
	static const int X_TITULO_SECCION_PUZZLE=16;
	static const int Y_TITULO_SECCION_PUZZLE=220;

	static const int W_REP_PIEZAS=180;
	static const int H_REP_PIEZAS=48;
	static const int X_REP_PIEZAS=16;
	static const int Y_REP_PIEZAS=248;

	static const int SALTO_Y_PIEZAS=20;
	static const int SALTO_X_PIEZAS=30;

	static const int X_TABLETA=32;
	static const int Y_TABLETA=64;
	static const int W_TABLETA=16;
	static const int H_TABLETA=16;

	enum datos_info{C_TIEMPO, C_SALAS, C_SECRETOS, C_COFRES, C_LLAVES, C_BONUS_TIEMPO, C_TESORO, C_MAX};

	//Propiedades
	private:

	//Referencias prestadas.
	Datos_sistema& datos_sistema;
	const Info_juego& info_juego;
	const Control_salas& control_salas;
	const Control_objetivos& control_objetivos;

	//Propios...
	Localizador loc;

	std::vector<unsigned int> piezas_obtenidas;
	std::vector<std::string> cadenas_info;
	std::vector<SDL_Rect> recortes_simbolos;
	DLibV::Representacion_bitmap_estatica bmp;

	bool mostrar_info_habilidad;
	DLibV::Representacion_bitmap_estatica icono_habilidad;
	DLibV::Representacion_texto_auto_estatica titulo_seccion_habilidad;
	DLibV::Representacion_texto_auto_estatica titulo_habilidad;
	DLibV::Representacion_texto_auto_estatica texto_habilidad;

	DLibV::Representacion_compuesta_estatica * rep_iconos_puzzle;
	DLibV::Representacion_texto_auto_estatica titulo_seccion_puzzle;

	//Métodos
	private:
	
	void limpiar_iconos_puzzle();
	void calcular_datos_puzzle(const std::vector<unsigned int>&);
	unsigned int obtener_indice_titulo_habilidad(unsigned int);
	unsigned int obtener_indice_texto_habilidad(unsigned int);

	//Estos están sin implementar: tiene un puntero que no quiero copiar. 
	Controlador_status(const Controlador_status&);
	Controlador_status& operator=(const Controlador_status&);
	
	public:

	Controlador_status(Datos_sistema&, const Info_juego&, const Control_salas&, const Control_objetivos&);
	~Controlador_status();
	void loop(float);
	void dibujar(DLibV::Pantalla&, float);
	void inicializar(const std::string&, const std::vector<unsigned int>&);
	void calcular_datos_habilidad(int);


};

#endif
