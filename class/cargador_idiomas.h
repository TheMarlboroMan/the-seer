#ifndef CARGADOR_IDIOMAS_H
#define CARGADOR_IDIOMAS_H

#include "utilidades_cargadores.h"

class Cargador_idiomas
{
	private:

	static const std::string RUTA_ARCHIVO;
	std::vector<std::string> idiomas;
	
	void inicializar();

	public:

	Cargador_idiomas();
	const std::vector<std::string>& acc_idiomas() const {return idiomas;}
};

#endif
