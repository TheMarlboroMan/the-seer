#include "cofre.h"

bool Cofre::habilitado_overlays=false;

Cofre::Cofre(unsigned int px, unsigned int py, unsigned int pz,
	unsigned int idp, bool pab)
	:Interactuable(px, py, pz), abierto(pab),
	id_pieza(idp)
{
	actualizar_volumen();
}

Cofre::~Cofre()
{}

void Cofre::transformar_representacion(Bloque_transformacion_representable& br)
{
	br.usar_bitmap();
	br.establecer_recurso_sin_recortar(2);
	Punto_2d p=obtener_coordenadas_origen_representacion();

	if(!abierto) 
	{
		br.establecer_recorte(0, 17, 16, 21);
		p.x-=8;
		p.y-=8;
		br.establecer_posicion(p.x, p.y, 16, 21);
	}
	else 
	{
		br.establecer_recorte(0, 38, 16, 23);
		p.x-=8;
		p.y-=8;
		br.establecer_posicion(p.x, p.y, 16, 23);
	}
}

void Cofre::transformar_overlay(Bloque_transformacion_representable& br)
{
	if(!abierto || !habilitado_overlays) 
	{
		br.usar_bitmap();
		br.establecer_recorte(0, 0, 0, 0);
		br.establecer_posicion(0, 0, 0, 0);
	}
	else 
	{
		br.usar_grupo();
	
		//El grupo recoge todo esto y lo limpiará el mismo...
		//No me gusta hacerlo así, pero es lo que tengo en el
		//motor, sin copias.
		
		DLibV::Representacion_bitmap_dinamica * marco=new DLibV::Representacion_bitmap_dinamica();
		marco->establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(2));
		marco->establecer_recorte(243, 0, 22, 29);
		marco->establecer_posicion(0, 0, 22, 29);
		marco->establecer_alpha(192);

		Definicion_pieza_puzzle d=Control_pieza_puzzle::obtener_definicion_pieza(id_pieza);
		int id_recurso=d.id_recurso;
		int x=d.recorte_x;
		int y=d.recorte_y;

		DLibV::Representacion_bitmap_dinamica * pieza=new DLibV::Representacion_bitmap_dinamica();
		pieza->establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(id_recurso));
		pieza->establecer_recorte(x, y, Control_pieza_puzzle::W_PIEZA, Control_pieza_puzzle::H_PIEZA);
		pieza->establecer_posicion(5, 4, 12, 11); 
		pieza->establecer_alpha(192);

		int xc=0;
		int yc=23;

		//TODO: 
		//Iluminación de la posición de turno.

/*		if(posicion_pieza < 5) yc=17;
		else if(posicion_pieza < 10) yc=20;

		switch(posicion_pieza % 5)
		{
			case 0: xc=4; break;
			case 1: xc=7; break;
			case 2: xc=10; break;
			case 3:	xc=13; break;
			case 4: xc=15; break;
		}

*/		SDL_Rect caja_pos=DLibH::Herramientas_SDL::nuevo_sdl_rect(xc, yc, 2, 2);
		DLibV::Primitiva_grafica_caja * posicion=new DLibV::Primitiva_grafica_caja_dinamica(caja_pos, DLibV::Gestor_color::color(49, 123, 244));
		posicion->establecer_alpha(192);

		br.insertar_en_grupo(marco);
		br.insertar_en_grupo(pieza);
		br.insertar_en_grupo(posicion);

		Punto_2d p=obtener_coordenadas_origen_representacion();

		//br.establecer_recorte(0, 38, 16, 23);
		p.x-=12;
		p.y-=40;
		br.establecer_posicion(p.x, p.y, 16, 23);
	}	
}
