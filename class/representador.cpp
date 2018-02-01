#include "representador.h"

Representador::Representador() 
{

}

Representador::~Representador()
{

}

/*
bool Representador::representar(
	DLibV::Pantalla& pantalla, 
	DLibV::Camara& camara, 
	Representable * r, 
	Bloque_transformacion_representable& temp,
	float offset_x,
	float offset_y)
{
	r->transformar_representacion(temp);

	int posx=temp.rep.acc_posicion().x+offset_x;
	int posy=temp.rep.acc_posicion().y+offset_y;
	temp.rep.establecer_posicion(posx, posy);
	return temp.rep->volcar(pantalla, camara);
}*/

unsigned int Representador::generar_vista(
	DLibV::Pantalla& pantalla, 
	DLibV::Camara& camara, 
	std::vector<Representable *>& v, 
	float offset_x,
	float offset_y,
	Bloque_transformacion_representable& temp)
{
	unsigned int total=0;

	//Y ahora a dibujar...
	
	for(Representable * r : v)
	{
		r->transformar_representacion(temp);
		int posx=temp.rep->acc_posicion().x+offset_x;
		int posy=temp.rep->acc_posicion().y+offset_y;
		temp.rep->establecer_posicion(posx, posy);
		if(temp.rep->volcar(pantalla, camara)) ++total;

		//total+=representar(pantalla, camara, r, temp, offset_x, offset_y);
	}

	return total;
}

unsigned int Representador::generar_overlays(
	DLibV::Pantalla& pantalla, 
	DLibV::Camara& camara, 
	std::vector<Representable_overlay *>& v, 
	float offset_x,
	float offset_y,
	Bloque_transformacion_representable& temp)
{
	unsigned int total=0;

	for(Representable_overlay * r : v)
	{
		r->transformar_overlay(temp);
		int posx=temp.rep->acc_posicion().x+offset_x;
		int posy=temp.rep->acc_posicion().y+offset_y;
		temp.rep->establecer_posicion(posx, posy);
		if(temp.rep->volcar(pantalla, camara)) ++total;
	}

	return total;
}


void Representador::generar_icono_secreto(DLibV::Pantalla& pantalla, bool hay_secreto)
{
	DLibV::Representacion_bitmap_dinamica rep;

	rep.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(8));
	rep.establecer_recorte(hay_secreto ? X_ICONO_SECRETO_PRESENTE : X_ICONO_SECRETO_AUSENTE, Y_ICONO_SECRETO, W_ICONO_SECRETO, H_ICONO_SECRETO);
	rep.establecer_posicion(X_SECRETO, Y_SECRETO);
	rep.volcar(pantalla);
}

void Representador::generar_texto_debug(DLibV::Pantalla& pantalla, const std::string& texto)
{
	DLibV::Representacion_texto_fija_estatica txt(DLibV::Gestor_recursos_graficos::obtener(3), texto, 600, 20);
	txt.establecer_posicion(6, 380);
	txt.mut_interletra(1);
	txt.volcar(pantalla);
}

void Representador::generar_texto_nombre_sala(DLibV::Pantalla& pantalla, const DLibH::t_cadena& texto)
{
	DLibV::Representacion_texto_fija_estatica txt(DLibV::Gestor_recursos_graficos::obtener(3), texto, 600, 20);
	txt.establecer_posicion(6, 38);
	txt.mut_interletra(1);
	txt.volcar(pantalla);
}

void Representador::generar_fondo_info_tanque(DLibV::Pantalla &pantalla)
{
	DLibV::Representacion_bitmap_dinamica rep;
	rep.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(8));
	rep.establecer_recorte(X_RECORTE_FONDO_TANQUE, Y_RECORTE_FONDO_TANQUE, W_RECORTE_FONDO_TANQUE, H_RECORTE_FONDO_TANQUE);
	rep.establecer_posicion(X_FONDO_TANQUE, Y_FONDO_TANQUE);
	rep.volcar(pantalla);
}

//La cantidad se expresa de 0 a 100..
void Representador::generar_info_tanque(DLibV::Pantalla& pantalla, float cantidad, bool usable, unsigned int indice)
{
	SDL_Rect r=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);
	int x=0, y=0;

	switch(indice)
	{
		case 0: r.x=72; r.y=22; r.w=4; r.h=6; x=0; y=0; break;
		case 1: r.x=77; r.y=22; r.w=3; r.h=3; x=5; y=0; break;
		case 2: r.x=81; r.y=22;	r.w=3; r.h=4; x=9; y=0; break;
		case 3:	r.x=80; r.y=27; r.w=4; r.h=3; x=8; y=5; break;
		case 4: r.x=80; r.y=31; r.w=4; r.h=3; x=8; y=9; break;
		case 5:	r.x=76; r.y=31; r.w=3; r.h=3; x=4; y=9; break;
		case 6: r.x=72; r.y=29; r.w=3; r.h=5; x=0; y=7; break;
		case 7: r.x=76; r.y=26;	r.w=3; r.h=4; x=4; y=4; break;
	}

	if(r.w && r.h)
	{	
		if(usable) r.x+=12;
		unsigned int alpha=(cantidad * 255) / 100;
		if(alpha==0) alpha=1;

		y+=Y_FONDO_TANQUE+2;
		x+=X_FONDO_TANQUE+2;

		DLibV::Representacion_bitmap_dinamica rep;
		rep.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(8));
		rep.establecer_recorte(r);
		rep.establecer_posicion(x, y);
		rep.establecer_alpha(alpha);
		rep.volcar(pantalla);
		
/*
	Uint32 color=usable ? DLibV::Gestor_color::color(0, 0, 255) : DLibV::Gestor_color::color(255, 0, 0);
	unsigned int alpha=(cantidad * 255) / 100;
	int x=X_TANQUES+( (W_TANQUE+W_MARGEN_TANQUE) * indice);

	DLibV::Primitiva_grafica_caja_estatica caja(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(x, Y_TANQUES, W_TANQUE, H_TANQUE),
		color);
	caja.establecer_alpha(alpha);
	caja.volcar(pantalla);
*/
	}
}

void Representador::generar_info_habilidad(DLibV::Pantalla& pantalla, unsigned int indice)
{
	unsigned int x=(indice * W_HABILIDAD) + X_RECORTE_HABILIDAD;

	DLibV::Representacion_bitmap_dinamica rep;
	rep.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(8));
	rep.establecer_recorte(x, Y_RECORTE_HABILIDAD, W_HABILIDAD, H_HABILIDAD);
	rep.establecer_posicion(X_HABILIDAD, Y_HABILIDAD);
	rep.volcar(pantalla);
}
