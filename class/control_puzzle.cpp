#include "control_puzzle.h"

Control_puzzle::Control_puzzle()
{

}

bool Control_puzzle::es_pieza_recogida(unsigned int idp) const
{
	if(!piezas_recogidas.count(idp)) return false;
	else return piezas_recogidas.at(idp);
}

void Control_puzzle::recoger_pieza(unsigned int idp)
{
	if(piezas_recogidas.count(idp)) 
	{
		piezas_recogidas[idp]=true;
	}
}

void Control_puzzle::reiniciar()
{
	piezas_recogidas.clear();
	std::vector<unsigned int> v=Control_pieza_puzzle::obtener_vector_claves();
	for(unsigned int i : v)
	{
		piezas_recogidas[i]=false;
	}
}

std::vector<unsigned int> Control_puzzle::obtener_piezas_recogidas() const
{
	std::vector<unsigned int> r;
	auto it=piezas_recogidas.begin();

	while(it != piezas_recogidas.end())
	{
		if(it->second)
		{
			r.push_back(it->first);
		}

		++it;
	}

	return r;
}


unsigned int Control_puzzle::obtener_total_piezas() const
{
	unsigned int resultado=0;
	auto it=piezas_recogidas.begin();

	while(it != piezas_recogidas.end())
	{
		if(it->second) ++resultado;
		++it;
	}

	return resultado;
}
