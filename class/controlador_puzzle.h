#ifndef CONTROLADOR_PUZZLE_H
#define CONTROLADOR_PUZZLE_H

/*Toma el control del juego cuando se entra en modo puzzle.

Va acompañado del "Cargador_puzzle" que cuando se cree cargará todos
los puzzles existentes para poder coger uno. Cuando se salga de ámbito
se eliminará toda la memoria, por supuesto.
*/

#include "input.h"
#include "tiempo_juego.h"
#include "herramientas_graficas.h"
#include "herramientas_calculos.h"
#include "control_pieza_puzzle.h"
#include "datos_sistema.h"
#include "datos_legibles.h"
#include "localizador.h"
#include "representador_tiempo_restante.h"
#include "utilidades_cargadores.h"
#include "../libdan_headers/libDanSDL.h"

/*Definición del puzzle...

rotacion:
0 -> 1
90 -> 2
180 -> 4
270 -> 8

id	w	h	x_rec	y_rec	w_rec	h_rec
0	2	2	0	0	0	0

solución:
indice	id_p	rotac
0	1	5	-> indice 0: x0 y0 en función de w y h	
1	2	0	-> indice 1: x1 y0
3	3	0	-> indice 3: x0 y1
[F]
*/


class Cargador_puzzle
{
	private:

	Controlador_definicion_puzzle cdp;
	static const std::string RUTA_DEFINICIONES;
	Pieza_colocada_puzzle leer_linea_como_info_solucion(const std::string&);

	public:

	void inicializar();
	Definicion_puzzle obtener_puzzle_por_id(unsigned int) const;

};

class Controlador_puzzle
{
	///////////////////////////////
	//Constantes y enumeraciones...

	private:

	//Medidas de la caja y ancho del borde.
	static const int X_CAJA=50;
	static const int Y_CAJA=50;
	static const int W_CAJA=500;
	static const int H_CAJA=300;
	static const int BORDE=4;

	//Posición del separador vertical.
	static const int X_SEPARADOR=300;
	static const int W_SEPARADOR=4;

	//Para centrar...
	static const int W_EFECTIVO_PIEZAS=178;
	static const int H_EFECTIVO_PIEZAS=178;

	static const int X_TABLERO=326;
	static const int Y_TABLERO=160;

	static const int W_HUECO_PIEZA=37;
	static const int H_HUECO_PIEZA=37;
	static const int BORDE_HUECO_PIEZA=2;
	static const int MARGEN_PIEZA=10;

	//Tamaños..
	static const int W_PIEZA_SIN_AMPLIAR=16;
	static const int H_PIEZA_SIN_AMPLIAR=16;

	//Posición del fondo de las piezas:
	static const int X_TABLETA=32;
	static const int Y_TABLETA=64;
	static const int W_TABLETA=16;
	static const int H_TABLETA=16;

	//Piezas disponibles...
	static const unsigned int PIEZAS_POR_LINEA=6;	//Cantidad de piezas que se muestran en una línea.
	static const int SALTO_X_PIEZAS=26;
	static const int SALTO_Y_PIEZAS=26;
	static const int W_REP_PIEZAS=146;
	static const int H_REP_PIEZAS=48;	//16+32
	static const int X_REP_PIEZAS=350;
	static const int Y_REP_PIEZAS=60;

	//Colores de ficha seleccionada.
	static Uint32 COLOR_TABLERO_ACTUAL;
	static Uint32 COLOR_TABLERO_NORMAL;
	static Uint32 COLOR_TABLERO_SELECCIONADA;

	//Offsets de iconos de ayuda...
	static const int X_INPUT_ARR_ABA=14;
	static const int Y_INPUT_ARR=-8;
	static const int Y_INPUT_ABA=38;
	static const int X_INPUT_IZQ=-11;
	static const int X_INPUT_DER=38;
	static const int Y_INPUT_IZQ_DER=14;

	//Recortes de los iconos de ayuda.
	static const int X_REC_IZQ=48;
	static const int Y_REC_IZQ=64;
	static const int X_REC_DER=58;
	static const int Y_REC_DER=64;
	static const int X_REC_ARR=48;
	static const int Y_REC_ARR=74;
	static const int X_REC_ABA=54;
	static const int Y_REC_ABA=74;

