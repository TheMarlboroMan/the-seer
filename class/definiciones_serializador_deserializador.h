#ifndef DEFINICIONES_SERIALIZADOR_DESERIALIZADOR
#define DEFINICIONES_SERIALIZADOR_DESERIALIZADOR

#include <string>
#include "../libdan_headers/herramientas/herramientas/herramientas.h"

class Definiciones_serializador_deserializador
{
	private:

	Definiciones_serializador_deserializador();
	static char obtener_marca(unsigned int);

	static const std::string RUTA_ARCHIVOS;
	static const std::string EXTENSION_ARCHIVOS;
	static const std::string NOMBRE_ARCHIVO;

	static const char C_ABRE_MARCA='<';
	static const char C_CIERRA_MARCA='>';
	static const char C_MARCA_INICIO='a';
	static const char C_MARCA_INFO_JUEGO='b';
	static const char C_MARCA_TIEMPO_JUEGO='c';
	static const char C_MARCA_CONTROL_SALAS='d';
	static const char C_MARCA_CONTROL_OBJETIVOS='e';
	static const char C_MARCA_CONTROL_PUZZLE='f';
	static const char C_MARCA_CONTROL_ENERGIA='g';
	static const char C_MARCA_CONTROL_HABILIDADES='h';
	static const char C_MARCA_CONTROL_DATOS_PERSISTENTES='i';
	static const char C_MARCA_CONTROL_ACTORES_PERSISTENTES='j';

	public:
	
	static const std::string SEPARADOR_SECCION;
	static const char SEPARADOR_DATOS_ACTORES='\t';
	static const char SEPARADOR_DATOS=' ';
	static const char SEPARADOR_INTERNO='_';

	enum marcas {
M_INICIO,
M_INFO_JUEGO,
M_TIEMPO_JUEGO,
M_CONTROL_SALAS,
M_CONTROL_OBJETIVOS,
M_CONTROL_PUZZLE,
M_CONTROL_ENERGIA,
M_CONTROL_HABILIDADES,
M_CONTROL_DATOS_PERSISTENTES,
M_CONTROL_ACTORES_PERSISTENTES
};

	static std::string construir_ruta_archivo(unsigned int); //const std::string&);
	static std::string marcar(unsigned int); //const std::string&);
	static bool comprobar_marca(const std::string&, unsigned int);
};

#endif
