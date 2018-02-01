#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <exception>

#include "libdan_headers/libDanSDL.h"
#include "libdan_headers/defDanSDL.h"

#include "class/cargador_recursos.h"
#include "class/datos_legibles.h"

DLibH::Log_base LOG;

class Visor
{
	private:
	
	static const int COLUMNAS=18;
	static const int FILAS=8;
	static const int TW=32;
	static const int TH=47;

	int pag;
	int max_pag;
	unsigned int seleccionado;

	DLibV::Pantalla& pantalla;
	Controlador_datos_representacion_bloque& crp;
	DLibI::Controles_SDL controles_sdl;

	void dibujar()
	{
		int x=0;
		int y=0;

		int rpp=FILAS * COLUMNAS;
		int i=1;
		int indice=pag * (FILAS * COLUMNAS);

		pantalla.rellenar(255,255,255);
		DLibV::Representacion_bitmap_dinamica rep;
		
		while(i <= rpp)
		{
			unsigned int actual=i + indice;

			if(actual >= crp.size()) break;
			else
			{
				if(crp.existe_entrada_por_id(actual))
				{
					Datos_representacion_animada dra=crp.obtener_por_id(actual);
					Frame_representacion fr=dra.obtener_frame_unico();

					int rec=dra.acc_indice_recurso();
					unsigned int alpha=actual==seleccionado ? 128 : 255;

					rep.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(rec));
					rep.establecer_recorte(fr.x, fr.y, fr.w, fr.h);
					rep.establecer_posicion(x, y);
					rep.establecer_alpha(alpha);

					rep.volcar(pantalla);

					if(i % COLUMNAS) x+=TW;
					else
					{
						x=0;
						y+=TH;
					}
				}
			}

			++i;
		}	

		if(seleccionado < crp.size())
		{
			pantalla.rellenar(0,0,0,0,0,40, 10);
			std::string texto=DLibH::Herramientas::entero_a_cadena(seleccionado);
			DLibV::Representacion_texto_fija_estatica txt(DLibV::Gestor_recursos_graficos::obtener(3), texto, 600, 20);
			txt.establecer_posicion(0, 0);
			txt.mut_interletra(1);
			txt.volcar(pantalla);
		}
	}

	void calcular_mouse()
	{
		int x=controles_sdl.acc_raton().acc_x();
		int y=controles_sdl.acc_raton().acc_y();

		int col=x / TW;
		int fila=y / TH;

		int indice=pag * (FILAS * COLUMNAS);

		seleccionado=(fila * COLUMNAS) + col + indice + 1;
	}

	public:
	
	Visor(DLibV::Pantalla& pp, Controlador_datos_representacion_bloque& pcrp): 
		pag(0), seleccionado(0), pantalla(pp), crp(pcrp)
	{
		max_pag=crp.size() / (FILAS * COLUMNAS);
	}

	bool loop()
	{
		controles_sdl.recoger();

		if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
		{
			return false;
		}
		else
		{
			//Sólo dibujamos si hay algún evento, para ahorrar batería.

			if(controles_sdl.recibe_eventos_teclado()	
			|| controles_sdl.recibe_eventos_raton())
			{
				if(controles_sdl.es_tecla_down(SDLK_UP) && pag > 0) --pag;
				else if(controles_sdl.es_tecla_down(SDLK_DOWN) && pag < max_pag) ++pag;
			
				calcular_mouse();
				dibujar();
				pantalla.flipar();
			}

			return true;
		}
	}
};

Frame_representacion obtener_frame_de_linea(std::string linea)
{
	const char separador='\t';
	std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);
	if(valores.size()!=7) 
	{
		LOG<<"ERROR: No hay 7 parametros para cadena de datos de frame, en su lugar "<<valores.size()<<" en linea "<<linea<<std::endl;
		return Frame_representacion(0,0,0,0,0,0,0);
	}
	else 
	{
		return Frame_representacion(
			valores[0], valores[1],
			valores[2], valores[3],
			valores[4], valores[5],
			valores[6]);
	}
}

Controlador_datos_representacion_bloque generar_datos_representacion_bloques()
{
	Controlador_datos_representacion_bloque resultado;
	std::ifstream archivo("data/info/datos_bloques.txt", std::ios::in);
	const char separador='\t';
	const std::string delimitador_linea="[F]";

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de representacion de bloques"<<std::endl;
	}
	else
	{

		//O el funcionamiento alternativo, si hacemos realmente el tema de las animaciones...	
		bool linea_iniciada=false;
		unsigned int id_tipo=0;
		Datos_representacion_animada temp;

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				
			if(linea.size())
			{

				//Si es fin de entrada podemos meterlo en el controlador y fuera.
				if(linea.find(delimitador_linea)!=std::string::npos)
				{
					resultado.insertar(id_tipo, temp);
					linea_iniciada=false;
					temp.reiniciar();
					continue;
				}

				//Si no es línea iniciada es una nueva: obtenemos el id y los datos..
				if(!linea_iniciada)
				{
					std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);
					if(valores.size()!=4) LOG<<"ERROR: No hay 4 parametros para cadena de representacion de bloques, en su lugar "<<valores.size()<<" en linea "<<linea<<std::endl;
					else 
					{
						id_tipo=valores[0];
						temp.asignar_indice_recurso(valores[1]);
						temp.asignar_total_frames(valores[2]);
						temp.asignar_duracion(valores[3]);
						linea_iniciada=true;
					}
				}
				//Si la línea es iniciada y no es fin, sabemos que es un frame.
				else temp.insertar_frame(obtener_frame_de_linea(linea));
			}
		}

		archivo.close();
	}

	LOG<<"Cargados "<<resultado.size()<<" tipos de bloque."<<std::endl;

	return resultado;
}

int main(int argc, char ** argv)
{
	if(DLibH::Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK))
	{
		LOG.inicializar("data/logs/vg.log");
		LOG.activar();
		DLibH::Log_motor::arrancar("data/logs/log_motor.log");

		//Inicializar sistema gráfico y pantalla...
		DLibV::Pantalla pantalla;
		pantalla.inicializar(600, 400, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);
		pantalla.establecer_titulo("--== VISOR BLOQUES ==--");

		DLibV::Utilidades_graficas_SDL::mostrar_ocultar_cursor(true);
		DLibV::Gestor_color::establecer_formato(pantalla.obtener_formato_pixeles());


		//Inicializar info...
		Cargador_recursos cargador_recursos;
		cargador_recursos.generar_recursos_graficos();
		Controlador_datos_representacion_bloque crp=generar_datos_representacion_bloques();

		//Visor
		Visor v(pantalla, crp);

		while(v.loop())
		{
			
		}

	}
	else
	{
		std::cout<<"ERROR AL INICIALIZAR"<<std::endl;
		DLibH::Herramientas_SDL::error();
		DLibH::Log_motor::finalizar();
		DLibH::Herramientas_SDL::apagar_SDL();
	}

	return 0;
}
