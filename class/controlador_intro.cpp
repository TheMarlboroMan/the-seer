#include "controlador_intro.h"

const std::string Controlador_intro::RUTA_LOCALIZACION="data/localizacion/info";
const float Controlador_intro::T_PARPADEO=2.5;

Controlador_intro::Controlador_intro(Datos_sistema& ds, Control_sesiones_guardadas& c, 
	Configuracion& cfg)
	:datos_sistema(ds), control_sesiones_guardadas(c), 
	configuracion(cfg),
	loc(RUTA_LOCALIZACION),
	txt_pulsar_tecla(DLibV::Gestor_recursos_graficos::obtener(3), ""),
	txt_iniciar_cargar_juego(DLibV::Gestor_recursos_graficos::obtener(3), ""),
	estado(E_INTRO), id_sesion_guardado(0), volver_a_juego_habilitado(false)
{
	loc.inicializar(ds.acc_id_idioma());

	txt_pulsar_tecla.establecer_posicion(X_TXT_PULSA_TECLA, Y_TXT_PULSA_TECLA);
	txt_pulsar_tecla.mut_interletra(1);
	txt_pulsar_tecla.asignar(loc.obtener(INDICE_CADENA_PULSA_TECLA));

	txt_iniciar_cargar_juego.establecer_posicion(X_TXT_INICIAR_CARGAR_JUEGO, Y_TXT_INICIAR_CARGAR_JUEGO);
	txt_iniciar_cargar_juego.mut_interletra(1);
	txt_iniciar_cargar_juego.asignar(loc.obtener(INDICE_CADENA_SELECCION_DATOS_JUEGO));

	fondo.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(10));
	fondo.establecer_recorte(0,0, 180, 400);
	fondo.establecer_posicion(X_FONDO_PRINCIPAL, Y_FONDO_PRINCIPAL);

	frente.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(10));
	frente.establecer_recorte(180,0, 53, 230);
	frente.establecer_posicion(X_FONDO_PRINCIPAL+DESP_X_FONDO, Y_FONDO_SECUNDARIO);

	actualizar_alpha_fondo();
	generar_menus();
	generar_sesiones();
	generar_opciones();
}

/*Generamos por una lado las posibilidades de configuración y por
otro los menús correspondientes.*/

void Controlador_intro::generar_opciones()
{
	items_configuracion.clear();
	menu_opciones_titulos.vaciar();
	menu_opciones_valores.vaciar();

	//Opciones de configuracion: Las creamos y rellenamos.
	//Seleccionamos la opción de turno y luego rellenamos 
	//el vector.

	Item_configuracion video_pantalla_completa(loc.obtener(INDICE_CADENA_CONFIG_FULLSCREEN));
	video_pantalla_completa.insertar_opcion(0, loc.obtener(INDICE_CADENA_VAL_CONFIG_NO));
	video_pantalla_completa.insertar_opcion(1, loc.obtener(INDICE_CADENA_VAL_CONFIG_SI));
	video_pantalla_completa.establecer_actual(configuracion.acc_pantalla_completa());

	Item_configuracion video_modo_hardware(loc.obtener(INDICE_CADENA_CONFIG_HARDWARE));
	video_modo_hardware.insertar_opcion(0, loc.obtener(INDICE_CADENA_VAL_CONFIG_NO));
	video_modo_hardware.insertar_opcion(1, loc.obtener(INDICE_CADENA_VAL_CONFIG_SI));
	video_modo_hardware.establecer_actual(configuracion.acc_modo_hardware());

	Item_configuracion audio_volumen_sonido(loc.obtener(INDICE_CADENA_CONFIG_SONIDO));
	audio_volumen_sonido.insertar_opcion(0, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_0));
	audio_volumen_sonido.insertar_opcion(16, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_1));
	audio_volumen_sonido.insertar_opcion(32, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_2));
	audio_volumen_sonido.insertar_opcion(64, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_3));
	audio_volumen_sonido.insertar_opcion(96, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_4));
	audio_volumen_sonido.insertar_opcion(127, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_5));
	audio_volumen_sonido.establecer_actual(configuracion.acc_volumen_audio());

	Item_configuracion audio_volumen_musica(loc.obtener(INDICE_CADENA_CONFIG_MUSICA));
	audio_volumen_musica.insertar_opcion(0, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_0));
	audio_volumen_musica.insertar_opcion(16, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_1));
	audio_volumen_musica.insertar_opcion(32, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_2));
	audio_volumen_musica.insertar_opcion(64, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_3));
	audio_volumen_musica.insertar_opcion(96, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_4));
	audio_volumen_musica.insertar_opcion(127, loc.obtener(INDICE_CADENA_VAL_CONFIG_VOL_5));
	audio_volumen_musica.establecer_actual(configuracion.acc_volumen_musica());

	Item_configuracion idioma(loc.obtener(INDICE_CADENA_CONFIG_IDIOMA));
	Cargador_idiomas ci;
	const std::vector<std::string>& idiomas=ci.acc_idiomas();
	unsigned int i=0;
	for(const std::string& c : idiomas) idioma.insertar_opcion(i++, c);
	idioma.establecer_actual(configuracion.acc_idioma());

	items_configuracion.push_back(video_pantalla_completa);
	items_configuracion.push_back(video_modo_hardware);
	items_configuracion.push_back(audio_volumen_sonido);
	items_configuracion.push_back(audio_volumen_musica);
	items_configuracion.push_back(idioma);
	

	for(Item_configuracion& i : items_configuracion)
	{
		menu_opciones_titulos.insertar_opcion(i.acc_titulo());
		menu_opciones_valores.insertar_opcion(i.acc_titulo_seleccionado());
	}
}

