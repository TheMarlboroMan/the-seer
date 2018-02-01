#ifndef CARGADOR_DIAPOSITIVAS_H
#define CARGADOR_DIAPOSITIVAS_H

/*
Al contrario que el resto de cargadores, este existirá durante poco rato,
sólo para cargar la información necesaria. Cuando se haya generado un pase
de diapositivas podemos prescindir de la memoria puesto que se harían
copias de la información necesaria.
*/

#include "utilidades_cargadores.h"
#include "datos_diapositivas.h"
#include "localizador.h"

extern DLibH::Log_base LOG;

struct Excepcion_diapositivas
{
	
};

class Cargador_diapositivas
{
	private:
	static const std::string FICHERO_DATOS;
	static const std::string DELIMITADOR;
	static const char SEPARADOR='\t';

	Gestor_diapositiva_texto textos;
	Gestor_diapositiva_imagen imagenes;
	Gestor_diapositiva diapositivas;
	Gestor_pase_diapositivas pases;
	Localizador& loc_textos; //Referencia prestada.

	void marco_lectura(std::ifstream&, void (Cargador_diapositivas::*)(std::vector<unsigned int>&));

	void inicializar_textos(std::vector<unsigned int>&);
	void inicializar_imagenes(std::vector<unsigned int>&);
	void inicializar_diapositivas(std::vector<unsigned int>&);
	void inicializar_pases(std::vector<unsigned int>&);

	public:
	Cargador_diapositivas(Localizador&);
	void inicializar();
	Pase_diapositivas obtener_pase_por_id(unsigned int pid) const;
};

#endif
