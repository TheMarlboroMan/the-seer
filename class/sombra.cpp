#include "sombra.h"

Uint32 Sombra::COLOR_ALTERAR=0;
Uint32 Sombra::COLOR_TRANSPARENTE=0;
Uint32 Sombra::COLOR_LAVA_01=0;
Uint32 Sombra::COLOR_LAVA_02=0;
Uint32 Sombra::COLOR_LAVA_03=0;
Uint32 Sombra::COLOR_LAVA_04=0;
Uint32 Sombra::COLOR_AGUA_01=0;
Uint32 Sombra::COLOR_AGUA_02=0;
Uint32 Sombra::COLOR_AGUA_03=0;
Uint32 Sombra::COLOR_AGUA_04=0;

Sombra::Sombra()
	:Actor(0, 0, 0), 
	recurso_alterado(NULL),visible(false),
	estado(E_NORMAL)
{
	actualizar_volumen();
	inicializar_colores();
	generar_recurso();
}

Sombra::~Sombra()
{
	liberar_recurso();
}

void Sombra::inicializar_colores()
{
	COLOR_ALTERAR=DLibV::Gestor_color::color(255,255,255);
	COLOR_TRANSPARENTE=DLibV::Gestor_color::color(0,255,255);

	COLOR_LAVA_01=DLibV::Gestor_color::color(20, 120, 220);
	COLOR_LAVA_02=DLibV::Gestor_color::color(20, 100, 220);
	COLOR_LAVA_03=DLibV::Gestor_color::color(20, 80, 220);
	COLOR_LAVA_04=DLibV::Gestor_color::color(20, 60, 220);

	COLOR_AGUA_01=DLibV::Gestor_color::color(254, 172, 0);
	COLOR_AGUA_02=DLibV::Gestor_color::color(255, 113, 0);
	COLOR_AGUA_03=DLibV::Gestor_color::color(255, 86, 0);
	COLOR_AGUA_04=DLibV::Gestor_color::color(254, 41, 0);
}

void Sombra::generar_recurso()
{
	//Generamos primero la superficie vacía...
	recurso_alterado=DLibV::Lienzo::generar_nuevo(W_RECORTE*(TOTAL_PASOS_ANIMACION*2), H_RECORTE);

	//Lo primero que necesitamos es una superficie para que sea el patrón.	
	SDL_Rect caja_recorte=DLibH::Herramientas_SDL::nuevo_sdl_rect(X_PATRON, Y_PATRON, W_RECORTE, H_RECORTE);

	//Antes de copiar el trozo de recurso tenemos que limpiar la clave de
	//color. El problema con la clave de color está en que tenemos que 
	//alterar el recurso original momentáneamente para que se copien los
	//píxeles tal cual, puesto que lo que se hace es un blit.
	//Buscamos conseguir así una copia 100% honesta, con el cian y todo.

	DLibV::Recurso_grafico * r=DLibV::Gestor_recursos_graficos::obtener(2);

	Uint32 c_trans=r->acc_color_transparencia();
	r->limpiar_transparencia();

	SDL_Surface * patron=DLibV::Utilidades_graficas_SDL::copiar_superficie(
		r->acc_superficie(), 
		caja_recorte);

	r->establecer_transparencia(c_trans);

	//Vamos a hacer un vector con todos los colores de la animación. 
	//Iteraremos por el mismo generando una hoja lineal.

	std::vector<Uint32> colores;
	colores.push_back(COLOR_LAVA_01);
	colores.push_back(COLOR_LAVA_02);
	colores.push_back(COLOR_LAVA_03);
	colores.push_back(COLOR_LAVA_04);
	colores.push_back(COLOR_AGUA_01);
	colores.push_back(COLOR_AGUA_02);
	colores.push_back(COLOR_AGUA_03);
	colores.push_back(COLOR_AGUA_04);

	//Generamos nuevas superficies cambiando el color del patrón. Cada una
	//de estas superficies la bliteamos sobre la superficie del recurso,
	//generando a cada paso el recurso final.


	unsigned int i=0;

	for(Uint32 c : colores)
	{
		SDL_Rect caja_origen=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, W_RECORTE, H_RECORTE);
		SDL_Rect caja_destino=DLibH::Herramientas_SDL::nuevo_sdl_rect(i*W_RECORTE, 0, W_RECORTE, H_RECORTE);

		SDL_Surface * alterada=Herramientas_graficas::generar_superficie_alterando_color(
			patron, caja_origen, COLOR_ALTERAR, c);

		SDL_BlitSurface(alterada, &caja_origen, recurso_alterado->acc_superficie(), &caja_destino);
		SDL_FreeSurface(alterada);

		++i;
	}
	
	//Liberamos el patrón, que ya no lo queremos para nada.
	SDL_FreeSurface(patron);
	recurso_alterado->establecer_transparencia(COLOR_TRANSPARENTE);
}

void Sombra::liberar_recurso()
{
	if(recurso_alterado)
	{
		delete recurso_alterado;
		recurso_alterado=NULL;
	}
}

void Sombra::transformar_representacion(Bloque_transformacion_representable& br) 
{
	br.usar_bitmap();

	if(!visible)
	{
		br.establecer_recorte(0, 0, 0, 0);
		br.establecer_posicion(0, 0, 0, 0);
	}
	else
	{
		Punto_2d p=obtener_coordenadas_origen_representacion();

		unsigned int x;
		
		switch(estado)
		{
			case E_LAVA:
				br.establecer_recurso_manual(recurso_alterado);
				x=calcular_x_representacion(0);
			break;

			case E_AGUA: 
				br.establecer_recurso_manual(recurso_alterado);
				x=calcular_x_representacion(4);
			break;
			
			default:
				br.establecer_recurso_sin_recortar(2);
				x=X_SOMBRA_NORMAL; 
			break;
		}

		br.establecer_recorte(W_RECORTE * x, Y_SOMBRA_NORMAL, W_RECORTE, H_RECORTE);

		p.x-=7;
		p.y+=3;
		br.establecer_posicion(p.x, p.y, 16, 16);
	}
}

unsigned int Sombra::calcular_x_representacion(unsigned int offset)
{
	float t_anim=fmod(Temporizador_animaciones::tiempo_animacion, 0.600);
	unsigned int x=0;

	if(t_anim < 0.100) x=0;
	else if(t_anim < 0.200) x=1;
	else if(t_anim < 0.300) x=2;
	else if(t_anim < 0.400) x=3;
	else if(t_anim < 0.500) x=2;
	else x=1;

	return x+offset;
}
