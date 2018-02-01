#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <exception>

#include "libdan_headers/libDanSDL.h"
#include "libdan_headers/defDanSDL.h"

#include "class/cargador_mapa.h"
#include "class/datos_mapa.h"

DLibH::Log_base LOG;

class Zonas_mapa
{
	private:

	struct Ordenador_guardar
	{
		Ordenador_guardar()
		{}

		bool operator()(const Mapa_sala& a, const Mapa_sala& b) const
		{
			return a.acc_id_sala() < b.acc_id_sala();
		}
	};

	std::vector<Mapa_sala> salas;
	std::vector<Mapa_sala*> mostrando;
	std::vector<unsigned int> zonas_usadas;

	//////////////////////////

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
				c.acc_flags_iconos()<<"\t\n";
			}

			archivo<<"[F]"<<"\n";
		}

		archivo.close();

	}

	void calcular_zonas_usadas()
	{
		zonas_usadas.clear();

		for(const Mapa_sala& s : salas)
		{
			unsigned int id_zona=s.acc_id_zona();
			if(std::find(zonas_usadas.begin(), zonas_usadas.end(), id_zona)==zonas_usadas.end())
			{
				zonas_usadas.push_back(id_zona);
			}
		}

		std::sort(zonas_usadas.begin(), zonas_usadas.end());
	}

	void intercambiar_zonas(unsigned int original, unsigned int nuevo)
	{
		for(Mapa_sala& s : salas)
		{
			unsigned int id_zona=s.acc_id_zona();
			if(id_zona==original) s.mut_id_zona(nuevo);
		}
		calcular_zonas_usadas();
	}

	public:

	Zonas_mapa()
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

		calcular_zonas_usadas();
	}

	void loop()
	{
		bool salir=false;
		unsigned int opcion=0;
		unsigned int original=0;
		unsigned int nuevo=0;

		while(!salir)
		{
			std::cout<<"ZONAS USADAS: "<<std::endl;
			for(unsigned int i: zonas_usadas) std::cout<<i<<std::endl;
			std::cout<<std::endl<<"0 - Salir, 1 - Reordenar, 2 - Guardar"<<std::endl<<">>";

			std::cin>>opcion;

			switch(opcion)
			{
				case 0: salir=true; break;
				case 1:
					std::cout<<"Original: >>";
					std::cin>>original;
					std::cout<<"Nuevo: >>";
					std::cin>>nuevo;

					intercambiar_zonas(original, nuevo);
				break;
				case 2: exportar_a_texto(); break;
			}
		}
	}
};

int main(int argc, char ** argv)
{
	LOG.inicializar("data/logs/zonas_mapa.log");
	LOG.activar();
	DLibH::Log_motor::arrancar("data/logs/log_motor.log");

	Zonas_mapa z;
	z.loop();

	DLibH::Log_motor::finalizar();

	return 0;
}
