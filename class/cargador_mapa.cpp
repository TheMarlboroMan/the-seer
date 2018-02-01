#include "cargador_mapa.h"

const std::string Cargador_mapa::FICHERO_DATOS="data/info/datos_mapa.txt";
const std::string Cargador_mapa::DELIMITADOR="[F]";

Cargador_mapa::Cargador_mapa():
	estado(E_SALA)
{

}

void Cargador_mapa::inicializar()
{
	std::ifstream archivo(FICHERO_DATOS);

	Mapa_sala s(0,0,0,0,0);

	while(!archivo.eof())
	{
		std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

		if(linea.size())
		{
			std::vector<unsigned int> datos=Utilidades_cargadores::explotar_linea_a_enteros(linea, SEPARADOR);

			switch(estado)
			{
				case E_SALA:
					leer_linea_como_sala(datos, s);
					estado=E_CELDA;
				break;

				case E_CELDA:
					
					if(linea.find(DELIMITADOR)!=std::string::npos)
					{
						mapa.insertar(s.acc_id_sala(), s);
						estado=E_SALA;
						s.vaciar();
					}
					else
					{
						leer_linea_como_celda(datos, s);
					}
				break;
			}
		}
	}

	archivo.close();
}

/*
id_sala	id_zona	x	y	z
1	1	0	0	0
*/

void Cargador_mapa::leer_linea_como_sala(std::vector<unsigned int>& datos, Mapa_sala& s)
{
	if(datos.size() < 5)
	{
		LOG<<"ERROR: leer_linea_como_sala debe tener al 5 parametros"<<std::endl;
	}
	else
	{
		s.mut_id_sala(datos[0]);
		s.mut_id_zona(datos[1]);
		s.mut_x(datos[2]);
		s.mut_y(datos[3]);
		s.mut_z(datos[4]);
	}
}

/*
x	y	flags
0	0	3
*/

void Cargador_mapa::leer_linea_como_celda(std::vector<unsigned int>& datos, Mapa_sala& s)
{
	if(datos.size() == 3)
	{
		LOG<<"ATENCION: Usando migrador de tres parametros"<<std::endl;
		Mapa_celda m=Mapa_celda::migrar_desde_flags_unicas(datos[0], datos[1], datos[2]);
		s.insertar_celda(m);
	}
	else if(datos.size() < 5)
	{
		LOG<<"ERROR: leer_linea_como_celda debe tener 3 parametros"<<std::endl;
	}
	else
	{
		Mapa_celda m(datos[0], datos[1], datos[2], datos[3], datos[4]);
		s.insertar_celda(m);
	}
}

//Indispensable haberlo inicializado.

const Mapa_sala& Cargador_mapa::obtener_sala_por_id(unsigned int id)
{
	if(!mapa.existe_entrada_por_id(id))
	{
		throw Excepcion_mapa();
	}
	else
	{
		return mapa.obtener_por_id(id);
	}
}
