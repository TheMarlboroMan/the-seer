#ifndef CONTROL_TEMPORIZADOR_H
#define CONTROL_TEMPORIZADOR_H

/*Controla cadenas de temporizadores... Residen en un array del controlador.*/

#include "datos_legibles.h"

class Control_temporizador
{
	private:

	float tiempo;	//Tiempo actual.
	float tiempo_maximo;	//Tiempo que debe alcanzar...
	unsigned int cadena_actual; //Cadena que se ejecutará cuando lleguemos al tiempo máximo.
	unsigned int siguiente_temporizador; //Siguiente entrada de temporizador...
	/*bool encendido;*/

	public:

	Control_temporizador();
	Control_temporizador(const Datos_temporizador& p_dt/*, bool p_encendido*/);
	bool procesar_turno(float p_delta);
	void actualizar_informacion(const Datos_temporizador& p_dt);

	unsigned int acc_cadena_actual() const {return cadena_actual;}
	unsigned int acc_siguiente_temporizador() const {return siguiente_temporizador;}
	/*bool es_encendido() const {return encendido;}*/
	bool es_fin_de_cadena() const {return siguiente_temporizador==0;}
};

#endif
