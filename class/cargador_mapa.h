#ifndef CARGADOR_MAPA_H
#define CARGADOR_MAPA_H

/*
Existirá cuando se abra el mapa. Cargará la información y se destruirá
luego. 
*/

#include "utilidades_cargadores.h"
#include "datos_mapa.h"
#include "localizador.h"

extern DLibH::Log_base LOG;

struct Excepcion_mapa
{
	
};

class Cargador_mapa
{
	private:

	static const std::string FICHERO_DATOS;
	static const std::string DELIMITADOR;
	static const char SEPARADOR='\t';

	enum estados{E_SALA, E_CELDA};

	Gestor_mapa_juego mapa;
	unsigned int estado;

	void leer_linea_como_sala(std::vector<unsigned int>&, Mapa_sala&);
	void leer_linea_como_celda(std::vector<unsigned int>&, Mapa_sala&);

	public:
	Cargador_mapa();
	void inicializar();
	const Gestor_mapa_juego& acc_mapa() const {return mapa;}
	const Mapa_sala& obtener_sala_por_id(unsigned int);			
};

#endif
