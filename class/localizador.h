#ifndef LOCALIZADOR_H
#define LOCALIZADOR_H

#include "../libdan_headers/herramientas/localizador_base/localizador_base.h"


class Localizador:public DLibH::Localizador_base
{
	private:
	DLibH::t_cadena ruta;
	DLibH::t_cadena c_no_cargado;
	DLibH::t_cadena c_no_encontrado;

	protected:
	DLibH::t_lista_nombres_archivo obtener_lista_archivos();
	DLibH::t_cadena const& cadena_no_cargado() {return c_no_cargado;}
	DLibH::t_cadena const& cadena_no_encontrado() {return c_no_encontrado;}

	public:
	void inicializar(unsigned short int p_idioma);

	Localizador(unsigned short int p_idioma=0)
	:DLibH::Localizador_base(p_idioma), ruta(""),
	c_no_cargado("ERROR 00"), c_no_encontrado("ERROR 01")
	{}

	Localizador(const DLibH::t_cadena& p_ruta, unsigned short int p_idioma=0)
	:DLibH::Localizador_base(p_idioma), ruta(p_ruta),
	c_no_cargado("ERROR 00"), c_no_encontrado("ERROR 01")
	{}

	~Localizador()
	{}
};
#endif