void Controlador_intro::generar_menus()
{
	menu_principal.vaciar();

	if(volver_a_juego_habilitado) menu_principal.insertar_opcion(loc.obtener(INDICE_CADENA_VOLVER_JUEGO));
	menu_principal.insertar_opcion(loc.obtener(INDICE_CADENA_INICIAR_CARGAR_JUEGO));
	menu_principal.insertar_opcion(loc.obtener(INDICE_CADENA_OPCIONES));
}

void Controlador_intro::generar_sesiones()
{
	const std::vector<Control_sesiones_guardadas::Info_sesion_guardada> v=control_sesiones_guardadas.obtener_sesiones();

	for(const Control_sesiones_guardadas::Info_sesion_guardada& ses : v)
	{
		const std::string info=ses.es_libre() ? 
			loc.obtener(INDICE_CADENA_DATOS_JUEGO_LIBRE) : 
			ses.como_cadena();	

		const std::string nombre=loc.obtener(INDICE_CADENA_DATOS_JUEGO)+" "+
			DLibH::Herramientas::entero_a_cadena(ses.acc_indice())+" ["+info+"]";

		menu_partidas.insertar_opcion(nombre);
	}
}

void Controlador_intro::limpiar_sesiones()
{
	menu_partidas.vaciar();
}

void Controlador_intro::restaurar_estado_menu()
{
	control_sesiones_guardadas.refrescar();
	limpiar_sesiones();
	generar_sesiones();

	estado=E_MENU;
	int dest=obtener_x_fondo_para_estado(estado);
	fondo.establecer_posicion(dest, Y_FONDO_PRINCIPAL);
	frente.establecer_posicion(dest+DESP_X_FONDO, Y_FONDO_SECUNDARIO);
	actualizar_alpha_fondo();
}

void Controlador_intro::dibujar(DLibV::Pantalla& p)
{
	p.rellenar(0,0,0);

	fondo.volcar(p);
	frente.volcar(p);

	if(!grupo_animacion.tiene_animaciones())
	{
		switch(estado)
		{
			case E_INTRO: dibujar_estado_intro(p); break;
			case E_MENU: dibujar_estado_menu(p); break;
			case E_SELECCION_PARTIDA: dibujar_estado_seleccion_partida(p); break;
			case E_OPCIONES: dibujar_estado_opciones(p); break;
		}
	}
}

/*Simplemente un "press any key"*/