	static const int W_REC_ARR_ABA=6;
	static const int H_REC_ARR_ABA=6;
	static const int W_REC_IZQ_DER=10;
	static const int H_REC_IZQ_DER=10;

	static const int X_TXT_TITULO=240;
	static const int Y_TXT_TITULO=360;

	//Posición del dibujo de ayuda.
	static const int X_DIBUJO=66;
	static const int Y_DIBUJO=92;
	static const int W_MAX_DIBUJO=220;
	static const int H_MAX_DIBUJO=164;

	//Posición del texto de ayuda.
	static const int X_TEXTO_AYUDA=60;
	static const int Y_TEXTO_AYUDA=270;

	static const std::string RUTA_LOCALIZACION;
	static const int OFFSET_TEXTO_AYUDA=1000;

	//Tiempo de encenderse y apagarse las indicaciones del input.
	static const float T_PARPADEO;
	static const float T_DESTELLO_FINAL;

	enum estados {E_MOVER, E_CAMBIAR, E_FINALIZANDO, E_FINALIZADO};
	enum movimiento {M_ARR=1, M_DER=2, M_ABA=3, M_IZQ=4};
	enum rotacion {R_0=1, R_90=2, R_180=4, R_270=8};

	struct Punto
	{
		unsigned int x;
		unsigned int y;

		Punto(unsigned int px, unsigned int py): x(px), y(py) {}
	};

	////////////////////////////
	//Propiedades...
	
	private:
	
	//Referencias prestadas.
	Datos_sistema& datos_sistema;
	Tiempo_juego& tiempo_juego;

	//Gráficos...
	DLibV::Imagen * img_piezas; //Ampliación de las piezas.
	DLibV::Representacion_texto_auto_estatica rep_txt_titulo;
	DLibV::Representacion_texto_auto_estatica rep_txt_ayuda;
	
	DLibV::Primitiva_grafica_caja_estatica caja_borde;	//Caja blanca que rodea todo el puzzle.
	DLibV::Primitiva_grafica_caja_estatica caja_interior;	//Caja negra que está dentro de la caja blanca.
	DLibV::Primitiva_grafica_caja_estatica caja_separador;	//Separador entre las dos mitades del puzzle.

	//Localizador.
	Localizador loc_textos;

	bool montado;
	bool comprobar_finalizacion; //Esta bandera la usaremos para comprobar si se ha finalizado sólo cuando haya cambios.
	unsigned int estado;
	unsigned int indice_tablero; //La posición del cursor actual.
	std::vector<unsigned int> piezas_obtenidas;
	float tiempo;

	//Funcionamiento del puzzle...
	Definicion_puzzle puzzle_actual; //Patrón del puzzle
	std::vector<Pieza_colocada_puzzle> estado_puzzle; //Vector con el estado de cada casilla.
	
	////////////////////////////
	//Métodos

	private:

	bool procesar_controles(const Input&);
	void iniciar_finalizacion(DLibV::Pantalla&);
	void dibujar_tablero(DLibV::Pantalla&);
	void dibujar_piezas_disponibles(DLibV::Pantalla&);
	void dibujar_ayuda_input(DLibV::Representacion_agrupada_estatica&);
	void dibujar_dibujo_puzzle(DLibV::Pantalla&);
	void dibujar(DLibV::Pantalla&);
	void generar_imagen_piezas();
	void descartar_imagen_piezas();

	void generar_estado_puzzle();
	bool es_pieza_colocada(int, int=-1) const;
	Punto obtener_coordenadas_de_indice(unsigned int i);

	void seleccionar_pieza_tablero(unsigned int);
	void rotar_pieza(int);
	void cambiar_pieza(int);

	bool comprobar_solucion();

	//Copia deshabilitada.
	Controlador_puzzle(const Controlador_puzzle&);
	Controlador_puzzle& operator=(const Controlador_puzzle&);

	public:

	void preparar(unsigned int, const std::vector<unsigned int>&);
	bool loop(float, DLibV::Pantalla&, const Input&);
	bool es_montado() const {return montado;}
	bool es_finalizado() const {return estado==E_FINALIZADO;}
	void desmontar();

	Controlador_puzzle(Datos_sistema&, Tiempo_juego&);
	~Controlador_puzzle();
};

#endif
