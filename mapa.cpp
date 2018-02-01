#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <exception>

#include "libdan_headers/libDanSDL.h"
#include "libdan_headers/defDanSDL.h"

#include "class/cargador_recursos.h"
#include "class/cargador_mapa.h"
#include "class/datos_mapa.h"

DLibH::Log_base LOG;

class Editor
{
	private:

	struct Ordenador_guardar
	{
		Ordenador_guardar()
		{}

//Del manual: "The value returned indicates whether the element passed 
//as first argument is considered to go before the second in the 
//specific strict weak ordering it defines.

		bool operator()(const Mapa_sala& a, const Mapa_sala& b) const
		{
			return a.acc_id_sala() < b.acc_id_sala();
		}
	};

	struct Ordenador_mostrar
	{
		Ordenador_mostrar() {}

		bool operator()(const Mapa_sala * a, const Mapa_sala * b) const
		{
			int az=a->acc_z(), bz=b->acc_z();

			if(az==bz)
			{
				return a->acc_id_sala() < b->acc_id_sala();
			}
			else
			{
				return az < bz;
			}
		}
	};

	static const unsigned int REJILLA_X=16;
	static const unsigned int REJILLA_Y=16;
	static const unsigned int Z_DEFECTO=20;
	enum estados {E_SALAS, E_CELDAS};
	enum flags {T_MUROS, T_PUERTAS, T_ICONOS};

	DLibV::Pantalla& pantalla;
	DLibI::Controles_SDL controles_sdl;
	std::vector<Mapa_sala> salas;
	std::vector<Mapa_sala*> mostrando;

	unsigned int estado;
	unsigned int sala_actual;
	unsigned int celda_actual;
	unsigned int id_zona_actual;
	unsigned int z_actual;

	//////////////////////////

	void dibujar_sala(Mapa_sala& s, unsigned int indice_sala)
	{
		if(s.acc_z() != z_actual) return;

		unsigned int celdas=s.total_celdas();
		unsigned int i=0;

		int offset_x=REJILLA_X * s.acc_x();
		int offset_y=REJILLA_Y * s.acc_y();
		unsigned int id_sala=s.acc_id_sala();

		while(i < celdas)
		{
			Mapa_celda mc=s.obtener_celda(i);
			dibujar_celda(mc, offset_x, offset_y, indice_sala, i);
			++i;
		}

		if(indice_sala==sala_actual)
		{
			DLibV::Primitiva_grafica_caja_estatica c(
				DLibH::Herramientas_SDL::nuevo_sdl_rect(offset_x, offset_y, 10, 10),
				DLibV::Gestor_color::color(200, 32, 32));
			c.establecer_alpha(128);
			c.volcar(pantalla);
		}

		std::string texto=DLibH::Herramientas::entero_a_cadena(id_sala);
		DLibV::Representacion_texto_fija_estatica txt(DLibV::Gestor_recursos_graficos::obtener(3), texto, 80, 20);
		txt.establecer_posicion(offset_x, offset_y);
		txt.mut_interletra(1);
		txt.volcar(pantalla);
	}

	void dibujar_celda(Mapa_celda& mc, int offset_x, int offset_y, unsigned int indice_sala, unsigned int indice)
	{
		DLibV::Representacion_bitmap_estatica bmp;
		bmp.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(7));
		bmp.establecer_posicion(
			(mc.acc_x() * REJILLA_X) + offset_x, 
			(mc.acc_y() * REJILLA_Y) + offset_y);

		Uint32 color=0;

		if(estado==E_CELDAS && indice_sala==sala_actual && indice==celda_actual)
		{
			color=DLibV::Gestor_color::color(200, 32, 32);
		}	
		else
		{
			color=DLibV::Gestor_color::color(200, 200, 200);
		}

		DLibV::Primitiva_grafica_caja_estatica c(
			DLibH::Herramientas_SDL::nuevo_sdl_rect(
				(mc.acc_x() * REJILLA_X) + offset_x, (mc.acc_y() * REJILLA_Y) + offset_y,
				REJILLA_X, REJILLA_Y),
			color);
		c.volcar(pantalla);

