#include "controlador_status.h"

const std::string Controlador_status::RUTA_LOCALIZACION="data/localizacion/info";
Uint32 Controlador_status::COLOR_FONDO_INFO=0;

Controlador_status::Controlador_status(
	Datos_sistema& pds, 
	const Info_juego& pij, 
	const Control_salas& pcs, 
	const Control_objetivos& pco):
	datos_sistema(pds), info_juego(pij), 
	control_salas(pcs), control_objetivos(pco),
	loc(RUTA_LOCALIZACION),
	mostrar_info_habilidad(false),
	titulo_seccion_habilidad(DLibV::Gestor_recursos_graficos::obtener(3), ""),
	titulo_habilidad(DLibV::Gestor_recursos_graficos::obtener(3), ""),
	texto_habilidad(DLibV::Gestor_recursos_graficos::obtener(3), ""),
	rep_iconos_puzzle(NULL),
	titulo_seccion_puzzle(DLibV::Gestor_recursos_graficos::obtener(3), "")
{
	COLOR_FONDO_INFO=DLibV::Gestor_color::color(0,0,0);
	loc.inicializar(datos_sistema.acc_id_idioma());

	recortes_simbolos.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_SIMBOLO_TIEMPO, Y_SIMBOLO_TIEMPO, W_SIMBOLO, H_SIMBOLO));
	recortes_simbolos.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_SIMBOLO_SALAS, Y_SIMBOLO_SALAS, W_SIMBOLO, H_SIMBOLO));
	recortes_simbolos.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_SIMBOLO_SECRETOS, Y_SIMBOLO_SECRETOS, W_SIMBOLO, H_SIMBOLO));
	recortes_simbolos.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_SIMBOLO_COFRES, Y_SIMBOLO_COFRES, W_SIMBOLO, H_SIMBOLO));
	recortes_simbolos.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_SIMBOLO_LLAVES, Y_SIMBOLO_LLAVES, W_SIMBOLO, H_SIMBOLO));
	recortes_simbolos.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_SIMBOLO_BONUS_TIEMPO, Y_SIMBOLO_BONUS_TIEMPO, W_SIMBOLO, H_SIMBOLO));
	recortes_simbolos.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_SIMBOLO_TESORO, Y_SIMBOLO_TESORO, W_SIMBOLO, H_SIMBOLO));

	titulo_seccion_habilidad.asignar(loc.obtener(INDICE_LOCALIZACION_AMULETO));
	titulo_seccion_puzzle.asignar(loc.obtener(INDICE_LOCALIZACION_PUZZLE));
}

Controlador_status::~Controlador_status()
{
	limpiar_iconos_puzzle();
}

