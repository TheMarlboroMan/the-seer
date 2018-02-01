#ifndef MENU_SELECCIONABLE_H
#define MENU_SELECCIONABLE_H

/*Sencillo y rápido. Podríamos especializarlo haciendo primero un menú
que sólo tuviera textos y que luego pudiéramos transformar a SDL, pero 
con esto tardamos poco.*/

#include "../libdan_headers/libDanSDL.h"
#include <string>
#include <vector>

class Menu_seleccionable
{
	private:

	static const int ALPHA_ACTUAL;
	static const int ALPHA_OTRO;
	static const int MARGEN_OPCIONES;

	std::vector<DLibV::Representacion_texto_auto_estatica> opciones;
	unsigned int opcion_actual;

	public:

	Menu_seleccionable();
	~Menu_seleccionable();

	void insertar_opcion(const std::string&);
	void eliminar_opcion(unsigned int);
	void cambiar_opcion(unsigned int, const std::string&);
	void vaciar();

	void subir();
	void bajar();
	void seleccionar_opcion(unsigned int);
	
	unsigned int acc_opcion_actual() const {return opcion_actual;}

	void dibujar(DLibV::Pantalla&, int, int);
};

#endif