		dibujar_flags(bmp, mc.acc_flags_muro(), Mapa_celda::OFFSET_MUROS);
		dibujar_flags(bmp, mc.acc_flags_puerta(), Mapa_celda::OFFSET_PUERTAS);
		dibujar_flags(bmp, mc.acc_flags_iconos(), Mapa_celda::OFFSET_ICONOS);
	}

	void dibujar_flags(DLibV::Representacion_bitmap_estatica& bmp, unsigned short int flags, unsigned int offset)
	{
		unsigned short flag=1;
		int i=0;

		while(flag < Mapa_celda::MAX_FLAG)
		{
			if(flags & flag)
			{
				bmp.establecer_recorte((offset+i) * REJILLA_X, 0, REJILLA_X, REJILLA_Y); 
				bmp.volcar(pantalla);
			}

			++i;
			flag=pow(2, i);
		}
	}

	void dibujar()
	{
		unsigned int visibles=mostrando.size();
		unsigned int total=salas.size();
		unsigned int celdas=visibles ? mostrando[sala_actual]->total_celdas() : 0; 
		unsigned int z=total ? mostrando[sala_actual]->acc_z() : 0;
		unsigned int id_sala=total ? mostrando[sala_actual]->acc_id_sala() : 0;
		unsigned int i=0;

		pantalla.rellenar(0,0,0);

		while(i < visibles)
		{
			dibujar_sala(*mostrando[i], i);
			++i;
		}

		std::string modo=estado==E_SALAS ? "SALAS" : "CELDAS";
		std::string texto=
			modo+" >> "+
			"Zona: "+DLibH::Herramientas::entero_a_cadena(id_zona_actual)+
			" Z: "+DLibH::Herramientas::entero_a_cadena(z_actual)+
			" id: "+DLibH::Herramientas::entero_a_cadena(id_sala)+
			" z: "+DLibH::Herramientas::entero_a_cadena(z)+
			"\nT: "+DLibH::Herramientas::entero_a_cadena(visibles)+
			"S: "+DLibH::Herramientas::entero_a_cadena(sala_actual)+" / "+
			DLibH::Herramientas::entero_a_cadena(total)+" C: "+
			DLibH::Herramientas::entero_a_cadena(celda_actual)+" / "+
			DLibH::Herramientas::entero_a_cadena(celdas);
		DLibH::Herramientas::entero_a_cadena(z);

		DLibV::Representacion_texto_fija_estatica txt(DLibV::Gestor_recursos_graficos::obtener(3), texto, 600, 20);
			txt.establecer_posicion(0, 0);
			txt.volcar(pantalla);

		pantalla.flipar();
	}

	void insertar_sala()
	{
		unsigned int id_sala=salas.size() ? salas[salas.size()-1].acc_id_sala()+1 : 1;
		salas.push_back(Mapa_sala(id_sala, id_zona_actual, 4, 4, Z_DEFECTO));
		actualizar_mostrando();
		sala_actual=mostrando.size()-1;
//std::cout<<"ACT: "<<sala_actual<<std::endl;
//		insertar_celda();	
	}

	void eliminar_sala()
	{
		if(mostrando.size())
		{
			std::vector<Mapa_sala*>::iterator act=mostrando.begin()+sala_actual;		
			unsigned int id_sala=(*act)->acc_id_sala();

			std::vector<Mapa_sala>::iterator it=salas.begin();
			while(it < salas.end())
			{
				Mapa_sala& s=*it;
				if(s.acc_id_sala()==id_sala)
				{
					salas.erase(it);
					break;
				}
				++it;
			}

			actualizar_mostrando();
			sala_actual=0;

		}
	}

	void insertar_celda()
	{
		if(mostrando.size())
		{
			mostrando[sala_actual]->insertar_celda(Mapa_celda(0, 0, 0, 0, 0));
			celda_actual=mostrando[sala_actual]->total_celdas()-1;
//std::cout<<"CELDA ACT "<<celda_actual<<std::endl;
		}
	}

	void eliminar_celda()
	{
		if(mostrando[sala_actual]->total_celdas() > 1)
		{
			mostrando[sala_actual]->eliminar_celda(celda_actual);
			celda_actual=0;
		}
	}

	void actualizar_mostrando()
	{
		mostrando.clear();

		for(Mapa_sala& s : salas)
		{
			if(s.acc_id_zona() == id_zona_actual)
			{
				mostrando.push_back(&s);
			}
		}

		Ordenador_mostrar ordenador;
		std::sort(mostrando.begin(), mostrando.end(), ordenador);
	}

	void exportar_a_texto()
	{
		std::vector<Mapa_sala> copia=salas;
		Ordenador_guardar ordenador;
		std::sort(copia.begin(), copia.end(), ordenador);

		std::ofstream archivo("mapa.txt");
	
		archivo<<"#idsala\tidzona\tx\ty\tz\n#x\ty\tflags\n";

		for(Mapa_sala& m : copia)
		{
			archivo<<m.acc_id_sala()<<"\t"<<
				m.acc_id_zona()<<"\t"<<
				m.acc_x()<<"\t"<<
				m.acc_y()<<"\t"<<
				m.acc_z()<<"\n";
		
			std::vector<Mapa_celda> celdas=m.obtener_celdas();

			for(Mapa_celda& c : celdas)
			{
				archivo<<
				c.acc_x()<<"\t"<<
				c.acc_y()<<"\t"<<
				c.acc_flags_muro()<<"\t"<<
				c.acc_flags_puerta()<<"\t"<<
				c.acc_flags_iconos()<<"\n";
			}

			archivo<<"[F]"<<"\n";
		}

		archivo.close();

	}

	void mover_sala(int px, int py)
	{
		if(!salas.size()) return;
		Mapa_sala& m=*mostrando[sala_actual];
		int x=m.acc_x() + px;
		int y=m.acc_y() + py;

		if(x < 0) x=m.acc_x();
		if(y < 0) y=m.acc_y();

		m.mut_x(x);
		m.mut_y(y);
	}

	void mover_celda(int px, int py)
	{
		if(!salas.size()) return;
		Mapa_sala& m=*mostrando[sala_actual];
		int total=mostrando[sala_actual]->total_celdas();
		if(!total) return;
		Mapa_celda& c=m.obtener_celda_para_modificar(celda_actual);

		int x=c.acc_x() + px;
		int y=c.acc_y() + py;

		if(x < 0) x=c.acc_x();
		if(y < 0) y=c.acc_y();

		c.mut_x(x);
		c.mut_y(y);		
	}

	void escoger_sala(int p)
	{
		int f=sala_actual + p;
		int total=mostrando.size();

		if(f >= 0 && f < total)
		{
			sala_actual=f;
			celda_actual=0;
		}
	}

	void escoger_celda(int p)
	{
		int total=mostrando[sala_actual]->total_celdas();
		if(!total) return;
		int f=celda_actual + p;
		if(f >= 0 && f < total)
		{
			celda_actual=f;
		}  
	}

	void escoger_z(int p)
	{
		if(!mostrando.size()) return;
		Mapa_sala& m=*mostrando[sala_actual];
		int z=m.acc_z() + p;
		if(z < 0) z=m.acc_z();
		m.mut_z(z);
		actualizar_mostrando();
	}

	void escoger_z_actual(int p)
	{
		int z=z_actual + p;
		if(z < 0) z=z_actual;
		z_actual=z;
	}

	void cambiar_id(int p)
	{
		if(!mostrando.size()) return;
		Mapa_sala& m=*mostrando[sala_actual];
		int id=m.acc_id_sala() + p;
		if(id < 0) id=m.acc_id_sala();
		m.mut_id_sala(id);
	}

	void cambiar_zona(int p)
	{
		int id_z=id_zona_actual + p;
		if(id_z < 0) id_z=id_zona_actual;
		id_zona_actual=id_z;
		actualizar_mostrando();
		sala_actual=0;
		celda_actual=0;
	}

	void cambiar_flag(unsigned int f, unsigned int tipo)
	{
		if(!salas.size()) return;
		Mapa_sala& m=*mostrando[sala_actual];
		Mapa_celda& c=m.obtener_celda_para_modificar(celda_actual);

		unsigned long int flags=0;

		switch(tipo)
		{
			case T_MUROS: flags=c.acc_flags_muro(); break;
			case T_PUERTAS: flags=c.acc_flags_puerta(); break;
			case T_ICONOS: flags=c.acc_flags_iconos(); break;
		}

		flags=flags^f;

		switch(tipo)
		{
			case T_MUROS: c.mut_flags_muro(flags); break;
			case T_PUERTAS: c.mut_flags_puerta(flags); break;
			case T_ICONOS: c.mut_flags_iconos(flags); break;
		}
	}

	void procesar_controles()
	{
		if(controles_sdl.es_tecla_down(SDLK_F1)) exportar_a_texto();

		switch(estado)
		{
			case E_SALAS:
				if(controles_sdl.es_tecla_down(SDLK_UP)) mover_sala(0, -1);
				else if(controles_sdl.es_tecla_down(SDLK_DOWN)) mover_sala(0, 1);
				else if(controles_sdl.es_tecla_down(SDLK_LEFT)) mover_sala(-1, 0);
				else if(controles_sdl.es_tecla_down(SDLK_RIGHT)) mover_sala(1, 0);
				else if(controles_sdl.es_tecla_down(SDLK_PAGEUP)) escoger_sala(-1);
				else if(controles_sdl.es_tecla_down(SDLK_PAGEDOWN)) escoger_sala(1);
				else if(controles_sdl.es_tecla_down(SDLK_MINUS)) escoger_z_actual(-1);
				else if(controles_sdl.es_tecla_down(SDLK_PLUS)) escoger_z_actual(1);
				else if(controles_sdl.es_tecla_down(SDLK_z)) escoger_z(-1);
				else if(controles_sdl.es_tecla_down(SDLK_x)) escoger_z(1);
				else if(controles_sdl.es_tecla_down(SDLK_a)) cambiar_zona(-1);
				else if(controles_sdl.es_tecla_down(SDLK_s)) cambiar_zona(1);
				else if(controles_sdl.es_tecla_down(SDLK_q)) cambiar_id(-1);
				else if(controles_sdl.es_tecla_down(SDLK_w)) cambiar_id(1);
				else if(controles_sdl.es_tecla_down(SDLK_n)) insertar_sala();
				else if(controles_sdl.es_tecla_down(SDLK_DELETE)) eliminar_sala();
				else if(controles_sdl.es_tecla_down(SDLK_SPACE)) estado=E_CELDAS;
			break;

			case E_CELDAS:
				if(controles_sdl.es_tecla_down(SDLK_UP)) mover_celda(0, -1);
				else if(controles_sdl.es_tecla_down(SDLK_DOWN)) mover_celda(0, 1);
				else if(controles_sdl.es_tecla_down(SDLK_LEFT)) mover_celda(-1, 0);
				else if(controles_sdl.es_tecla_down(SDLK_RIGHT)) mover_celda(1, 0);
				else if(controles_sdl.es_tecla_down(SDLK_PAGEUP)) escoger_celda(-1);
				else if(controles_sdl.es_tecla_down(SDLK_PAGEDOWN)) escoger_celda(1);
				else if(controles_sdl.es_tecla_down(SDLK_n)) insertar_celda();
				else if(controles_sdl.es_tecla_down(SDLK_DELETE)) eliminar_celda();
				else if(controles_sdl.es_tecla_down(SDLK_q)) cambiar_flag(Mapa_celda::M_PARED_NE, T_MUROS);
				else if(controles_sdl.es_tecla_down(SDLK_w)) 
				{
					if(controles_sdl.es_tecla_pulsada(SDLK_LSHIFT)) cambiar_flag(Mapa_celda::P_PUERTA_N, T_PUERTAS);
					else if(controles_sdl.es_tecla_pulsada(SDLK_LCTRL)) cambiar_flag(Mapa_celda::P_PUERTA_ZONA_N, T_PUERTAS);
					else cambiar_flag(Mapa_celda::M_PARED_N, T_MUROS);
				}
				else if(controles_sdl.es_tecla_down(SDLK_e)) cambiar_flag(Mapa_celda::M_PARED_NO, T_MUROS);
				else if(controles_sdl.es_tecla_down(SDLK_a)) 
				{
					if(controles_sdl.es_tecla_pulsada(SDLK_LSHIFT)) cambiar_flag(Mapa_celda::P_PUERTA_E, T_PUERTAS);
					else if(controles_sdl.es_tecla_pulsada(SDLK_LCTRL)) cambiar_flag(Mapa_celda::P_PUERTA_ZONA_E, T_PUERTAS);
					else cambiar_flag(Mapa_celda::M_PARED_E, T_MUROS);
				}
				else if(controles_sdl.es_tecla_down(SDLK_d)) 
				{
					if(controles_sdl.es_tecla_pulsada(SDLK_LSHIFT)) cambiar_flag(Mapa_celda::P_PUERTA_O, T_PUERTAS);
					else if(controles_sdl.es_tecla_pulsada(SDLK_LCTRL)) cambiar_flag(Mapa_celda::P_PUERTA_ZONA_O, T_PUERTAS);
					else cambiar_flag(Mapa_celda::M_PARED_O, T_MUROS);
				}
				else if(controles_sdl.es_tecla_down(SDLK_z)) cambiar_flag(Mapa_celda::M_PARED_SE, T_MUROS);
				else if(controles_sdl.es_tecla_down(SDLK_x))
				{
					if(controles_sdl.es_tecla_pulsada(SDLK_LSHIFT)) cambiar_flag(Mapa_celda::P_PUERTA_S, T_PUERTAS);
					else if(controles_sdl.es_tecla_pulsada(SDLK_LCTRL)) cambiar_flag(Mapa_celda::P_PUERTA_ZONA_S, T_PUERTAS);
					else cambiar_flag(Mapa_celda::M_PARED_S, T_MUROS);
				}
				else if(controles_sdl.es_tecla_down(SDLK_c)) cambiar_flag(Mapa_celda::M_PARED_SO, T_MUROS);
				else if(controles_sdl.es_tecla_down(SDLK_1)) cambiar_flag(Mapa_celda::I_COFRE, T_ICONOS);
				else if(controles_sdl.es_tecla_down(SDLK_2)) cambiar_flag(Mapa_celda::I_FLECHA, T_ICONOS);
				else if(controles_sdl.es_tecla_down(SDLK_3)) cambiar_flag(Mapa_celda::I_PORTAL, T_ICONOS);
				else if(controles_sdl.es_tecla_down(SDLK_4)) cambiar_flag(Mapa_celda::I_PUNTO_GUARDADO, T_ICONOS);
				else if(controles_sdl.es_tecla_down(SDLK_5)) cambiar_flag(Mapa_celda::I_PUNTO_TIEMPO, T_ICONOS);
				else if(controles_sdl.es_tecla_down(SDLK_SPACE)) estado=E_SALAS;
			break;
		}
	}

	public:

	Editor(DLibV::Pantalla& pp): 
		pantalla(pp), estado(E_SALAS), sala_actual(0), celda_actual(0),
		id_zona_actual(0), z_actual(Z_DEFECTO)
	{
		Cargador_mapa cm;
		cm.inicializar();
		const std::map<unsigned int, Mapa_sala> mapa=cm.acc_mapa().acc_mapa();
		std::map<unsigned int, Mapa_sala>::const_iterator ini=mapa.begin();

		while(ini != mapa.end())
		{
			Mapa_sala s=(*ini).second; //Copia...
			salas.push_back(s);	//Recopia.
			++ini;
		}

		actualizar_mostrando();
	}

	void loop()
	{
		while(true)
		{
			controles_sdl.recoger();

			if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
			{
				return;
			}
			else
			{
				procesar_controles();
				dibujar();
			}
		}
	}
};

int main(int argc, char ** argv)
{
	if(DLibH::Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK))
	{
		LOG.inicializar("data/logs/mapa.log");
		LOG.activar();
		DLibH::Log_motor::arrancar("data/logs/log_motor.log");

		//Inicializar sistema gráfico y pantalla...
		DLibV::Pantalla pantalla;
		pantalla.inicializar(600, 400, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT);
		pantalla.establecer_titulo("--== MAPEADOR ==--");

		DLibV::Utilidades_graficas_SDL::mostrar_ocultar_cursor(true);
		DLibV::Gestor_color::establecer_formato(pantalla.obtener_formato_pixeles());

		//Inicializar info...
		Cargador_recursos cargador_recursos;
		cargador_recursos.generar_recursos_graficos();

		//Visor
		Editor e(pantalla);
		e.loop();
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
