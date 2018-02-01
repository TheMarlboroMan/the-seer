#ifndef CONTROLADOR_AUXILIAR_H
#define CONTROLADOR_AUXILIAR_H

#include "input.h"
#include "controlador_mapa.h"
#include "controlador_status.h"
#include "control_habilidades.h"

/*Este controla la pantalla conjunta de mapa e inventario.
Las dos se muestran a la vez, pero tienen controladores 
diferentes.*/

class Controlador_auxiliar
{
	private:

	static Uint32 COLOR_FONDO_PANTALLA;

	//Referencias prestadas.
	Datos_sistema& datos_sistema;
	DLibV::Pantalla &pantalla;
	const Input& input;
	//DLibI::Controles_SDL &controles_sdl;
	//DLibH::Controlador_fps_SDL &control_frames;
	const Control_salas& control_salas;
	Control_habilidades& control_habilidades;
	const int W_PANTALLA;
	const int H_PANTALLA;

	//Información propia...
	Controlador_mapa controlador_mapa;
	Controlador_status controlador_status;

	bool montado;
	
	private:

	//Métodos...
	
	bool procesar_controles();
	void dibujar(float);
	void calcular_habilidad_activa();

	public:

	Controlador_auxiliar(Datos_sistema&, DLibV::Pantalla&, 
	//DLibI::Controles_SDL&, /*DLibH::Controlador_fps_SDL&,*/ 
	const Input&,
	const Control_salas&, 
	Control_habilidades&,
	const Info_juego&, const Control_objetivos&, 
	int, int);
	bool loop(float);
	void inicializar(const std::string&, unsigned int, const std::vector<unsigned int>&);
	void desmontar();
	bool es_inicializado() const {return montado;}
};

#endif