void Controlador_intro::dibujar_estado_intro(DLibV::Pantalla &p)
{
	float falpha=Herramientas_calculos::calcular_valor_onda_triangulo(tiempo, T_PARPADEO);
	unsigned short int alpha=falpha*255;
	 
	//Triste, pero a veces pasa de 0 a 255, por alguna historia interna.
	if(alpha) txt_pulsar_tecla.establecer_alpha(alpha);
	txt_pulsar_tecla.volcar(p);
}

/*Se dibujan las opciones de menú: que son iniciar partida y opciones.*/

void Controlador_intro::dibujar_estado_menu(DLibV::Pantalla &p)
{
	menu_principal.dibujar(p, X_MENUS, Y_MENUS);
}

/*Se muestran los slots para iniciar partida.*/

void Controlador_intro::dibujar_estado_seleccion_partida(DLibV::Pantalla &p)
{
	txt_iniciar_cargar_juego.volcar(p);
	menu_partidas.dibujar(p, X_MENUS, Y_MENUS);
}

void Controlador_intro::dibujar_estado_opciones(DLibV::Pantalla &p)
{
	menu_opciones_titulos.dibujar(p, X_MENUS, Y_MENUS);
	menu_opciones_valores.dibujar(p, X_MENUS+DESP_X_CONFIG, Y_MENUS);
}

void Controlador_intro::procesar_input(const Input& input)
{
	if(grupo_animacion.tiene_animaciones()) return;

	bool atras=false;
	bool tecla_down=false;
	bool abajo=false;
	bool arriba=false;
	bool izquierda=false;
	bool derecha=false;
	bool borrar=false;

	if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE)) atras=true;
	else if(input.es_input_down(Input::I_DELETE)) borrar=true;
	else if(input.es_input_down(Input::I_ABAJO)) abajo=true;
	else if(input.es_input_down(Input::I_ARRIBA)) arriba=true;
	else if(input.es_input_down(Input::I_IZQUIERDA)) izquierda=true;
	else if(input.es_input_down(Input::I_DERECHA)) derecha=true;
	else if(input.hay_eventos_teclado_down()) tecla_down=true;

	switch(estado)
	{
		case E_INTRO:
			if(atras) estado=E_SALIR_SISTEMA;
			else if(tecla_down) cambiar_estado(E_MENU);
		break;

		case E_MENU:
			if(atras) cambiar_estado(E_INTRO);
			else if(arriba) menu_principal.subir();
			else if(abajo) menu_principal.bajar();
			else if(tecla_down)
			{
				unsigned int id_opcion=menu_principal.acc_opcion_actual();
				if(!volver_a_juego_habilitado) ++id_opcion;
	
				switch(id_opcion)
				{
					case 0: cambiar_estado(E_VOLVER_JUEGO); break;
					case 1: cambiar_estado(E_SELECCION_PARTIDA); break;
					case 2: cambiar_estado(E_OPCIONES); break;
				}			
			}
		break;

		case E_SELECCION_PARTIDA:
			if(atras) cambiar_estado(E_MENU);
			else if(borrar) eliminar_sesion(menu_partidas.acc_opcion_actual());
			else if(arriba) menu_partidas.subir();
			else if(abajo) menu_partidas.bajar();
			else if(tecla_down) 
			{
				id_sesion_guardado=menu_partidas.acc_opcion_actual();
				estado=E_INICIAR_JUEGO;
			}
		break;

		case E_OPCIONES: 
			if(atras) 
			{
				configuracion.grabar();
				cambiar_estado(E_MENU);
			}
			else if(izquierda) cambiar_valor_opciones(menu_opciones_titulos.acc_opcion_actual(), -1);
			else if(derecha) cambiar_valor_opciones(menu_opciones_titulos.acc_opcion_actual(), 1);
			else if(arriba) 
			{
				menu_opciones_titulos.subir();
				menu_opciones_valores.subir();
			}
			else if(abajo) 
			{
				menu_opciones_titulos.bajar();
				menu_opciones_valores.bajar();
			}
		break;
	}
}

