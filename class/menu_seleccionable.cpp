#include "menu_seleccionable.h"

const int Menu_seleccionable::ALPHA_ACTUAL=255;
const int Menu_seleccionable::ALPHA_OTRO=128;
const int Menu_seleccionable::MARGEN_OPCIONES=8;

Menu_seleccionable::Menu_seleccionable()
	:opcion_actual(0)
{
	
}

Menu_seleccionable::~Menu_seleccionable()
{
	opciones.clear();
}

void Menu_seleccionable::insertar_opcion(const std::string& c)
{
/*	DLibV::Representacion_texto_auto_estatica rep(DLibV::Gestor_recursos_graficos::obtener(3), "");
	rep.mut_interletra(1);
	rep.asignar(c);

	opciones.push_back(rep);
*/

	opciones.push_back(
		DLibV::Representacion_texto_auto_estatica(DLibV::Gestor_recursos_graficos::obtener(3), ""));

	unsigned int indice=opciones.size()-1;
	DLibV::Representacion_texto_auto_estatica& r=opciones.at(indice);

//	r.mut_interletra(1);
	r.asignar(c);
}

void Menu_seleccionable::eliminar_opcion(unsigned int i)
{
	if(i < opciones.size())
	{
		opciones.erase(opciones.begin()+i);
		opcion_actual=0;
	}
}

void Menu_seleccionable::cambiar_opcion(unsigned int i, const std::string& c)
{
	try
	{
		DLibV::Representacion_texto_auto_estatica& rep=opciones.at(i);
		rep.asignar(c);
	}
	catch(std::out_of_range& e)
	{}
}

void Menu_seleccionable::subir()
{
	int val=opcion_actual;
	--val;
	if(val >= 0) opcion_actual=val;

}

void Menu_seleccionable::bajar()
{
	unsigned int val=opcion_actual;
	++val;
	if(val < opciones.size()) opcion_actual=val;
}

void Menu_seleccionable::seleccionar_opcion(unsigned int i)
{
	if(i < opciones.size()) opcion_actual=i;
}

void Menu_seleccionable::dibujar(DLibV::Pantalla& p, int pos_x, int pos_y)
{
	unsigned int indice=0;
	int x=pos_x;
	int y=pos_y;

	for(DLibV::Representacion_texto_auto_estatica& rep : opciones)
	{
		int alpha=indice==opcion_actual ? ALPHA_ACTUAL : ALPHA_OTRO;

		rep.establecer_alpha(alpha);
		rep.establecer_posicion(x, y);
		rep.volcar(p);

		y+=rep.acc_alto_area()+MARGEN_OPCIONES;

		++indice;
	}
}

void Menu_seleccionable::vaciar()
{
	opciones.clear();
}
