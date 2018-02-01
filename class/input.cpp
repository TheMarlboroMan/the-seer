#include "input.h"

void Input::configurar()
{
	mapa.insert(std::make_pair(I_ESCAPE, SDLK_ESCAPE));
	mapa.insert(std::make_pair(I_DELETE, SDLK_DELETE));
	mapa.insert(std::make_pair(I_PAUSA, SDLK_p));

	mapa.insert(std::make_pair(I_ARRIBA, SDLK_UP));
	mapa.insert(std::make_pair(I_ABAJO, SDLK_DOWN));
	mapa.insert(std::make_pair(I_IZQUIERDA, SDLK_LEFT));
	mapa.insert(std::make_pair(I_DERECHA, SDLK_RIGHT));

	mapa.insert(std::make_pair(I_SALTAR, SDLK_SPACE));

	mapa.insert(std::make_pair(I_INTERACCION, SDLK_RCTRL));
	mapa.insert(std::make_pair(I_INTERACCION, SDLK_LCTRL));

	mapa.insert(std::make_pair(I_CORRER, SDLK_RSHIFT));
	mapa.insert(std::make_pair(I_CORRER, SDLK_LSHIFT));
	
	mapa.insert(std::make_pair(I_HABILIDAD, SDLK_LALT));
	mapa.insert(std::make_pair(I_HABILIDAD, SDLK_RALT));
//	mapa.insert(std::make_pair(I_HABILIDAD, SDLK_0));

//	mapa.insert(std::make_pair(I_HABILIDAD_SIGUIENTE, SDLK_9));
//	mapa.insert(std::make_pair(I_HABILIDAD_ANTERIOR, SDLK_8));
	mapa.insert(std::make_pair(I_MAPA, SDLK_TAB));
}

void Input::turno()
{
	controles_sdl.recoger();
}

Input::tipo_par Input::obtener(unsigned int i) const
{
	tipo_par resultado=mapa.equal_range(i);
	return resultado;
}

bool Input::es_senal_salida() const
{
	return controles_sdl.es_senal_salida();
}

bool Input::es_input_down(unsigned int i) const
{
	tipo_par resultado=obtener(i);
	for(tipo_mapa::const_iterator it=resultado.first; it!=resultado.second; ++it)
	{
		//std::cout<<it->first<<" "<<it->second<<std::endl;
		if(controles_sdl.es_tecla_down(it->second)) return true;
	}


	return false;
}

bool Input::es_input_up(unsigned int i) const
{
	tipo_par resultado=obtener(i);
	for(tipo_mapa::const_iterator it=resultado.first; it!=resultado.second; ++it)
		if(controles_sdl.es_tecla_up(it->second)) return true;

	return false;
}

bool Input::es_input_pulsado(unsigned int i) const
{
	tipo_par resultado=obtener(i);
	for(tipo_mapa::const_iterator it=resultado.first; it!=resultado.second; ++it)
		if(controles_sdl.es_tecla_pulsada(it->second)) return true;

	return false;
}
