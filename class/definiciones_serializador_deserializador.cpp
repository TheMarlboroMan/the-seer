#include "definiciones_serializador_deserializador.h"

const std::string Definiciones_serializador_deserializador::SEPARADOR_SECCION="[F]";
const std::string Definiciones_serializador_deserializador::RUTA_ARCHIVOS="data/usuario/";
const std::string Definiciones_serializador_deserializador::EXTENSION_ARCHIVOS=".dat";
const std::string Definiciones_serializador_deserializador::NOMBRE_ARCHIVO="sesion_";

std::string Definiciones_serializador_deserializador::construir_ruta_archivo(unsigned int i)
{
	std::string resultado=RUTA_ARCHIVOS+
		NOMBRE_ARCHIVO+
		DLibH::Herramientas::entero_a_cadena(i)+
		EXTENSION_ARCHIVOS;
	return resultado;
}

char Definiciones_serializador_deserializador::obtener_marca(unsigned int i)
{
	char c;

	switch(i)
	{
		case M_INICIO: c=C_MARCA_INICIO; break;
		case M_INFO_JUEGO: c=C_MARCA_INFO_JUEGO; break;
		case M_TIEMPO_JUEGO: c=C_MARCA_TIEMPO_JUEGO; break;
		case M_CONTROL_SALAS: c=C_MARCA_CONTROL_SALAS; break;
		case M_CONTROL_OBJETIVOS: c=C_MARCA_CONTROL_OBJETIVOS; break;
		case M_CONTROL_PUZZLE: c=C_MARCA_CONTROL_PUZZLE; break;
		case M_CONTROL_ENERGIA: c=C_MARCA_CONTROL_ENERGIA; break;
		case M_CONTROL_HABILIDADES: c=C_MARCA_CONTROL_HABILIDADES; break;
		case M_CONTROL_DATOS_PERSISTENTES: c=C_MARCA_CONTROL_DATOS_PERSISTENTES; break;
		case M_CONTROL_ACTORES_PERSISTENTES: c=C_MARCA_CONTROL_ACTORES_PERSISTENTES; break;
	}

	return c;
}

std::string Definiciones_serializador_deserializador::marcar(unsigned int i)
{
	char c=obtener_marca(i);
	std::string m;
	m+=c;

	std::string resultado;
	resultado+=C_ABRE_MARCA;
	resultado+=m;
	resultado+=C_CIERRA_MARCA;

	return resultado;
}

bool Definiciones_serializador_deserializador::comprobar_marca(const std::string& pieza, unsigned int i)
{
	char c=obtener_marca(i);

	return 
		pieza[0]==C_ABRE_MARCA 
		&& pieza[2]==C_CIERRA_MARCA
		&& pieza[1]==c;
}
