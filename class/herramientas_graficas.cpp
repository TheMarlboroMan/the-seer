#include "herramientas_graficas.h"

/*Pasando las dos superficies, la segunda será una copia de la primera
con los colores alterados. No hace ningún tipo de trabajo con la 
memoria de modo que asumimos que toda la gestión se hará fuera. 

Se asume que la nueva viene con un NULL como un castillo, ya que
el método para copiar_superificie devuelve una nueva.*/

SDL_Surface * Herramientas_graficas::generar_superficie_alterando_color(
	SDL_Surface * original, 
	SDL_Rect caja_recorte, 
	Uint32 color_original, Uint32 nuevo_color) 
{
	//De la original, recortamos el trozo deseado...
	SDL_Surface * nueva=DLibV::Utilidades_graficas_SDL::copiar_superficie(
			original,
			caja_recorte);

	DLibV::Proceso_superficie_cambiar_colores * pr=DLibV::Proceso_superficie_cambiar_colores::generar(color_original, nuevo_color);
	pr->actuar(nueva);
	delete pr;
	
	return nueva;
}

SDL_Surface * Herramientas_graficas::pantallazo_alterando_canales(
		DLibV::Pantalla& p, int r, int g, int b)
{
	SDL_Surface * superficie=DLibV::Utilidades_graficas_SDL::copiar_superficie(p.acc_superficie());
	DLibV::Proceso_superficie_alterar_canales_rgb * proc=DLibV::Proceso_superficie_alterar_canales_rgb::generar(r, g, b, false);
	proc->actuar(superficie);
	delete proc;

	return superficie;
}

SDL_Surface * Herramientas_graficas::ampliar_superficie(
		SDL_Surface * original, unsigned int factor,
		const SDL_Rect& caja)
{
	//Este es el resultado de ampliar.
	int W_IMG=caja.w;
	int H_IMG=caja.h;

	SDL_Rect caja_ampliada=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, W_IMG*factor, H_IMG*factor);
	SDL_Surface * resultado=DLibV::Utilidades_graficas_SDL::nueva_superficie_formato(original, caja_ampliada);

	//Y este es el proceso de ampliación.
	unsigned int px=0, py=0;
	unsigned int lim_x=caja.x+caja.w;
	unsigned int lim_y=caja.y+caja.h;

	unsigned int act_x=0, act_y=0;

	for(px=caja.x; px < lim_x; ++px)
	{
		act_y=0;

		for(py=caja.y; py < lim_y; ++py)
		{
			Uint32 pixel=DLibV::Utilidades_graficas_SDL::SDL_GetPixel(original, px, py);

			for(unsigned int rx=0; rx < factor; ++rx)
				for(unsigned int ry=0; ry < factor; ++ry)
				{
					DLibV::Utilidades_graficas_SDL::SDL_PutPixel(resultado, act_x+rx, act_y+ry, pixel);
				}

			act_y+=factor;
		}

		act_x+=factor;
	}
	
	return resultado;
}

SDL_Surface * Herramientas_graficas::rotar_superficie_90_derecha(SDL_Surface * original, const SDL_Rect& caja)
{
	SDL_Surface * resultado=DLibV::Utilidades_graficas_SDL::nueva_superficie_formato(original, caja);

	unsigned int lim_x=caja.x+caja.w;
	unsigned int lim_y=caja.y+caja.h;

	//Trazamos de fila por fila, y columna por columna.
	//Dibujamos desde el final.	
	unsigned int dest_x=caja.w-1;

	for(unsigned int y=caja.y; y < lim_y; ++y)
	{
		unsigned int dest_y=0;

		for(unsigned int x=caja.x; x < lim_x; ++x)
		{
			Uint32 pixel=DLibV::Utilidades_graficas_SDL::SDL_GetPixel(original, x, y);
			DLibV::Utilidades_graficas_SDL::SDL_PutPixel(resultado, dest_x, dest_y, pixel);
			++dest_y;
			
		}

		--dest_x;
	}

	return resultado;
}

SDL_Surface * Herramientas_graficas::rotar_superficie_90_derecha(SDL_Surface * original)
{
	SDL_Rect caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0, original->w, original->h);
	return rotar_superficie_90_derecha(original, caja);
}
