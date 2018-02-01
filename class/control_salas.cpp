#include "control_salas.h"
#include <iostream>

Control_salas::Control_salas()
{

}

Control_salas::~Control_salas()
{

}

bool Control_salas::descubrir_sala(unsigned int pid)
{
	if(!diccionario.count(pid)) return false;
	else 
	{
		Info_sala& is=diccionario[pid];

		if(!is.descubierta)
		{
			descubiertas[is.tipo]++;
			is.descubierta=true;
		}

		return true;
	}
}

bool Control_salas::marcar_gemas(unsigned int pid, bool v)
{
	if(!diccionario.count(pid)) return false;
	else 
	{
		Info_sala& is=diccionario[pid];
		is.con_gemas=v;
		return true;
	}
}

bool Control_salas::es_con_gemas(unsigned int pid) const
{
	if(!diccionario.count(pid)) return false;
	else return diccionario.at(pid).con_gemas;
}

bool Control_salas::es_sala_descubierta(unsigned int pid) const
{
	if(!diccionario.count(pid)) return false;
	else return diccionario.at(pid).descubierta;
}

bool Control_salas::es_sala_secreta(unsigned int pid) const
{
	if(!diccionario.count(pid)) return false;
	else return diccionario.at(pid).tipo==TP_SECRETO;
}

void Control_salas::insertar_info_sala(unsigned int pid, unsigned int pt)
{
	if(!diccionario.count(pid))
	{
		if(!totales.count(pt))
		{
			totales.insert(std::pair<unsigned int, unsigned int>(pt, 0));
			descubiertas.insert(std::pair<unsigned int, unsigned int>(pt, 0));
		}

		totales[pt]++;
		Info_sala datos(pid, pt);
		diccionario.insert(std::pair<unsigned int, Info_sala>(pid, datos));
	}
}

unsigned int Control_salas::obtener_total_salas_por_tipo(unsigned int pt) const
{
	if(!totales.count(pt)) return 0;
	else return totales.at(pt);
}

unsigned int Control_salas::obtener_total_salas_descubiertas_por_tipo(unsigned int pt) const
{
	if(!descubiertas.count(pt)) return 0;
	else return descubiertas.at(pt);	
}

void Control_salas::reiniciar()
{
	descubiertas.clear();
	totales.clear();
	diccionario.clear();
}

std::vector<unsigned int> Control_salas::obtener_vector_salas_descubiertas() const
{
	std::vector<unsigned int> resultado;

	for(const std::pair<const unsigned int, const Info_sala> par : diccionario)
	{
		if(par.second.descubierta) resultado.push_back(par.second.id);
	}

	return resultado;
}

std::vector<unsigned int> Control_salas::obtener_vector_salas_con_gemas() const
{
	std::vector<unsigned int> resultado;

	for(const std::pair<const unsigned int, const Info_sala> par : diccionario)
	{
		if(par.second.con_gemas) resultado.push_back(par.second.id);
	}

	return resultado;
}
