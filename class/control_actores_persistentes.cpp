#include "control_actores_persistentes.h"

Control_actores_persistentes::Control_actores_persistentes()
{

}

Control_actores_persistentes::~Control_actores_persistentes()
{
	reiniciar();
}


/*Ojo al dato: no podemos devolver un vector vacío porque van por referencia.
La excepción saltará muy a menudo, de hecho.*/

std::vector<Actor *>& Control_actores_persistentes::obtener_actores_persistentes_por_id(unsigned int id)
{
	if(!actores_persistentes.count(id))
	{
		throw std::out_of_range("No existen actores persistentes en la sala");
	}

	return actores_persistentes[id];
}

const std::vector<Actor *>& Control_actores_persistentes::obtener_actores_persistentes_por_id(unsigned int id) const
{
	if(!actores_persistentes.count(id))
	{
		throw std::out_of_range("No existen actores persistentes en la sala");
	}

	return actores_persistentes.at(id);
}

void Control_actores_persistentes::reiniciar()
{
	for(std::pair<unsigned int, std::vector<Actor *> > par : actores_persistentes) 
	{
		for(Actor * a : par.second) 
		{
			delete a;
		}

		par.second.clear();
	}

	actores_persistentes.clear();
}

/*Al igual que la sala, si un actor se marca para borrar, liberaremos la
memoria aquí.*/

void Control_actores_persistentes::actualizar_borrado_actores(unsigned int id)
{
	if(actores_persistentes.count(id))
	{
		std::vector<Actor *>& actores=obtener_actores_persistentes_por_id(id);
		std::vector<Actor *>::iterator ini=actores.begin(), fin=actores.end();

		while(ini < fin)
		{
			Actor* a=*ini;

			if(a->es_marcado_para_borrar())
			{
//				delete *ini;
				delete a;
				ini=actores.erase(ini);
				fin=actores.end();
			}
			else ++ini;
		}
	}
}

/*Lee todos los actores persistentes y efectúa el recuento de los tipos de items 
que hay.*/

void Control_actores_persistentes::efectuar_recuento_items(Control_objetivos& contadores_items)
{
	for(std::pair<unsigned int, std::vector<Actor *> > par : actores_persistentes)
	{
		Visitante_recuento_items vr;
		for(Actor * a : par.second) a->aceptar_visitante(vr);

		contadores_items.sumar_total(Control_objetivos::T_TESORO, vr.tesoro);
		contadores_items.sumar_total(Control_objetivos::T_COFRE, vr.cofres);
	}
}

void Control_actores_persistentes::insertar_actores(unsigned int id_sala, const std::vector<Actor*>& actores)
{
	actores_persistentes.insert(std::pair<unsigned int, std::vector<Actor *> >(id_sala, actores));
}

/*Esto devuelve un vector de enteros. Cada entero es el id de una sala en la que 
hay actores persisentes.*/


std::vector<unsigned int> Control_actores_persistentes::obtener_vector_salas_con_actores_persistentes() const
{
	std::vector<unsigned int> resultado;


	for(std::pair<unsigned int, std::vector<Actor *> > par : actores_persistentes)
	{
		if(par.second.size()) 
		{
			resultado.push_back(par.first);
		}
	}

	return resultado;
}
