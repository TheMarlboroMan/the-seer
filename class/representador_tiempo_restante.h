#ifndef REPRESENTADOR_TIEMPO_RESTANTE
#define REPRESENTADOR_TIEMPO_RESTANTE

/*Dado que el tiempo restante se representa uniformemente entre los
diferentes modos en los que corre el tiempo, vamos a eliminar algo
de duplicidad de código haciendo esta clase, que se encargará de
representarlo.*/

#include <string>
#include "../libdan_headers/libDanSDL.h"

class Representador_tiempo_restante
{
	private:

	Representador_tiempo_restante(); //Yeah. Fuck off.

	static const int X_CAJA=200;
	static const int Y_CAJA=-4;
	static const int W_CAJA=200;
	static const int H_CAJA=30;
	static const int BORDE_CAJA=2;

	static const int X_TEXTO=220;
	static const int Y_TEXTO=12;

	public:

	static void generar_contador_tiempo(DLibV::Pantalla& pantalla, const std::string& texto);

};

#endif
