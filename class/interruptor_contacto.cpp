#include "interruptor_contacto.h"

Interruptor_contacto::Interruptor_contacto(int px, int py, int pz, int pw, int ph, int pd, 
	unsigned int pidc_in, unsigned int pidc_out, bool pr, bool pa,
	unsigned int pid, unsigned int pcuin, unsigned int pcuout):
	Actor(px, py, pz), Activable(pa), w(pw), h(ph), d(pd), 
	id_cadena_in(pidc_in), id_cadena_out(pidc_out), reusable(pr),
	estado(E_OUT), cuenta_in(pcuin), cuenta_out(pcuout), colision_este_turno(false)
{
	actualizar_volumen();
	actualizar_id_localizador(pid);
}

Interruptor_contacto::~Interruptor_contacto()
{

}

void Interruptor_contacto::actualizar_volumen()
{
	establecer_volumen(w, d, h);
}

unsigned int Interruptor_contacto::obtener_id_activo() const
{
	if(!es_activo()) return 0;
	else switch(estado)
	{
		case E_RECIEN_IN: 
			if(!reusable && cuenta_in) return 0;
			else
			{
				++cuenta_in;
				return id_cadena_in; 
			}
		break;
		case E_RECIEN_OUT: 
			if(!reusable && cuenta_out) return 0;
			else
			{
				++cuenta_out;
				return id_cadena_out;
			}
		break;
		default: return 0; break;
	}
}

//Este es el que ocurre antes de marcar como colisión porque el 
//visitante se ejecuta primero.

void Interruptor_contacto::actualizar_estado(/*bool col*/)
{
	if(colision_este_turno)
	{
		if(estado==E_RECIEN_IN) estado=E_IN;
		else if(estado!=E_IN) estado=E_RECIEN_IN;
	}
	else
	{
		if(estado==E_RECIEN_OUT) estado=E_OUT;
		else if(estado!=E_OUT) estado=E_RECIEN_OUT;
	}

//std::cout<<"estado "<<estado<<std::endl;

	//Reset de bandera.
	colision_este_turno=false;
}

//Este ocurre después de actualizar estado.

void Interruptor_contacto::marcar_como_colision()
{
	colision_este_turno=true;
}
