#include "disparador_proyectiles.h"

Disparador_proyectiles::Disparador_proyectiles(
		unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int pt, unsigned int pp, unsigned int pd, 
		unsigned int pvel, unsigned int pid_enl, unsigned int pact,
		bool pactivo, unsigned int pid)
	:Actor(px, py, pz), Activable(pactivo), tipo_proyectil(pt), patron(pp),
	direccion(pd), velocidad(pvel), id_enlace_activacion(pid_enl),
	paso_actual(pact), total_pasos(0), 
	estado_paso_actual(E_ESPERA), tiempo_paso_actual(0.0), tiempo_max_paso(0.0) 
{
	actualizar_volumen();
	cargar_patron_pasos();
	actualizar_id_localizador(pid);
}

Disparador_proyectiles::~Disparador_proyectiles()
{}

void Disparador_proyectiles::cargar_patron_pasos()
{
	const Datos_patron_disparo& dp=Controlador_patrones_disparo::controlador_patrones_disparo.obtener_por_id(patron);
//	paso_actual=0;
	total_pasos=dp.obtener_total_pasos();
	cargar_info_paso();
}

void Disparador_proyectiles::cargar_info_paso()
{
	const Datos_patron_disparo& dp=Controlador_patrones_disparo::controlador_patrones_disparo.obtener_por_id(patron);
	const Info_patron_disparo& d=dp.obtener_paso(paso_actual);
	estado_paso_actual=d.estado;
	tiempo_max_paso=float(d.pausa_ms) / 1000.0;
}

void Disparador_proyectiles::turno(float p_delta)
{
	if(!es_activo()) return;

	tiempo_paso_actual+=p_delta;

	if(tiempo_paso_actual > tiempo_max_paso)
	{
		tiempo_paso_actual=0.0;

		++paso_actual;
		if(paso_actual >= total_pasos) paso_actual=0;
		cargar_info_paso();
	}
}

Actor * Disparador_proyectiles::generar_proyectil()
{
	Actor * resultado=new Proyectil_obstaculo(acc_x(), acc_y(), acc_z(), tipo_proyectil, direccion, velocidad);

	if(estado_paso_actual==E_DISPARO)
	{
		estado_paso_actual=E_ESPERA;
	}

	return resultado;
}