void Controlador_intro::cambiar_valor_opciones(unsigned int opc, int dir)
{
	try
	{
		Item_configuracion& ic=items_configuracion.at(opc);
		ic.cambiar_seleccion(dir);
		menu_opciones_valores.cambiar_opcion(opc, ic.acc_titulo_seleccionado());
		int valor_actual=ic.acc_valor_seleccionado();
		unsigned int reconfig=Datos_sistema::C_NADA;

		switch(opc)
		{
			case VIDEO_PANTALLA_COMPLETA: 
				configuracion.mut_pantalla_completa(valor_actual);
				reconfig=reconfig | Datos_sistema::C_VIDEO;
			break;
			case VIDEO_HARDWARE: 
				configuracion.mut_modo_hardware(valor_actual);
				reconfig=reconfig | Datos_sistema::C_VIDEO;
			break;
			case VOLUMEN_SONIDO: 
				configuracion.mut_volumen_audio(valor_actual);
				reconfig=reconfig | Datos_sistema::C_AUDIO;
			break;
			case VOLUMEN_MUSICA: 
				configuracion.mut_volumen_musica(valor_actual);
				reconfig=reconfig | Datos_sistema::C_AUDIO;
			break;
			case IDIOMA:
				configuracion.mut_idioma(valor_actual);
				reconfig=reconfig | Datos_sistema::C_IDIOMA;
			break;
		}

		datos_sistema.establecer_reconfiguracion(reconfig);
	}
	catch(std::out_of_range& e) {}

}

void Controlador_intro::cambiar_estado(unsigned int e)
{
	estado=e;

	//Al cambiar el estado se mueve la posición de los objetos del fondo.
	//Lo que hacemos es crear una animación, que se procesará con su turno
	//e impedirá más input hasta completarse.

	grupo_animacion.vaciar();
	int pos=fondo.acc_posicion().x;
	int dest=obtener_x_fondo_para_estado(estado);

	if(pos!=dest)
	{
		float v=pos < dest ? 200 : -200;
		grupo_animacion.asignar_animacion(
			new Estructura_animacion_movimiento(fondo, DLibH::Vector_2d(v, 0), dest, Y_FONDO_PRINCIPAL));
	
		grupo_animacion.asignar_animacion(
			new Estructura_animacion_movimiento(frente, DLibH::Vector_2d(v, 0), dest+DESP_X_FONDO, Y_FONDO_SECUNDARIO));
	}
}

int Controlador_intro::obtener_x_fondo_para_estado(unsigned int estado)
{	
	unsigned int paso=0;
	switch(estado)
	{
		case E_INTRO: paso=0; break;
		case E_VOLVER_JUEGO:
		case E_MENU: 
			paso=1; 
		break;
		case E_SELECCION_PARTIDA: 
		case E_OPCIONES:
			paso=2; 
		break;
	}

	return (paso * SALTO_X_FONDO) + X_FONDO_PRINCIPAL;
}

void Controlador_intro::eliminar_sesion(unsigned int i)
{
	limpiar_sesiones();
	control_sesiones_guardadas.eliminar_sesion(i);
	generar_sesiones();	
}

void Controlador_intro::logica(float delta)
{
	grupo_animacion.turno(delta);
	if(grupo_animacion.es_finalizado()) grupo_animacion.vaciar();
	else if(grupo_animacion.tiene_animaciones()) actualizar_alpha_fondo();
}

/*Según se mueven los elementos del fondo vamos actualizando también el alpha
de los mismos.*/

void Controlador_intro::actualizar_alpha_fondo()
{
	//TODO: Hacer esto mejor.
	int alpha=fondo.acc_posicion().x - X_FONDO_PRINCIPAL + 5;
	if(alpha < 0) alpha=0;
	else if(alpha > 255) alpha=255;

	fondo.establecer_alpha(alpha / 2);
	frente.establecer_alpha(alpha);
}

void Controlador_intro::loop(float delta, DLibV::Pantalla& p, const Input& input)
{
	tiempo+=delta;
	procesar_input(input);
	logica(delta);
	dibujar(p);
}

void Controlador_intro::habilitar_volver_a_juego()
{
	volver_a_juego_habilitado=true;
	generar_menus();
}
