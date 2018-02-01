#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <exception>

/*
find ./ -name '*~' -exec rm '{}' \; -print -or -name ".*~" -exec rm {} \; -print
*/

#include "libdan_headers/libDanSDL.h"
#include "libdan_headers/defDanSDL.h"
#include "class/sistema.h"

DLibH::Log_base LOG;
const float Definiciones::MAX_DELTA=0.030;

int main(int argc, char ** argv)
{
	if(DLibH::Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK))
	{
		LOG.inicializar("data/logs/info.log");
		LOG.activar();
		DLibH::Log_motor::arrancar("data/logs/log_motor.log");
		srand(time(NULL));

		Sistema sistema(argc, argv);
		sistema.iniciar();
	}
	
	DLibH::Log_motor::finalizar();
	DLibH::Herramientas_SDL::apagar_SDL();

	return 0;
}
