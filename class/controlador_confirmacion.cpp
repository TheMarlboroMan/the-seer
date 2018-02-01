#include "controlador_confirmacion.h"

Controlador_confirmacion::Controlador_confirmacion(
	Datos_sistema& ds, const Input& i):
		Controlador_info_juego(ds, i),
		abandonar_juego(false)
{

}

void Controlador_confirmacion::preparar()
{
	Controlador_info_juego::preparar(Diccionario_info_juego::D_CONFIRMACION_ABANDONAR_PARTIDA);
}

bool Controlador_confirmacion::procesar_input(const Input& input)
{
	bool resultado=true;

	if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE))
	{
		abandonar_juego=true;
		resultado=false;
	}
	else if(
		input.es_input_down(Input::I_SALTAR) || 
		input.es_input_down(Input::I_INTERACCION))
	{
		abandonar_juego=false;
		resultado=false;
	}

	return resultado;
}