void Controlador_status::inicializar(const std::string& pt, const std::vector<unsigned int>& v)
{
	//Preparar cadenas de información de cofres, llaves y demás...

	cadenas_info.clear();	

	unsigned int total_normal=control_salas.obtener_total_salas_por_tipo(Control_salas::TP_NORMAL);
	unsigned int visitada_normal=control_salas.obtener_total_salas_descubiertas_por_tipo(Control_salas::TP_NORMAL);

	unsigned int total_secretas=control_salas.obtener_total_salas_por_tipo(Control_salas::TP_SECRETO);
	unsigned int visitada_secretas=control_salas.obtener_total_salas_descubiertas_por_tipo(Control_salas::TP_SECRETO);

	unsigned int total_bonus=control_objetivos.obtener_total(Control_objetivos::T_TESORO);
	unsigned int recogido_bonus=control_objetivos.obtener_contados(Control_objetivos::T_TESORO);
	
	unsigned int total_cofres=control_objetivos.obtener_total(Control_objetivos::T_COFRE);
	unsigned int recogido_cofres=control_objetivos.obtener_contados(Control_objetivos::T_COFRE);

	unsigned int total_llaves=info_juego.acc_llaves();
	unsigned int total_bonus_tiempo=info_juego.acc_bonus_tiempo();

	cadenas_info.push_back(pt);
	cadenas_info.push_back(DLibH::Herramientas::entero_a_cadena(visitada_normal)+"/"+DLibH::Herramientas::entero_a_cadena(total_normal));
	cadenas_info.push_back(DLibH::Herramientas::entero_a_cadena(visitada_secretas)+"/"+DLibH::Herramientas::entero_a_cadena(total_secretas));
	cadenas_info.push_back(DLibH::Herramientas::entero_a_cadena(recogido_cofres)+"/"+DLibH::Herramientas::entero_a_cadena(total_cofres));
	cadenas_info.push_back(DLibH::Herramientas::entero_a_cadena(total_llaves));
	cadenas_info.push_back(DLibH::Herramientas::entero_a_cadena(total_bonus_tiempo));
	cadenas_info.push_back(DLibH::Herramientas::entero_a_cadena(recogido_bonus)+"/"+DLibH::Herramientas::entero_a_cadena(total_bonus));

	bmp.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(8));

	icono_habilidad.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(8));
	icono_habilidad.establecer_posicion(X_ICONO_HABILIDAD, Y_ICONO_HABILIDAD);

	titulo_habilidad.establecer_posicion(X_TITULO_HABILIDAD, Y_TITULO_HABILIDAD);

	texto_habilidad.establecer_posicion(X_TEXTO_HABILIDAD, Y_TEXTO_HABILIDAD);

	titulo_seccion_habilidad.establecer_posicion(X_TITULO_SECCION_HABILIDAD, Y_TITULO_SECCION_HABILIDAD);

	titulo_seccion_puzzle.establecer_posicion(X_TITULO_SECCION_PUZZLE, Y_TITULO_SECCION_PUZZLE);

	calcular_datos_puzzle(v);
}

void Controlador_status::loop(float delta)
{

}

void Controlador_status::dibujar(DLibV::Pantalla& pantalla, float delta)
{
	pantalla.rellenar(COLOR_FONDO_INFO,
		DLibH::Herramientas_SDL::nuevo_sdl_rect(
			X_INFO, Y_INFO, W_INFO, H_INFO));

	unsigned int i=0;
	unsigned int y=Y_INICIO_SIMBOLOS;

	DLibV::Representacion_texto_auto_estatica txt(DLibV::Gestor_recursos_graficos::obtener(3), "");

	while(i < C_MAX)
	{
		bmp.establecer_recorte(recortes_simbolos[i]); 			
		bmp.establecer_posicion(X_SIMBOLOS, y);

		txt.asignar(cadenas_info[i]);
		txt.establecer_posicion(X_TEXTOS, y+MARGEN_Y_TEXTO);

		bmp.volcar(pantalla);
		txt.volcar(pantalla);

		++i;
		y+=SALTO_Y_SIMBOLOS;
	}

	//Mostrado de las habilidades del amuleto...
	if(mostrar_info_habilidad)
	{
		icono_habilidad.volcar(pantalla);
		titulo_seccion_habilidad.volcar(pantalla);
		titulo_habilidad.volcar(pantalla);
		texto_habilidad.volcar(pantalla);
	}

	//Mostrado de las piezas del puzzle...
	if(piezas_obtenidas.size())
	{
		titulo_seccion_puzzle.volcar(pantalla);
		rep_iconos_puzzle->volcar(pantalla);
	}
}

void Controlador_status::limpiar_iconos_puzzle()
{
	if(rep_iconos_puzzle)
	{
		delete rep_iconos_puzzle;
	}

	rep_iconos_puzzle=NULL;
}

