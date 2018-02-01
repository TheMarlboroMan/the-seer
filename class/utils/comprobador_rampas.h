#ifndef UTIL_COMPROBADOR_RAMPAS
#define UTIL_COMPROBADOR_RAMPAS

#include <fstream>
#include "../sala.h"

/*
Se le pasan mapas y por cada tile de tipo rampa comprueba que la siguiente es 
de tipo tope de rampa...
*/

class Comprobador_rampas
{
	private:

	std::ofstream archivo; //Donde se escriben los resultados.

	struct Apuntador
	{
		unsigned int x, y, z;
		Apuntador(unsigned int px, unsigned int py, unsigned int pz):
			x(px), y(py), z(pz) {}

		Apuntador(): x(0), y(0), z(0) {}
	};

	Apuntador obtener_posicion_tope(const Bloque& bl)
	{
		Apuntador resultado(bl.acc_matriz_x(), bl.acc_matriz_y(), bl.acc_matriz_z());

		switch(bl.acc_tipo_comportamiento())
		{
			case Bloque::TC_RAMPA_O_E: 
			case Bloque::TC_RAMPA_NO_ESCALABLE_O_E: 
				resultado.x++; break;
			case Bloque::TC_RAMPA_E_O: 
			case Bloque::TC_RAMPA_NO_ESCALABLE_E_O: 
				resultado.x--; break;
			case Bloque::TC_RAMPA_S_N: 
			case Bloque::TC_RAMPA_NO_ESCALABLE_S_N: 
				resultado.y--; break;
			case Bloque::TC_RAMPA_N_S:
 			case Bloque::TC_RAMPA_NO_ESCALABLE_N_S: 
				resultado.y++; break;
		}

		return resultado;
	}

	public:

	Comprobador_rampas()
	{
		archivo.open("resultados_rampas.log", std::ios::out);
	}

	~Comprobador_rampas()
	{
		archivo.close();
	}

	bool comprobar() const {return archivo.is_open();}

	void procesar_sala(Sala& s)
	{
		unsigned int w=s.acc_w();
		unsigned int h=s.acc_h();
		unsigned int d=s.acc_d();
		unsigned int x=0, y=0, z=0;
		std::string resultados;
		unsigned int rampas=0;
		unsigned int errores=0;

		for(x=0; x<w; ++x)
		{
			for(y=0; y<d; ++y)
			{
				for(z=0; z<h; ++z)
				{
					const Bloque& bl=s.obtener_bloque(x, y, z);
					if(bl.es_tipo_rampa())
					{
						++rampas;

						Apuntador ap=obtener_posicion_tope(bl);
						const Bloque& tope=s.obtener_bloque(ap.x, ap.y, ap.z);
			
						if(tope.es_bloque_error()) {}
						else if(!tope.es_solido_rampa())
						{
							++errores;
							resultados+="\n"+
								DLibH::Herramientas::entero_a_cadena(ap.x)+","+
								DLibH::Herramientas::entero_a_cadena(ap.y)+","+
								DLibH::Herramientas::entero_a_cadena(ap.z)+" debe ser tope rampa para "+
								DLibH::Herramientas::entero_a_cadena(x)+","+
								DLibH::Herramientas::entero_a_cadena(y)+","+
								DLibH::Herramientas::entero_a_cadena(z)+" actual como "+
								DLibH::Herramientas::entero_a_cadena(tope.acc_tipo_comportamiento());
		;
						}
						
					}
				}
			}
		}

		if(errores)
		{
			archivo<<"** Sala "<<DLibH::Herramientas::entero_a_cadena(s.acc_id())<<
" ** Rampas: "<<DLibH::Herramientas::entero_a_cadena(rampas)<<
" ** Errores: "<<DLibH::Herramientas::entero_a_cadena(errores)<<
"\n"<<resultados<<"\n\n";
		}
	}
};

#endif
