#ifndef CONTROL_ACTORES_PERSISTENTES_H
#define CONTROL_ACTORES_PERSISTENTES_H

/*Esta clase lleva el control de todos los actores persistentes que existen
en un momento dado en el juego. Los actores persistentes se organizan por salas,
que es la única unidad en la que pueden operar.

En un principio estas funcionalidades estaban integradas en el cargador de 
datos, pero las hemos sacado fuera para repartir responsabilidades.

Tiene una conexión con el visitante de recuento, para sumar los items
únicos que hay en el juego.*/

#include <map>
#include "sala.h" //La forma más sencilla de sacar el tema de los actores.
#include "control_objetivos.h"
#include "visitante_recuento_items.h"

class Control_actores_persistentes
{
	private:

	std::map<unsigned int, std::vector<Actor *> > actores_persistentes;

	public:

	Control_actores_persistentes();
	~Control_actores_persistentes();

	void efectuar_recuento_items(Control_objetivos&);
	void actualizar_borrado_actores(unsigned int id);
	void reiniciar();
	void insertar_actores(unsigned int, const std::vector<Actor*>&);
	std::vector<Actor *>& obtener_actores_persistentes_por_id(unsigned int id);
	const std::vector<Actor *>& obtener_actores_persistentes_por_id(unsigned int id) const;
	std::vector<unsigned int> obtener_vector_salas_con_actores_persistentes() const;
};

#endif
