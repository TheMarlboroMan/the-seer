#include "controlador_diapositivas.h"

const float Controlador_diapositivas::T_FADE=0.4;
const float Controlador_diapositivas::T_PARPADEO_SIGUIENTE=2.4;
const std::string Controlador_diapositivas::RUTA_LOCALIZACION="data/localizacion/diapositivas";

Controlador_diapositivas::Controlador_diapositivas(
	Datos_sistema& ds,
	DLibV::Pantalla& pp, 
	const Input& i,
//DLibI::Controles_SDL& pc, 
//	DLibH::Controlador_fps_SDL& pf, 
	int w, int h):
	datos_sistema(ds), 
	pantalla(pp), 
	input(i),
//controles_sdl(pc), 
//control_frames(pf),
	W_PANTALLA(w), H_PANTALLA(h),
//	rep(),
	recurso_ampliado(NULL),
	caja_trans(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_IMG, Y_IMG, W_IMG*2, H_IMG*2),
			DLibV::Gestor_color::color(0,0,0)),
	loc_textos(RUTA_LOCALIZACION),
	estado(E_APARECER),
	diapositiva_actual(0), indice_texto_actual(0),
	total_diapositivas(0), total_textos(0),
	cadena_actual(""),
	tiempo(0.0), tiempo_siguiente(0.0)
{
	loc_textos.inicializar(datos_sistema.acc_id_idioma());
}

Controlador_diapositivas::~Controlador_diapositivas()
{
	limpiar_recurso_ampliado();
}

void Controlador_diapositivas::limpiar_recurso_ampliado()
{
	if(recurso_ampliado)
	{
		delete recurso_ampliado;
		recurso_ampliado=NULL;
	}
}

Pase_diapositivas Controlador_diapositivas::obtener_y_preparar_pase(unsigned int pid)
{
	Cargador_diapositivas cd(loc_textos);
	cd.inicializar();
	Pase_diapositivas pase=cd.obtener_pase_por_id(pid);

	return pase;
}

void Controlador_diapositivas::reinicializar(Pase_diapositivas& p)
{
	tiempo=0;
	tiempo_siguiente=0;
	diapositiva_actual=0;
	indice_texto_actual=0;

	total_diapositivas=p.longitud();
	cadena_actual=obtener_texto_actual(p);
	rep=preparar_actual(p);
}

bool Controlador_diapositivas::loop(float delta, Pase_diapositivas& p)
{
	bool resultado=true;

	if(estado==E_FIN)
	{
		resultado=false;
	}
	else
	{
		tiempo+=delta;
		tiempo_siguiente+=delta;

		if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE))
		{
			resultado=false;
		}
		//Espacio obliga a saltar el paso...
		else if(estado==E_ESPERA &&
			(
				this->input.es_input_down(Input::I_SALTAR) || 
				this->input.es_input_down(Input::I_INTERACCION))
		)
		{
			siguiente();
			cadena_actual=obtener_texto_actual(p);
		}

		dibujar(delta, p); //, texto_actual, rep, caja_trans);
	}

	return resultado;
}

/*DLibV::Primitiva_grafica_caja_estatica Controlador_diapositivas::obtener_caja_transparencia()
{
	DLibV::Primitiva_grafica_caja_estatica resultado(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_IMG, Y_IMG, W_IMG*2, H_IMG*2),
		DLibV::Gestor_color::color(0,0,0));
	
	return resultado;
}*/

std::string Controlador_diapositivas::obtener_texto_actual(Pase_diapositivas& p)
{
	std::string resultado;

	try
	{
		Diapositiva& dp=p.obtener_diapositiva(diapositiva_actual);
		total_textos=dp.longitud();

		if(total_textos)
		{
			resultado=dp.obtener_texto(indice_texto_actual).texto;
		}
	}
	catch(std::exception& e)
	{

	}

	return resultado;
}


