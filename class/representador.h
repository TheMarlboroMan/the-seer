#ifndef REPRESENTADOR_H
#define REPRESENTADOR_H

#include <vector>
#include "representable.h"

class Representador
{
	private:

	//Constantes... 
	//Representación de los tanques de energía.
//	static const int X_TANQUES=20;
//	static const int Y_TANQUES=100;
//	static const int W_TANQUE=16;
//	static const int H_TANQUE=16;
//	static const int W_MARGEN_TANQUE=16;

	static const int X_RECORTE_FONDO_TANQUE=32;
	static const int Y_RECORTE_FONDO_TANQUE=64;
	static const int W_RECORTE_FONDO_TANQUE=16;
	static const int H_RECORTE_FONDO_TANQUE=16;
	static const int X_FONDO_TANQUE=40;
	static const int Y_FONDO_TANQUE=80;

	static const int W_HABILIDAD=16;
	static const int H_HABILIDAD=16;
	static const int X_RECORTE_HABILIDAD=0;
	static const int Y_RECORTE_HABILIDAD=48;
	static const int X_HABILIDAD=20;
	static const int Y_HABILIDAD=80;

	static const unsigned int W_ICONO_SECRETO=16;
	static const unsigned int H_ICONO_SECRETO=16;
	static const unsigned int Y_ICONO_SECRETO=64;
	static const unsigned int X_ICONO_SECRETO_PRESENTE=0;
	static const unsigned int X_ICONO_SECRETO_AUSENTE=16;
	static const unsigned int X_SECRETO=580;
	static const unsigned int Y_SECRETO=6;


	Representador();
	~Representador();

	public:

/*	static bool representar(
		DLibV::Pantalla& pantalla, 
		DLibV::Camara& camara, 
		Representable * r,
		Bloque_transformacion_representable& temp,
		float offset_x,
		float offset_y);*/

	static unsigned int generar_vista(
		DLibV::Pantalla& pantalla, 
		DLibV::Camara& camara, 
		std::vector<Representable *>& v,
		float offset_x,
		float offset_y,
		Bloque_transformacion_representable&);

	static unsigned int generar_overlays(
		DLibV::Pantalla& pantalla, 
		DLibV::Camara& camara, 
		std::vector<Representable_overlay *>& v,
		float offset_x,
		float offset_y, 
		Bloque_transformacion_representable&);

	static void generar_icono_secreto(DLibV::Pantalla& pantalla, bool);
	static void generar_texto_debug(DLibV::Pantalla& pantalla, const std::string& texto);
	static void generar_texto_nombre_sala(DLibV::Pantalla& pantalla, const DLibH::t_cadena& texto);
	static void generar_fondo_info_tanque(DLibV::Pantalla&);
	static void generar_info_tanque(DLibV::Pantalla&, float, bool, unsigned int);
	static void generar_info_habilidad(DLibV::Pantalla&, unsigned int);
/*
	void test_debug()
	{
		if(debug)
		{
			bool siguiente=false;

			while(!siguiente)
			{		
				controles_sdl.recoger();

				if(controles_sdl.es_tecla_down(SDLK_ESCAPE)) 
				{	
					debug=false;
					siguiente=true;
				}
				else if(controles_sdl.es_tecla_down(SDLK_x))
				{
					siguiente=true;
				}
			
				pantalla.flipar();
			}
		}
	}
*/

/*	
	void dibujar_fila_vista_pajaro(unsigned int indice, int offsetx, int offsety, Sala &s, Representable * ref_actor)
	{
		std::vector<Representable *> v;
		for(unsigned int x=0; x< s.acc_w(); ++x)
			for(unsigned int y=0; y<s.acc_d(); ++y)
			{
				//Ojo con la referencia... Si no metemos siempre lo mismo en el vector.
				Bloque& bloque=s.obtener_bloque(x, y, indice);
				if(bloque.acc_tipo_comportamiento()) v.push_back(&bloque);
			}
		
		v.push_back(ref_actor);

		//Dibujar rejilla...
		DLibV::Primitiva_grafica_linea_dinamica l(0, 0, 0, 0, DLibV::Gestor_color::color(64, 64, 64));

		const int W=(s.acc_w()*Bloque::W_BLOQUE)+1;	
		const int H=(s.acc_d()*Bloque::D_BLOQUE)+1;

		//Lineas horizontales...

		for(int y=0; y<H; y+=16)
		{
			l.mut_puntos(offsetx, offsety+y, offsetx+W, offsety+y);
			l.volcar(pantalla);
		}

		//Líneas verticales.
		for(int x=0; x<W; x+=16)
		{
			l.mut_puntos(offsetx+x, offsety, offsetx+x, offsety+H);
			l.volcar(pantalla);
		}

		//Dibujar cosas...
		DLibV::Primitiva_grafica_caja_estatica caja(DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0), DLibV::Gestor_color::color(128,128,128));
		
		for(Representable * o : v)
		{			
			o->transformar_representacion_vista_pajaro(caja);
			unsigned int x=caja.acc_posicion().x+offsetx;
			unsigned int y=caja.acc_posicion().y+offsety;
			caja.establecer_posicion(x, y);
			caja.volcar(pantalla);
		}
	}
*/
};

#endif
