#include "utilidades_cargadores.h"

std::vector<unsigned int> Utilidades_cargadores::explotar_linea_a_enteros(const std::string& linea, const char separador)
{
	std::vector<unsigned int> resultado;

	std::vector<std::string> cadenas=DLibH::Herramientas::explotar(linea, separador);
	for(const std::string& c : cadenas) resultado.push_back(DLibH::Herramientas::cadena_a_entero(c));
	return resultado;
}

std::string Utilidades_cargadores::obtener_siguiente_linea_archivo(std::ifstream& archivo)
{
	if(archivo.eof())
	{
		return "";
	}
	else
	{
		char * buffer=new char[1024];
		std::string cadena;

		memset(buffer, '\0', 1024);
		archivo.getline(buffer, 1024);
		cadena.assign(buffer);
		delete [] buffer;

		//La cadena esta vacia?	Si no estamos leyendo saltar al siguiente con "continue".
		if(!cadena.size() || cadena[0]=='#') return obtener_siguiente_linea_archivo(archivo);
		else return cadena;
	}
}
