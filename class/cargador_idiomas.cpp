#include "cargador_idiomas.h"

const std::string Cargador_idiomas::RUTA_ARCHIVO="data/info/datos_idiomas.txt";

Cargador_idiomas::Cargador_idiomas()
{
	inicializar();
}

void Cargador_idiomas::inicializar()
{
	idiomas.clear();
	std::ifstream archivo(RUTA_ARCHIVO.c_str(), std::ios::in);

	if(archivo)
	{
		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
			if(linea.size())
			{
				idiomas.push_back(linea);
			}
		}
	}
}

