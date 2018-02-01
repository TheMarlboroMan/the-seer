#include "localizador.h"

DLibH::t_lista_nombres_archivo Localizador::obtener_lista_archivos()
{
	DLibH::t_lista_nombres_archivo resultado;

	resultado.push_back(ruta);

	return resultado;
}

void Localizador::inicializar(unsigned short int p_idioma)
{		
	DLibH::Localizador_base::cambiar_idioma(p_idioma);
}
