#ifndef CARGADOR_RECURSOS_H
#define CARGADOR_RECURSOS_H

#include "utilidades_cargadores.h"

extern DLibH::Log_base LOG;

class Cargador_recursos
{
	private:

	static const std::string FICHERO_AUDIO;
	static const std::string FICHERO_GRAFICOS;

	public:
	
	Cargador_recursos();
	~Cargador_recursos();

	void generar_recursos_graficos();
	void generar_recursos_audio();
};

#endif
