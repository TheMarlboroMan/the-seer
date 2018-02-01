#include "control_objetivos.h"

Control_objetivos::Control_objetivos()
{
	preparar();
}

Control_objetivos::~Control_objetivos()
{

}

void Control_objetivos::preparar()
{
	unsigned int i=0;

	while(i < T_MAX)
	{
		totales.insert(std::pair<unsigned int, unsigned int>(i, 0));
		contados.insert(std::pair<unsigned int, unsigned int>(i, 0));
		++i;
	}

	i=0;

	while(i < M_MAX)
	{
		mensajes_mostrados.insert(std::pair<unsigned int, bool>(i, false));
		++i;
	}
}

unsigned int Control_objetivos::obtener_total(unsigned int pt) const
{
	if(!totales.count(pt)) return 0;
	else return totales.at(pt);
}

unsigned int Control_objetivos::obtener_contados(unsigned int pt) const
{
	if(!contados.count(pt)) return 0;
	else return contados.at(pt);
}

void Control_objetivos::sumar_total(unsigned int pt, unsigned int pc)
{
	totales[pt]+=pc;
}

void Control_objetivos::contar(unsigned int pt, unsigned int pc)
{
	contados[pt]+=pc;
}

void Control_objetivos::marcar_mensaje_mostrado(unsigned int i)
{
	if(mensajes_mostrados.count(i)) mensajes_mostrados[i]=true;
}

void Control_objetivos::desmarcar_mensaje_mostrado(unsigned int i)
{
	if(mensajes_mostrados.count(i)) mensajes_mostrados[i]=false;
}

bool Control_objetivos::es_mensaje_mostrado(unsigned int i) const
{
	if(mensajes_mostrados.count(i)) return mensajes_mostrados.at(i);
	else return false;
}

void Control_objetivos::reiniciar()
{
	totales.clear();
	contados.clear();
	mensajes_mostrados.clear();
	preparar();
}
