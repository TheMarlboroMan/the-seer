#include "control_pieza_puzzle.h"

extern DLibH::Log_base LOG;

const std::string Control_pieza_puzzle::RUTA_INFO="data/info/datos_piezas.txt";
Controlador_piezas_puzzle Control_pieza_puzzle::controlador_piezas;


Definicion_pieza_puzzle Control_pieza_puzzle::obtener_pieza_desde_linea(const std::string& linea)
{
	const char separador='\t';
	unsigned int id=0;
	unsigned int idr=0;
	unsigned int x=0;
	unsigned int y=0;

	std::vector<unsigned int> d=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);
	if(d.size()!=4)
	{
		LOG<<"ERROR: La pieza de puzzle "<<linea<<" no tiene 4 parámetros"<<std::endl;
	}
	else
	{
		id=d[0];
		idr=d[1];
		x=d[2];
		y=d[3];
	}

	return Definicion_pieza_puzzle(id,idr,x,y);
}

/*Prepara tanto el cargador de piezas como el mapa de piezas disponibles...
Las piezas disponibles se inician a cero.*/

void Control_pieza_puzzle::preparar_datos()
{
	std::ifstream archivo(RUTA_INFO.c_str(), std::ios::in);

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de datos piezas de puzzle"<<std::endl;
	}
	else
	{
		controlador_piezas.vaciar();

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				

			if(linea.size())
			{
				Definicion_pieza_puzzle d=obtener_pieza_desde_linea(linea);

				unsigned int id=d.id_pieza;
				if(controlador_piezas.existe_entrada_por_id(id))
				{
					LOG<<"ERROR: Pieza "<<id<<" repetida en puzzle"<<std::endl;
				}
				else
				{
					controlador_piezas.insertar(id, d);
				}
			}
		}

		archivo.close();
	}

	LOG<<"Cargadas "<<controlador_piezas.size()<<" piezas de puzzle"<<std::endl;
}

Definicion_pieza_puzzle Control_pieza_puzzle::obtener_definicion_pieza(unsigned int idp)
{
	if(!controlador_piezas.existe_entrada_por_id(idp))
	{
		return Definicion_pieza_puzzle(0,0,0,0);
	}
	else
	{
		return controlador_piezas.obtener_por_id(idp);
	}
}

std::vector<unsigned int> Control_pieza_puzzle::obtener_vector_claves()
{
	return controlador_piezas.obtener_vector_claves();
}

unsigned int Control_pieza_puzzle::obtener_id_maximo()
{
	return controlador_piezas.acc_mapa().rbegin()->first;
}