void Controlador_diapositivas::dibujar(float delta, Pase_diapositivas& p)
/*, 
	const std::string& texto, DLibV::Representacion_pixeles_estatica& rep,
	DLibV::Primitiva_grafica_caja_estatica& caja_trans)*/
{

	DLibV::Representacion_texto_fija_estatica txt(
		DLibV::Gestor_recursos_graficos::obtener(3), 
		cadena_actual, W_TXT, H_TXT);
	txt.establecer_posicion(X_TXT, Y_TXT);
	txt.mut_interletra(1);

	DLibV::Representacion_texto_auto_estatica txt_saltar(
		DLibV::Gestor_recursos_graficos::obtener(3), 
		loc_textos.obtener(INDICE_CADENA_SALTAR));
	txt_saltar.establecer_posicion(X_TXT_SALTAR, Y_TXT_SALTAR);

	DLibV::Representacion_texto_auto_estatica txt_siguiente(
		DLibV::Gestor_recursos_graficos::obtener(3), 
		loc_textos.obtener(INDICE_CADENA_SIGUIENTE));
	txt_siguiente.establecer_posicion(X_TXT_CONTINUAR, Y_TXT_CONTINUAR);

	DLibV::Primitiva_grafica_caja_estatica marco_img(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_MARCO_IMG, Y_MARCO_IMG, W_MARCO_IMG, H_MARCO_IMG),
		DLibV::Gestor_color::color(255, 255, 255));

	calcular_opacidad_letrero_siguiente(txt_siguiente);

	switch(estado)
	{
		case E_APARECER:
			txt.hacer_invisible();
			txt_siguiente.hacer_invisible();
			controlar_transparencia(); //caja_trans);

			if(tiempo >= T_FADE)
			{
				tiempo=0;
				tiempo_siguiente=0;
				estado=E_ESPERA;
			}
		break;

		case E_ESPERA:
			txt.hacer_visible();
			txt_siguiente.hacer_visible();
			caja_trans.establecer_alpha(0);
		break;

		case E_DESAPARECER:
			txt.hacer_invisible();
			txt_siguiente.hacer_invisible();
			controlar_transparencia(); //caja_trans);

			if(tiempo >= T_FADE)
			{
				if(diapositiva_actual >= total_diapositivas)
				{
					estado=E_FIN;
					return;
				}
				else
				{
					tiempo=0;
					estado=E_APARECER;
					rep=preparar_actual(p);
				}
			}
		break;

		default:
			return;
		break;
	}

	pantalla.rellenar(0,0,0);

	txt_saltar.volcar(pantalla);
	txt_siguiente.volcar(pantalla);
	txt.volcar(pantalla);

	marco_img.volcar(pantalla);
	rep.volcar(pantalla);
	caja_trans.volcar(pantalla);

	//pantalla.flipar();
}

void Controlador_diapositivas::calcular_opacidad_letrero_siguiente(DLibV::Representacion& r)
{
	float alpha=Herramientas_calculos::calcular_valor_onda_triangulo(tiempo_siguiente, T_PARPADEO_SIGUIENTE) * 255;	
	r.establecer_alpha(alpha);
}

void Controlador_diapositivas::controlar_transparencia() //DLibV::Primitiva_grafica_caja_estatica& caja_trans)
{
	int alpha=(tiempo / T_FADE) * 255 ;
	if(estado==E_APARECER) alpha=255-alpha;

	if(alpha < 0) alpha=0;
	else if(alpha > 255) alpha=255;

	caja_trans.establecer_alpha(alpha);
}

/*Se crea una imagen escalada a partir del trozo correspondiente a la 
diapositiva actual. Se escala justo al doble.*/

DLibV::Representacion_bitmap_estatica Controlador_diapositivas::preparar_actual(Pase_diapositivas& p)
{
	int x=0;
	int y=0;

	try
	{
		Diapositiva& dp=p.obtener_diapositiva(diapositiva_actual);
		x=dp.img.x;
		y=dp.img.y;
	}
	catch(std::exception& e)
	{

	}

	//Esto es lo que vamos a ampliar.
	DLibV::Representacion_bitmap_estatica base;
	base.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(INDICE_IMG));

	//Y esta es la ampliación.
	static const unsigned int factor=2;
	SDL_Rect caja_ampliacion=DLibH::Herramientas_SDL::nuevo_sdl_rect(x, y, W_IMG, H_IMG);
	//DLibV::Representacion_pixeles_estatica rep=Herramientas_graficas::ampliar_bitmap(base, factor, caja_ampliacion);
	SDL_Surface * ampliada=Herramientas_graficas::ampliar_superficie(base.ref_superficie(), factor, caja_ampliacion);

	//El recurso ampliado es una propiedad de esta clase, que ella misma
	//se encarga de limpiar. Cuando eliminamos el recurso ampliado (si ya existe)
	//liberamos la memoria de la superficie ampliada.

	limpiar_recurso_ampliado();
	recurso_ampliado=new DLibV::Imagen(ampliada);

	DLibV::Representacion_bitmap_estatica rep;
	rep.establecer_recurso(recurso_ampliado);
	rep.establecer_recorte(0, 0, W_IMG*factor, H_IMG*factor);
	rep.establecer_posicion(X_IMG, Y_IMG);

	return rep;
}

void Controlador_diapositivas::siguiente()
{
	++indice_texto_actual;
	if(indice_texto_actual >= total_textos)
	{
		++diapositiva_actual;
		indice_texto_actual=0;
		tiempo=0;
		estado=E_DESAPARECER;
	}
}
