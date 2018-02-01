#include "deserializador.h"

Deserializador::Deserializador(unsigned int indice_archivo):
		archivo(Definiciones_serializador_deserializador::construir_ruta_archivo(indice_archivo), std::ios::in),
		version(0), id_sala(0), id_entrada(0), error(false)
{

}

Deserializador::~Deserializador()
{
}

bool Deserializador::es_fin_seccion(const std::string& linea)
{
	return linea.find(Definiciones_serializador_deserializador::SEPARADOR_SECCION)!=std::string::npos
	|| archivo.eof();
}

bool Deserializador::comprobar_inicio_seccion(const std::string& linea, const char codigo, bool marcar)
{
	std::string pieza=linea.substr(0, 3);
	
	bool resultado=Definiciones_serializador_deserializador::comprobar_marca(pieza, codigo);

	if(!marcar)
	{
		return resultado;
	}
	else
	{
		error=!resultado;
		return resultado;
	}
}

std::string Deserializador::extraer_cadena_seccion(const std::string& linea)
{
	std::string resultado(linea.substr(3));
	return resultado;	
}

bool Deserializador::es_valido() const
{
	return archivo.is_open();
}
 
void Deserializador::finalizar()
{
	archivo.close();
}

void Deserializador::iniciar()
{
	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
	if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_INICIO)) 
	{
		return;
	}

	int paso=0;

	while(!es_fin_seccion(linea))
	{

		switch(paso)
		{
			case 0:
			{
				std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(
				extraer_cadena_seccion(linea), 
				Definiciones_serializador_deserializador::SEPARADOR_DATOS
				);

				if(v.size()!=3)
				{
					LOG<<"ERROR: Al iniciar deserializacion no hay 3 parametros";
				}
				else
				{
					version=v[0];
					id_sala=v[1];
					id_entrada=v[2];
				}
			}
			break;
			case 1:
				nombre=linea; 
			break;
		}

		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
		++paso;
	}
}

void Deserializador::deserializar(Info_juego& c)
{
	if(es_error()) return;

	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

	while(!es_fin_seccion(linea))
	{
		if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_INFO_JUEGO)) 
		{
			return;
		}

		std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(
			extraer_cadena_seccion(linea), 
			Definiciones_serializador_deserializador::SEPARADOR_DATOS
			);

		if(v.size()!=3)
		{
			LOG<<"ERROR: deserializacion info juego no hay 3 parametros";
		}
		else
		{
			c.actualizar_vidas_perdidas(v[0]);
			c.actualizar_llaves(v[1]);
			c.actualizar_bonus_tiempo(v[2]);
		}

		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
	}
}

void Deserializador::deserializar(Tiempo_juego& c)
{
	if(es_error()) return;

	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

	while(!es_fin_seccion(linea))
	{
		if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_TIEMPO_JUEGO)) 
		{
			return;
		}

		unsigned int segundos=DLibH::Herramientas::cadena_a_entero(extraer_cadena_seccion(linea));
		c.establecer_segundos_restantes(segundos);

		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
	}
}

void Deserializador::deserializar(Control_salas& c)
{
	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

	unsigned int num_linea=0;

	while(!es_fin_seccion(linea))
	{
		switch(num_linea)
		{
			case 0:
			{
				if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_CONTROL_SALAS))
				{
					return;
				}

				std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(
					extraer_cadena_seccion(linea), 
					Definiciones_serializador_deserializador::SEPARADOR_DATOS);

				for(unsigned int i : v) c.descubrir_sala(i);
			}
			break;

			case 1:
			{		
				std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(
					linea, 
					Definiciones_serializador_deserializador::SEPARADOR_DATOS);

				for(unsigned int i : v) c.marcar_gemas(i, true);
			}
			break;

			default: break;
		}

		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
		++num_linea;
	
/*
		if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_CONTROL_SALAS))
		{
			return;
		}

		std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(
			extraer_cadena_seccion(linea), 
			Definiciones_serializador_deserializador::SEPARADOR_DATOS
			);

		for(unsigned int i : v) 
		{
			c.descubrir_sala(i);
		}

		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
*/
	}
}

void Deserializador::deserializar(Control_objetivos& c)
{
	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

	while(!es_fin_seccion(linea))
	{
		if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_CONTROL_OBJETIVOS))
		{
			return;
		}

		std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(
			extraer_cadena_seccion(linea), 
			Definiciones_serializador_deserializador::SEPARADOR_DATOS
			);

		unsigned int i=0;
		unsigned int indice=0;

		//Primero los objetivos

		while(i<Control_objetivos::T_MAX)
		{
			unsigned int cuenta=v[indice++];
			unsigned int total=v[indice++];

			c.contar(i, cuenta);
			c.sumar_total(i, total);
			++i;
		}

		//Y luego los mensajes.
		i=0;
		while(i<Control_objetivos::M_MAX)
		{
			unsigned int val=v[indice++];
			if(val) c.marcar_mensaje_mostrado(i);
			else c.desmarcar_mensaje_mostrado(i);
			++i;
		}

		//Luego los mensajes...

		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
	}
}

