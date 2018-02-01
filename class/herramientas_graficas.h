#ifndef HERRAMIENTAS_GRAFICAS_H
#define HERRAMIENTAS_GRAFICAS_H

#include "../libdan_headers/libDanSDL.h"

class Herramientas_graficas
{
	public:

	static SDL_Surface * generar_superficie_alterando_color(
		SDL_Surface * original, 
		SDL_Rect caja_recorte, 
		Uint32 color_original, Uint32 nuevo_color);

	static SDL_Surface * pantallazo_alterando_canales(
		DLibV::Pantalla&, int, int, int);

	static SDL_Surface * ampliar_superficie(
		SDL_Surface * , unsigned int,
		const SDL_Rect&);

	static SDL_Surface * rotar_superficie_90_derecha(SDL_Surface *, const SDL_Rect&);
	static SDL_Surface * rotar_superficie_90_derecha(SDL_Surface *);
};

#endif
