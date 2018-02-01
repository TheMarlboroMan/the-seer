#include "serializador.h"

Serializador::Serializador(unsigned int indice_archivo):
		archivo(Definiciones_serializador_deserializador::construir_ruta_archivo(indice_archivo), std::ios::out)
{

}

Serializador::~Serializador()
{
}

bool Serializador::es_valido() const
{
	return archivo.is_open();
}
 
void Serializador::nueva_seccion()
{
	archivo<<std::endl<<Definiciones_serializador_deserializador::SEPARADOR_SECCION<<std::endl;
}

void Serializador::finalizar()
{
	archivo.close();
}

void Serializador::iniciar(unsigned int id_sala, unsigned int id_entrada, const std::string& nombre)
{
	archivo<<
		Definiciones_serializador_deserializador::marcar(Definiciones_serializador_deserializador::M_INICIO)<<
		VERSION<<Definiciones_serializador_deserializador::SEPARADOR_DATOS<<
		id_sala<<Definiciones_serializador_deserializador::SEPARADOR_DATOS<<
		id_entrada<<std::endl<<
		nombre;
		
	nueva_seccion();
}

/*De info juego guardamos la cantidad de veces que te han liquidado y las 
llaves que tienes.*/

void Serializador::serializar(const Info_juego& i)
{
	archivo<<
		Definiciones_serializador_deserializador::marcar(Definiciones_serializador_deserializador::M_INFO_JUEGO)<<
			i.acc_vidas_perdidas()<<Definiciones_serializador_deserializador::SEPARADOR_DATOS<<
			i.acc_llaves()<<Definiciones_serializador_deserializador::SEPARADOR_DATOS<<
			i.acc_bonus_tiempo();

	nueva_seccion();
}

/*De aquí guardamos los segundos como cadena.*/

void Serializador::serializar(const Tiempo_juego& i)
{
	archivo<<
		Definiciones_serializador_deserializador::marcar(Definiciones_serializador_deserializador::M_TIEMPO_JUEGO)<<		
		i.acc_segundos_restantes();

	nueva_seccion();
}

/*Aquí guardamos la ristra de ids de salas descubiertas. En otra línea guardamos
los ids con sala que tienen gemas. Por defecto se asume que una sala no tiene
gemas.*/

void Serializador::serializar(const Control_salas& c)
{
	archivo<<
		Definiciones_serializador_deserializador::marcar(Definiciones_serializador_deserializador::M_CONTROL_SALAS);
	std::vector<unsigned int> va=c.obtener_vector_salas_descubiertas();
	std::vector<unsigned int> vb=c.obtener_vector_salas_con_gemas();

	auto metodo=[this](std::vector<unsigned int> &v)
	{
		bool primera=true;

		for(unsigned int i : v)
		{
			if(primera)
			{
				archivo<<i;
				primera=false;
			}
			else
			{
				archivo<<
					Definiciones_serializador_deserializador::SEPARADOR_DATOS<<
					i;
			}
		}
	};

	metodo(va);
	archivo<<std::endl;
	metodo(vb);	
	nueva_seccion();
}

/*Guardamos literalmente las líneas de totales y las de contadores, de seguido
y sin nuevas líneas.*/

void Serializador::serializar(const Control_objetivos& c)
{
	archivo<<
		Definiciones_serializador_deserializador::marcar(Definiciones_serializador_deserializador::M_CONTROL_OBJETIVOS);

	unsigned int i=0;

	//Primero los objetivos.
	while(i<Control_objetivos::T_MAX)
	{
		archivo<<
			c.obtener_contados(i)<<
			Definiciones_serializador_deserializador::SEPARADOR_DATOS<<
			c.obtener_total(i)<<
			Definiciones_serializador_deserializador::SEPARADOR_DATOS;
		++i;
	}

	//Luego los mensajes...
	i=0;
	while(i<Control_objetivos::M_MAX)
	{
		const unsigned int val=c.es_mensaje_mostrado(i);
		archivo<<val;
		++i;
		if(i!=Control_objetivos::M_MAX) archivo<<Definiciones_serializador_deserializador::SEPARADOR_DATOS;
	}

	nueva_seccion();
}

/*Guardar literalmente los ids de piezas recogidas.*/

void Serializador::serializar(const Control_puzzle& c)
{
	archivo<<
		Definiciones_serializador_deserializador::marcar(Definiciones_serializador_deserializador::M_CONTROL_PUZZLE);

	std::vector<unsigned int> v=c.obtener_piezas_recogidas();
	unsigned int i=0;
	unsigned int max=v.size()-1;

	for(unsigned int id : v)
	{
		archivo<<id;
		if(i!=max) archivo<<Definiciones_serializador_deserializador::SEPARADOR_DATOS;
		++i;
	}

	nueva_seccion();
}

/*Guardar un sólo número, la cantidad de tanques.*/

void Serializador::serializar(const Control_energia& c)
{
	archivo<<
		Definiciones_serializador_deserializador::marcar(Definiciones_serializador_deserializador::M_CONTROL_ENERGIA)<<
		c.obtener_cantidad_tanques();

	nueva_seccion();
}

/*Guardar primero un 1 o 0 para indicar que se posee o no el amuleto. 
Luego guardar los ids de las habilidades que se poseen.*/

