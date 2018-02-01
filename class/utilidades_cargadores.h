#ifndef UTILIDADES_CARGADORES_H
#define UTILIDADES_CARGADORES_H

/*Herramientas para los cargadores de datos...*/

#include "../libdan_headers/libDanSDL.h"
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <exception>

extern DLibH::Log_base LOG;

class Utilidades_cargadores
{
	private:
	
	Utilidades_cargadores();

	public:

	static std::vector<unsigned int> explotar_linea_a_enteros(const std::string& linea, const char separador);
	static std::string obtener_siguiente_linea_archivo(std::ifstream& archivo);
};

#endif
