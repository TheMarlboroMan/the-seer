#ifndef CONTROL_PUZZLE_H
#define CONTROL_PUZZLE_H

/*Llevará el control de las piezas que están en la posesión del 
jugador en este momento. 

Por conveniencia vamos a poner que depende del controlador de juego
y que se inicializa con el mismo, a la vez que se inicializan los datos
de actores y demás películas.*/

#include "control_pieza_puzzle.h"

class Control_puzzle
{
	private:

	std::map<unsigned int, bool> piezas_recogidas; //Mapa con todas las piezas, indicando aquellas que se han recogido.		

	public:

	bool es_pieza_recogida(unsigned int) const;
	void recoger_pieza(unsigned int);
	void reiniciar();
	std::vector<unsigned int> obtener_piezas_recogidas() const;
	unsigned int obtener_total_piezas() const;

	Control_puzzle();
};

#endif
