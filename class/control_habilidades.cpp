#include "control_habilidades.h"
#include <iostream>

const float Control_habilidades::CONSUMO_SEGUNDO_RESISTENCIA_LAVA=50.0;
const float Control_habilidades::CONSUMO_SEGUNDO_RESISTENCIA_AGUA=50.0;
const float Control_habilidades::CONSUMO_SEGUNDO_RESISTENCIA_DARDO=80.0;
const float Control_habilidades::SEGUNDOS_UTIL_PARAR_TIEMPO=3.0; 
const float Control_habilidades::SEGUNDOS_UTIL_INSPECTOR_SECRETOS=10.0;

Control_habilidades::Control_habilidades():
	habilidades_activadas(false),
	indice_habilidad(0), tiempo_parando_tiempo(0.0), 
	tiempo_inspeccionando_secretos(0.0), 
	reanudar_sonido(false)
{
	inicializar();
}

void Control_habilidades::inicializar()
{
	habilidades_activadas=false;
	unsigned int i=0;

	while(i < H_MAX)
	{
		habilidades[i]=false;
		++i;
	}
}

/*Escoge la siguiente habilidad disponible o vuelve a la que tiene originalmente
si no encuentra ninguna.*/

void Control_habilidades::cambiar_habilidad(int p)
{
	int actual=indice_habilidad;
	int nindice=indice_habilidad;

	//Iteramos sin parar...
	while(true)
	{
		nindice+=p;

		if(nindice==actual) break; //Vuelta completa
		else if(nindice < 0) nindice=H_MAX;
		else if(nindice >= H_MAX) nindice=-1; //Para que luego se ponga en 0.

		if(habilidades[nindice])
		{
			indice_habilidad=nindice;
			break;
		}
	}
}

bool Control_habilidades::es_habilidad_usable(unsigned int p) const
{
	if(habilidades_activadas && habilidades.count(p))
	{
		return habilidades.at(p);
	}
	else 
	{
		return false;
	}
}

bool Control_habilidades::es_habilidad_concedida(unsigned int p) const
{
	if(habilidades.count(p))
	{
		return habilidades.at(p);
	}
	else 
	{
		return false;
	}
}

void Control_habilidades::conceder_habilidad(unsigned int p)
{
	if(habilidades.count(p)) 
	{
		habilidades[p]=true;
		indice_habilidad=p;
	}
}

void Control_habilidades::retirar_habilidad(unsigned int p)
{
	if(habilidades.count(p)) habilidades[p]=false;
}

void Control_habilidades::turno(float delta)
{
	reanudar_sonido=false;

	auto sumar_tiempo=[](float &t, float max, float delta) -> bool
	{
		if(t)
		{
			t+=delta;
			if(t >= max) 
			{
				t=0.0;
				return true;
			}
		}

		return false;
	};

	if(sumar_tiempo(tiempo_parando_tiempo, SEGUNDOS_UTIL_PARAR_TIEMPO, delta))
	{
		reanudar_sonido=true;
	}
	sumar_tiempo(tiempo_inspeccionando_secretos, SEGUNDOS_UTIL_INSPECTOR_SECRETOS, delta);
}

void Control_habilidades::reiniciar()
{
	inicializar();
	desactivar_todo();
	indice_habilidad=0;
	habilidades_activadas=false;
}
