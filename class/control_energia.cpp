#include "control_energia.h"

const float Control_energia::Tanque::CANTIDAD_RECUPERADA_POR_SEGUNDO=10.0;
const float Control_energia::Tanque::SEGUNDOS_INTERMEDIO=0.8;
const float Control_energia::Tanque::MAX_CANTIDAD=100.0;

////////////////////
// Tanques.

void Control_energia::Tanque::turno(float delta, bool recuperacion_rapida)
{
	switch(estado)
	{
		case E_DISPONIBLE: //No se hace nada de nada.

		break;

		case E_CONSUMO:
			cantidad-=consumo * delta;
			if(cantidad <= 0.0)
			{
				cantidad=0.0;
				estado=E_ENFRIAMIENTO;
			}
		break;
/*		
		case E_INTERMEDIO:
			tiempo+=delta;
			if(tiempo >= SEGUNDOS_INTERMEDIO)
			{
				tiempo=0.0;
				estado=E_RECARGA;
			}
		break;
*/

		case E_ENFRIAMIENTO:
			tiempo+=delta;
			if(tiempo >= SEGUNDOS_INTERMEDIO)
			{
				tiempo=0.0;
				estado=E_RECARGA;
			}
		break;

		case E_RECARGA: //Se suma la cantidad de energía de turno.
		{
			float c=CANTIDAD_RECUPERADA_POR_SEGUNDO * delta;
			if(recuperacion_rapida) c*=2.0;
			
			cantidad+=c;
			if(cantidad >= MAX_CANTIDAD)
			{
				cantidad=MAX_CANTIDAD;
				estado=E_DISPONIBLE;
			}
		}
		break;
	}
}

/*Un tanque que se vacíe del todo pasa al estado de recarga cuando haya pasado
el tiempo de turno.*/

void Control_energia::Tanque::vaciar()
{
	tiempo=0.0;
	cantidad=0;
	estado=E_ENFRIAMIENTO; //E_INTERMEDIO; //E_RECARGA;
}

/*Al iniciar el consumo se reinicia el contador interno de tiempo. Este contador
se usa para ver cuando se empieza a recargar sólo.*/

/*
void Control_energia::Tanque::consumir(float c)
{
	cantidad-=c;
	tiempo=0.0;

	if(cantidad <=0.0) 
	{
		cantidad=0.0;
		estado=E_RECARGA;
	}
	else
	{
		estado=E_INTERMEDIO;
	}
}
*/

/////////////////////
//Control de energía

Control_energia::Control_energia()
{

}

/*
bool Control_energia::hay_reserva(float c) const
{
	for(const Tanque& t : tanques)
	{
		if(t.hay_reserva(c) && t.es_usable())
		{
			return true;
		}
	}

	return false;
}
*/

bool Control_energia::hay_tanques_completos(unsigned int t) const
{
	if(tanques.size() < t) return false;

	unsigned int r=0;

	for(const Tanque& t : tanques)
	{
		if(t.es_lleno()) ++r;
	}

	return r >= t;
}

void Control_energia::gastar_completo(unsigned int c)
{
	unsigned int r=0;

	for(Tanque& t : tanques)
	{
		if(t.es_lleno() && r < c) 
		{
			t.vaciar();
			++r;
		}
	}
}

/*
bool Control_energia::gastar(float c)
{
	for(Tanque& t : tanques)
	{
		//Si no queda suficiente cantidad en la reserva
		//ese tanque se queda con lo que tuviera, que
		//no será mucho.
		if(t.hay_reserva(c) && t.es_usable())
		{
			t.consumir(c);
			return true;
		}
	}

	return false;
}
*/

void Control_energia::rellenar()
{
	for(Tanque& t : tanques) t.rellenar();
}

void Control_energia::nuevo_tanque()
{
	tanques.push_back(Control_energia::Tanque());
}

void Control_energia::turno(float delta, bool recuperacion_rapida)
{
	for(Tanque& t : tanques) t.turno(delta, recuperacion_rapida);
}

std::vector<Control_energia::Info_tanque> Control_energia::obtener_info_tanques() const
{
	std::vector<Control_energia::Info_tanque> resultado;
	for(const Tanque& t : tanques) resultado.push_back(t.como_info_tanque());
	return resultado;
}

/*
float Control_energia::calcular_ratio_gasto(float delta, float cantidad)
{
	return cantidad * delta;
}
*/


void Control_energia::iniciar_gasto(float consumo)
{
	for(Tanque& t : tanques)
	{
		if(t.es_lleno()) 
		{
			t.iniciar_consumo(consumo);
			return;
		}
	}
}

bool Control_energia::es_consumiendo() const
{
	for(const Tanque& t : tanques)
	{
		if(t.es_consumiendo()) return true;
	}
	return false;
}

void Control_energia::reiniciar()
{
	tanques.clear();
}
