#ifndef INPUT_H
#define INPUT_H

#include <map>
#include "../libdan_headers/input/controles_sdl/controles_sdl.h"

/*Abstrae los sistemas de input usando la clase Controles_SDL. Cada input del
enum de turno se puede asignar a uno o más valores SDLK_xxx, según lo mismo
pueda hacerse con varias teclas, joystick o no. */

class Input
{
	////////////////////////
	//Definiciones....

	public:

	enum inputs{
I_ESCAPE=0,
I_DELETE,
I_PAUSA,
I_ARRIBA,
I_ABAJO,
I_IZQUIERDA,
I_DERECHA,
I_SALTAR,
I_INTERACCION,
I_CORRER,
I_HABILIDAD,
//I_HABILIDAD_SIGUIENTE,
//I_HABILIDAD_ANTERIOR,
I_MAPA
};

	typedef std::multimap<unsigned int, unsigned int> tipo_mapa;
	typedef std::multimap<unsigned int, unsigned int>::const_iterator tipo_iterador;

	private:

	typedef std::pair <tipo_iterador, tipo_iterador> tipo_par;

	////////////////////////
	//Propiedades.

	private:
	//std::multimap<unsigned int, unsigned int> 
	tipo_mapa mapa;
	DLibI::Controles_SDL controles_sdl;

	////////////////////////
	//Métodos

	private:

	tipo_par obtener(unsigned int) const;

	public:

	void configurar();
	void turno();

	bool es_senal_salida() const;
	bool es_input_down(unsigned int) const;
	bool es_input_up(unsigned int) const;
	bool es_input_pulsado(unsigned int) const;

	bool es_tecla_down(unsigned int i) const {return controles_sdl.es_tecla_down(i);}
	bool es_tecla_up(unsigned int i) const {return controles_sdl.es_tecla_up(i);}	
	bool es_tecla_pulsada(unsigned int i) const {return controles_sdl.es_tecla_pulsada(i);}

	bool hay_eventos_teclado_down() const {return controles_sdl.recibe_eventos_teclado_down();}

	DLibI::Controles_SDL& acc_controles_sdl() {return controles_sdl;}

	Input() {}
};

#endif
