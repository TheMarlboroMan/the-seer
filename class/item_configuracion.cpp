#include "item_configuracion.h"

Item_configuracion::Item_configuracion(std::string p_titulo)
{
	this->titulo=p_titulo;	
	this->iterador_opciones=this->opciones.begin();
}

Item_configuracion::Item_configuracion(const Item_configuracion& p):
	titulo(p.titulo),
	opciones(p.opciones),
	iterador_opciones(opciones.begin())
{
	establecer_actual(p.acc_valor_seleccionado());
}

Item_configuracion& Item_configuracion::operator=(const Item_configuracion& p)
{
	titulo=p.titulo;
	opciones=p.opciones;
	iterador_opciones=opciones.begin();
	establecer_actual(p.acc_valor_seleccionado());

	return *this;
}

Item_configuracion::~Item_configuracion()
{
	this->opciones.clear();
}
