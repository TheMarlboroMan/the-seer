#include "controlador_auxiliar.h"

Uint32 Controlador_auxiliar::COLOR_FONDO_PANTALLA=0; //DLibV::Gestor_color::color(0, 0, 0);

Controlador_auxiliar::Controlador_auxiliar(
	Datos_sistema& pds, DLibV::Pantalla& pp,
	const Input& i,
//	DLibI::Controles_SDL& pc, 
/*DLibH::Controlador_fps_SDL& pcf, */
	const Control_salas& pcs,
	Control_habilidades& pch,
	const Info_juego& info_juego,
	const Control_objetivos& control_objetivos,
	int pw, int ph):
	datos_sistema(pds), pantalla(pp),
	input(i),
//	controles_sdl(pc), 
//control_frames(pcf),
	control_salas(pcs), control_habilidades(pch),
	W_PANTALLA(pw), H_PANTALLA(ph),
	controlador_mapa(datos_sistema, control_salas),
	controlador_status(datos_sistema, info_juego, control_salas, control_objetivos),
	montado(false)
{
	COLOR_FONDO_PANTALLA=DLibV::Gestor_color::color(0, 0, 0);
}

void Controlador_auxiliar::inicializar(
	const std::string& t_restante, 
	unsigned int id_sala_actual,
	const std::vector<unsigned int>& v)
{
	controlador_status.inicializar(t_restante, v);
	
	calcular_habilidad_activa();
	controlador_mapa.configurar(id_sala_actual);

	montado=true;
}

void Controlador_auxiliar::calcular_habilidad_activa()
{
	unsigned int activa=control_habilidades.acc_habilidad_activa();
	int indice=control_habilidades.es_habilidad_usable(activa) ? activa : -1;
	controlador_status.calcular_datos_habilidad(indice);
}	

void Controlador_auxiliar::desmontar()
{
	montado=false;
}

bool Controlador_auxiliar::loop(float delta)
{
	bool resultado=true;

	resultado=procesar_controles();

	controlador_status.loop(delta);
	controlador_mapa.loop(delta);
	dibujar(delta);

	return resultado;
}

void Controlador_auxiliar::dibujar(float delta)
{
	pantalla.rellenar(COLOR_FONDO_PANTALLA);
	controlador_status.dibujar(pantalla, delta);
	controlador_mapa.dibujar(pantalla, delta);
}

bool Controlador_auxiliar::procesar_controles()
{
	bool resultado=true;

	if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE)) datos_sistema.establecer_en_menu(true);
	else if(input.es_input_down(Input::I_MAPA)) resultado=false;
	else if(input.es_input_down(Input::I_IZQUIERDA)) controlador_mapa.escoger_zona(-1);
	else if(input.es_input_down(Input::I_DERECHA)) controlador_mapa.escoger_zona(1);
	else if(input.es_input_down(Input::I_ARRIBA)) 
	{
		control_habilidades.habilidad_anterior();
		calcular_habilidad_activa();
	}
	else if(input.es_input_down(Input::I_ABAJO)) 
	{
		control_habilidades.habilidad_siguiente();
		calcular_habilidad_activa();
	}

	return resultado;
}
