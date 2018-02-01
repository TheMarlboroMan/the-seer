#include "controlador_info_juego.h"

const std::string Controlador_info_juego::RUTA_LOCALIZACION="data/localizacion/info";

Controlador_info_juego::Controlador_info_juego(Datos_sistema& ds, const Input& i):
	datos_sistema(ds),
	input(i),
	loc(RUTA_LOCALIZACION),
	caja_fondo(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(
			X_CAJA+BORDE_CAJA, Y_CAJA+BORDE_CAJA,
			W_CAJA-(2*BORDE_CAJA), H_CAJA-(2*BORDE_CAJA)),
		DLibV::Gestor_color::color(0,0,0)),
	caja_borde(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA, Y_CAJA, W_CAJA, H_CAJA),
		DLibV::Gestor_color::color(255,255,255)),
	txt_texto(DLibV::Gestor_recursos_graficos::obtener(3), ""),
	montado(false)
{
	loc.inicializar(ds.acc_id_idioma());
	txt_texto.establecer_posicion(X_TEXTO, Y_TEXTO);
	txt_texto.mut_interlineado(4);
}

void Controlador_info_juego::preparar(unsigned int i)
{
	//Este es el texto que vamos a mostrar.
	unsigned int indice_texto=Diccionario_info_juego::obtener_indice_texto(i);
	txt_texto.asignar(loc.obtener(indice_texto));
	montado=true;
}

void Controlador_info_juego::desmontar()
{
	montado=false;
}

bool Controlador_info_juego::procesar_input(const Input& input)
{
	bool resultado=true;

	if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE))
	{
		datos_sistema.establecer_en_menu(true);
	}
	else if(
		input.es_input_down(Input::I_SALTAR) || 
		input.es_input_down(Input::I_INTERACCION))
	{
		resultado=false;
	}

	return resultado;
}

bool Controlador_info_juego::loop(float delta, DLibV::Pantalla& p)
{
	bool resultado=procesar_input(input);
	dibujar(p);
	return resultado;
}

void Controlador_info_juego::dibujar(DLibV::Pantalla& p)
{
	caja_borde.volcar(p);
	caja_fondo.volcar(p);

	txt_texto.volcar(p);
}