void Deserializador::deserializar(Control_puzzle& c)
{
	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

	while(!es_fin_seccion(linea))
	{
		if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_CONTROL_PUZZLE))
		{
			return;
		}

		linea=extraer_cadena_seccion(linea);

		if(linea.size())
		{
			std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(
				linea,
				Definiciones_serializador_deserializador::SEPARADOR_DATOS
				);


			LOG<<"Deserializar "<<v.size()<<" piezas "<<std::endl;
			for(unsigned int i : v) c.recoger_pieza(i);	
		}

		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
	}
}

void Deserializador::deserializar(Control_energia& c)
{
	if(es_error()) return;

	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

	while(!es_fin_seccion(linea))
	{
		if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_CONTROL_ENERGIA)) 
		{
			return;
		}

		unsigned int tanques=DLibH::Herramientas::cadena_a_entero(extraer_cadena_seccion(linea));
		unsigned int i=0;

		while(i<tanques)
		{
			c.nuevo_tanque();
			++i;
		}

		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
	}
}

void Deserializador::deserializar(Control_habilidades& c)
{
	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

	while(!es_fin_seccion(linea))
	{
		if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_CONTROL_HABILIDADES))
		{
			return;
		}

		std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(
			extraer_cadena_seccion(linea), 
			Definiciones_serializador_deserializador::SEPARADOR_DATOS
			);

		LOG<<"Deserializar "<<v.size()<<" entradas: activas + habilidades. La linea es :"<<linea<<std::endl;

		bool primero=true;

		for(unsigned int i : v)
		{
			if(primero)
			{	
				LOG<<"Activar habilidades?"<<i<<std::endl;
				primero=false;
				if(i) c.activar_habilidades();
				else c.desactivar_habilidades();
			}
			else
			{
				LOG<<"Concediendo habilidad "<<i<<std::endl;
				c.conceder_habilidad(i);
			}
		}

		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
	}
}

void Deserializador::deserializar(Controlador_datos_persistentes& c)
{
	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

	while(!es_fin_seccion(linea))
	{
		if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_CONTROL_DATOS_PERSISTENTES))
		{
			return;
		}

		linea=extraer_cadena_seccion(linea);

		if(linea.size())
		{
			const std::vector<std::string> partes=DLibH::Herramientas::explotar(
				linea, 
				Definiciones_serializador_deserializador::SEPARADOR_DATOS);

			for(const std::string& cad : partes)
			{
				std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(cad, Definiciones_serializador_deserializador::SEPARADOR_INTERNO);
				c.actualizar_valor(v[0], v[1]);			
			}
		}
	
		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
	}
}

void Deserializador::deserializar(Control_actores_persistentes& c)
{
	unsigned int id_sala=0;
	unsigned int total_sala=0;
	unsigned int indice=0;
	
	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

	if(!comprobar_inicio_seccion(linea, Definiciones_serializador_deserializador::M_CONTROL_ACTORES_PERSISTENTES))
	{
		return;
	}
	else
	{
		linea=extraer_cadena_seccion(linea);
	}

	//No hay propiedad de los actores: se servirán al controlador, que
	//se adueñará de ellos.	
	std::vector<Actor *> actores;

	while(!es_fin_seccion(linea))
	{
LOG<<"Deserializando linea "<<linea<<std::endl;

		if(indice==total_sala)
		{
			if(id_sala) 
			{
LOG<<"Insertando "<<actores.size()<<" actores en sala "<<id_sala<<std::endl;
				c.insertar_actores(id_sala, actores);
			}

			std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(linea, Definiciones_serializador_deserializador::SEPARADOR_DATOS);

			actores.clear();
			id_sala=v[0];
			total_sala=v[1];
			indice=0;

LOG<<"Se lee la cabecera de "<<total_sala<<" actores en sala "<<id_sala<<std::endl;
		}
		else
		{
			std::vector<unsigned int> v=Utilidades_cargadores::explotar_linea_a_enteros(linea, Definiciones_serializador_deserializador::SEPARADOR_DATOS_ACTORES);

			const unsigned int id_tipo=v[0];
			Factoria_actores::Parametros_factoria params(v[1], v[2], v[3]);
			unsigned int ip=4;
			while(ip < v.size()) 
			{
				params.insertar_param(v[ip]);
				ip++;
			}

			actores.push_back(Factoria_actores::fabricar(id_tipo, params));
			++indice;
		}

		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
	}
}

bool Deserializador::localizar_seccion(const char clave)
{
	//Rebobinar...

	archivo.seekg(std::ios_base::beg);

	int pos=archivo.tellg();
	std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

	while(!archivo.eof())
	{
		if(comprobar_inicio_seccion(linea, clave, false))
		{
			archivo.seekg(pos);
			return true;	
		}

		pos=archivo.tellg();
		linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
	}

	return false;
}