void Controlador_status::calcular_datos_puzzle(const std::vector<unsigned int>& v)
{
	piezas_obtenidas=v;

	if(piezas_obtenidas.size())
	{
		if(rep_iconos_puzzle) limpiar_iconos_puzzle();
		rep_iconos_puzzle=new DLibV::Representacion_compuesta_estatica(W_REP_PIEZAS, H_REP_PIEZAS);

		int x=0, y=0;
		DLibV::Representacion_bitmap_estatica rbe;
		
		for(unsigned int i : piezas_obtenidas)
		{
			if(x >= W_REP_PIEZAS)
			{
				x=0;
				y+=SALTO_Y_PIEZAS;
			}

			Definicion_pieza_puzzle d=Control_pieza_puzzle::obtener_definicion_pieza(i);
			rbe.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(d.id_recurso));

			//Marco...
			rbe.establecer_posicion(x, y);
			rbe.establecer_recorte(X_TABLETA, Y_TABLETA, W_TABLETA, H_TABLETA);
			rep_iconos_puzzle->volcar_representacion(rbe, 
				DLibH::Herramientas_SDL::nuevo_sdl_rect(
					x, y, W_TABLETA, H_TABLETA));

			//Símbolo.
			rbe.establecer_posicion(x+2, y+2);
			rbe.establecer_recorte(d.recorte_x, d.recorte_y, Control_pieza_puzzle::W_PIEZA, Control_pieza_puzzle::H_PIEZA);
			rep_iconos_puzzle->volcar_representacion(rbe, 
				DLibH::Herramientas_SDL::nuevo_sdl_rect(
					x+2, y+2, Control_pieza_puzzle::W_PIEZA, Control_pieza_puzzle::H_PIEZA
					));

			x+=SALTO_X_PIEZAS;
		}
		
		rep_iconos_puzzle->preparar();
		rep_iconos_puzzle->establecer_posicion(X_REP_PIEZAS, Y_REP_PIEZAS);
	}
}

//Diccionarios a palo seco...

unsigned int Controlador_status::obtener_indice_titulo_habilidad(unsigned int i)
{
	switch(i)
	{
		case Control_habilidades::H_SUPER_SALTO: return 107; break;
		case Control_habilidades::H_RESISTENCIA_LAVA: return 100; break;
		case Control_habilidades::H_RESISTENCIA_AGUA: return 101; break;
		case Control_habilidades::H_RESISTENCIA_DARDO: return 106; break;
		case Control_habilidades::H_TELETRANSPORTE: return 102; break;
		case Control_habilidades::H_IMPULSO: return 103; break;
		case Control_habilidades::H_PARAR_TIEMPO: return 104; break;
		case Control_habilidades::H_INSPECTOR_SECRETOS: return 105; 
		default: return 0; break;
	}
}

unsigned int Controlador_status::obtener_indice_texto_habilidad(unsigned int i)
{
	switch(i)
	{
		case Control_habilidades::H_SUPER_SALTO: return 207; break;
		case Control_habilidades::H_RESISTENCIA_LAVA: return 200; break;
		case Control_habilidades::H_RESISTENCIA_AGUA: return 201; break;
		case Control_habilidades::H_RESISTENCIA_DARDO: return 206; break;
		case Control_habilidades::H_TELETRANSPORTE: return 202; break;
		case Control_habilidades::H_IMPULSO: return 203; break;
		case Control_habilidades::H_PARAR_TIEMPO: return 204; break;
		case Control_habilidades::H_INSPECTOR_SECRETOS: return 205; 
		default: return 0; break;
	}
}

void Controlador_status::calcular_datos_habilidad(int indice)
{
	if(indice==-1)
	{
		mostrar_info_habilidad=false;
	}
	else
	{
		mostrar_info_habilidad=true;

		unsigned int x=(indice * W_RECORTE_HABILIDAD) + X_RECORTE_HABILIDAD;
		unsigned int indice_titulo=obtener_indice_titulo_habilidad(indice);
		unsigned int indice_texto=obtener_indice_texto_habilidad(indice);

		icono_habilidad.establecer_recorte(x, Y_RECORTE_HABILIDAD, W_RECORTE_HABILIDAD, H_RECORTE_HABILIDAD);
		titulo_habilidad.asignar(loc.obtener(indice_titulo));
		texto_habilidad.asignar(loc.obtener(indice_texto));	
	}
}