void Serializador::serializar(const Control_habilidades& c)
{
	const unsigned int habilidades_activas=c.es_habilidades_activadas();

	archivo<<
		Definiciones_serializador_deserializador::marcar(Definiciones_serializador_deserializador::M_CONTROL_HABILIDADES)<<
		habilidades_activas;

	unsigned int i=0;

	while(i < Control_habilidades::H_MAX)
	{
		if(c.es_habilidad_concedida(i))
		{
			archivo<<
				Definiciones_serializador_deserializador::SEPARADOR_DATOS<<
				i;
		}

		++i;
	}

	nueva_seccion();
}

/*Guarda, de seguidos, el id y el valor de cada uno de los datos que sean
del tipo "leer al entrar". Cada par se separa de otro par por el separador 
estándar. Entre ellos, por el separador interno.*/

void Serializador::serializar(const Controlador_datos_persistentes& c)
{
	const std::vector<Dato_persistente> v=c.obtener_leer_al_entrar();
	bool primera=true;

	archivo<<
		Definiciones_serializador_deserializador::marcar(Definiciones_serializador_deserializador::M_CONTROL_DATOS_PERSISTENTES);

	for(const Dato_persistente& d : v)
	{
		if(!d.es_aplicable()) continue;

		if(primera)
		{
			primera=false;
			archivo<<
				d.acc_id_dato()<<
				Definiciones_serializador_deserializador::SEPARADOR_INTERNO<<
				d.acc_valor_actual();
		}
		else
		{
			archivo<<
				Definiciones_serializador_deserializador::SEPARADOR_DATOS<<
				d.acc_id_dato()<<
				Definiciones_serializador_deserializador::SEPARADOR_INTERNO<<
				d.acc_valor_actual();
		}
	}

	nueva_seccion();
}

/*Guarda, en cada sala, todos los actores del tipo persistente con las mismas
características con las que se define en la factoría. Para algunos actores hace
falta algo más de información (relativa al estado) que no tiene sentido en la
factoría (por ejemplo, el puzzle está o no resuelto). En estos casos guardaremos
la información extra en la línea y la factoría sabrá como identificarla.

La primera línea define el id de la sala y la cantidad de actores que hay en la 
misma. Luego van todos los actores, cada uno en una línea. Luego otra línea con
id sala y cantidad y así sucesivamente.
*/

void Serializador::serializar(const Control_actores_persistentes& c)
{
	std::vector<unsigned int> v=c.obtener_vector_salas_con_actores_persistentes();
	unsigned int cuenta=0;
	const unsigned int total=v.size();

	archivo<<
		Definiciones_serializador_deserializador::marcar(Definiciones_serializador_deserializador::M_CONTROL_ACTORES_PERSISTENTES);

	LOG<<"Hay "<<cuenta<<" salas con actores persistentes"<<std::endl;

	std::string sep;
	sep+=Definiciones_serializador_deserializador::SEPARADOR_DATOS_ACTORES;

	Visitante_serializacion vs(archivo, sep);
	
	for(unsigned int id_sala : v)
	{
		const std::vector<Actor *>& va=c.obtener_actores_persistentes_por_id(id_sala);
		unsigned int t_actores=va.size();

		++cuenta;

		LOG<<"Serializando "<<t_actores<<" actores persistentes de sala "<<id_sala<<" -> "<<cuenta<<" / "<<total<<std::endl;
		archivo<<id_sala<<Definiciones_serializador_deserializador::SEPARADOR_DATOS<<t_actores<<std::endl;
		LOG<<"Cabecera escrita, escribiendo actores"<<std::endl;

		unsigned int actores_escritos=0;		

		for(Actor * a : va) 
		{
			LOG<<"\t"<<(actores_escritos+1)<<" / "<<t_actores<<" : Pasando visitante para actor "<<a->string_me_about_yourself()<<std::endl;
			a->aceptar_visitante(vs);
			++actores_escritos;
		}

		LOG<<"Se han escrito "<<actores_escritos<<" actores en la sala"<<std::endl;
	}
	nueva_seccion();
}

void Serializador::realizar_copia_seguridad(unsigned int indice_archivo)
{
	std::string nombre_in=Definiciones_serializador_deserializador::construir_ruta_archivo(indice_archivo);
	std::ifstream archivo_in(nombre_in.c_str(), std::ios::in);

	if(!archivo_in) //Si no existe el archivo de input es que no podemos hacer la copia de seguridad.
	{
		LOG<<"No se encuentra archivo original "<<nombre_in<<" para realizar la copia de seguridad"<<std::endl;
		return;
	}
	else
	{		
		std::string nombre_out=nombre_in+".copia";
		std::ofstream archivo_out(nombre_out.c_str(), std::ios::out);

		if(!archivo_out) //En caso de fallo, fallamos silenciosamente, por muy mal que esté.
		{
			LOG<<"ERROR: No se ha podido crear archivo archivo de salida "<<nombre_out<<" para la copia de seguridad."<<std::endl;			
			return;
		}
		else
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo_in);

			while(!archivo_in.eof())
			{
				archivo_out<<linea<<std::endl;
				linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo_in);
			}

			archivo_out.close();
			archivo_in.close();
			LOG<<"Finalizando copia de seguridad en archivo "<<nombre_out<<std::endl;
		}
	}
}
