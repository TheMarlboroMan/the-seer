#ifndef CONTROL_PIEZA_PUZZLE_H
#define CONTROL_PIEZA_PUZZLE_H

/*Llevará el control de las estructuras de piezas, que definen el aspecto
de la misma. Se implementa como una clase abstracta con métodos de acceso
a un contenedor estático.

Una pieza se define de este modo:

#id	id_r	x_rec	y_rec 
1	1	0	0
2	1	32	0
3	1	0	32

*/

#include "utilidades_cargadores.h"
#include "datos_legibles.h"

class Control_pieza_puzzle
{
	private:

	static const std::string RUTA_INFO;

	public:
	
	static const int W_PIEZA=12;
	static const int H_PIEZA=11;

	private:

	static Controlador_piezas_puzzle controlador_piezas; //Contenedor de definiciones de las piezas.

	private:

	Control_pieza_puzzle() {}
	static Definicion_pieza_puzzle obtener_pieza_desde_linea(const std::string&);

	public:

	static void preparar_datos(); //Cargar definiciones y preparar el mapa.
	static Definicion_pieza_puzzle obtener_definicion_pieza(unsigned int);
	static std::vector<unsigned int> obtener_vector_claves();
	static unsigned int obtener_id_maximo();

};

#endif
