#include "tiempo_juego.h"
#include "../libdan_headers/herramientas/herramientas/herramientas.h"

const float Tiempo_juego::VAL_MULTIPLICADOR_TIEMPO_NORMAL=1.0;
const float Tiempo_juego::VAL_MULTIPLICADOR_TIEMPO_PARADO=0.0;

Tiempo_juego::Tiempo_juego():
	segundos_restantes(0.0), multiplicador_tiempo(VAL_MULTIPLICADOR_TIEMPO_NORMAL)
{
}

void Tiempo_juego::turno(float p_delta)
{
	segundos_restantes-=(p_delta * multiplicador_tiempo);
	if(segundos_restantes < 0.0) segundos_restantes=0.0;
}

void Tiempo_juego::sumar_segundos(int t)
{
	segundos_restantes+=t;
	if(segundos_restantes < 0.0) segundos_restantes=0.0;
}

void Tiempo_juego::establecer_segundos_restantes(unsigned int t)
{
	segundos_restantes=t;
}

std::string Tiempo_juego::como_cadena(unsigned int segundos_restantes)
{
	unsigned int s=segundos_restantes % 60;
	unsigned int tm=floor(segundos_restantes / 60); 
	unsigned int h=floor(segundos_restantes / 3600);
	unsigned int m=tm % 60;

	std::string pad_s=s < 10 ? "0" : "";
	std::string pad_m=m < 10 ? "0" : "";


	std::string texto=
		DLibH::Herramientas::entero_a_cadena(h)+":"+
		pad_m+DLibH::Herramientas::entero_a_cadena(m)+":"+
		pad_s+DLibH::Herramientas::entero_a_cadena(s);

	return texto;
}
