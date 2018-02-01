#ifndef CONTROLADOR_CONFIRMACION_H
#define CONTROLADOR_CONFIRMACION_H

#include "controlador_info_juego.h"

class Controlador_confirmacion:public Controlador_info_juego
{
	private:

	bool abandonar_juego;
	virtual bool procesar_input(const Input&);

	public:

	void preparar();
	bool es_abandonar_juego() const {return abandonar_juego;}

	Controlador_confirmacion(Datos_sistema&, const Input&);
};

#endif
