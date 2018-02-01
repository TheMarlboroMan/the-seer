#include "cargador_datos.h"

const std::string Cargador_datos::FICHERO_DATOS="data/info/datos.txt";

Cargador_datos::Cargador_datos()
{

}

Cargador_datos::~Cargador_datos() 
{

}

/*Pre condiciones: la sala está vacía de actores. Los vectores de salidas están 
vacios.
Post condiciones: la sala queda complementamente cargada.
*/

void Cargador_datos::interpretar_linea_como_propiedades_sala(const std::string& linea, Sala& sala)
{
	const char separador='\t';
	std::vector<unsigned int> datos=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);

	if(datos.size()!=6)
	{
		LOG<<"ERROR: ["<<numero_linea<<"] interpretar_linea_como_propiedades_sala debe tener 6 parametros"<<std::endl;
	}
	else
	{
		sala.establecer_propiedades(datos[0], datos[1], datos[2], datos[3], datos[4], datos[5]);
		sala.iniciar_bloques();
	}
}

/*
Un asterisco significa que es para todos los bloques en la dimensión. 
Una barra entre dos números significa "desde este índice hasta este otro", ambos incluidos.
x	y	z	comp	rep	id	pla
0	0	0	1	1	0	1
*	*	0	1	1	0	1
0|3	1	0|5	1	1	0	1
*/

void Cargador_datos::interpretar_linea_como_bloques(const std::string& linea, Sala& sala, const std::string& nombre_archivo)
{
	const char separador='\t';
	const char todos='*';
	const char rango='|';
	const char ignorar_propiedad='-';
	std::vector<std::string> datos=DLibH::Herramientas::explotar(linea, separador);

	if(datos.size()!=7)
	{
		LOG<<"ERROR: ["<<numero_linea<<"] interpretar_linea_como_bloques debe tener 7 parametros, se encuentran "<<datos.size()<<" en "<<nombre_archivo<<" -> "<<linea<<std::endl;
	}
	else
	{
		//Descomponemos cada uno de ellos... Comportamientos, representación,
		//id y plano pueden ser un guión, que es "ignora esto"...

		unsigned int comportamientos=0;
		unsigned int representacion=0;
		unsigned int id=0;
		unsigned int plano=0;

		bool ignorar_comportamiento=datos[3].find(ignorar_propiedad)!=std::string::npos ? true : false;
		bool ignorar_representacion=datos[4].find(ignorar_propiedad)!=std::string::npos ? true : false;
		bool ignorar_id=datos[5].find(ignorar_propiedad)!=std::string::npos ? true : false;
		bool ignorar_plano=datos[6].find(ignorar_propiedad)!=std::string::npos ? true : false;

		comportamientos=DLibH::Herramientas::cadena_a_entero(datos[3]);
		representacion=DLibH::Herramientas::cadena_a_entero(datos[4]);
		id=DLibH::Herramientas::cadena_a_entero(datos[5]);
		plano=DLibH::Herramientas::cadena_a_entero(datos[6]);

		unsigned int ini_rango_x=0, fin_rango_x=0,
			ini_rango_y=0, fin_rango_y=0,
			ini_rango_z=0, fin_rango_z=0;

		if(datos[0].find(todos)!=std::string::npos) fin_rango_x=sala.acc_w();
		else if(datos[0].find(rango)!=std::string::npos) interpretar_info_bloques_rangos(datos[0], ini_rango_x, fin_rango_x);
		else interpretar_info_bloques_simple(datos[0], ini_rango_x, fin_rango_x);

		if(datos[1].find(todos)!=std::string::npos) fin_rango_y=sala.acc_d();
		else if(datos[1].find(rango)!=std::string::npos) interpretar_info_bloques_rangos(datos[1], ini_rango_y, fin_rango_y);
		else interpretar_info_bloques_simple(datos[1], ini_rango_y, fin_rango_y);

		if(datos[2].find(todos)!=std::string::npos) fin_rango_z=sala.acc_h();
		else if(datos[2].find(rango)!=std::string::npos) interpretar_info_bloques_rangos(datos[2], ini_rango_z, fin_rango_z);
		else interpretar_info_bloques_simple(datos[2], ini_rango_z, fin_rango_z);

		if(fin_rango_x >= sala.acc_w()) fin_rango_x=sala.acc_w();
		if(fin_rango_y >= sala.acc_d()) fin_rango_y=sala.acc_d();
		if(fin_rango_z >= sala.acc_h()) fin_rango_z=sala.acc_h();

		for(unsigned int z=ini_rango_z; z < fin_rango_z; ++z)
			for(unsigned int y=ini_rango_y; y < fin_rango_y; ++y)
				for(unsigned int x=ini_rango_x; x < fin_rango_x; ++x)
				{
					Bloque& b=sala.obtener_bloque(x, y, z);
					if(b.es_bloque_error())
					{
						LOG<<"ERROR: ["<<numero_linea<<"] el bloque "<<x<<","<<y<<","<<z<<" es error en carga de archivo"<<std::endl;
					}
					else
					{
						if(!ignorar_comportamiento) b.establecer_tipo_comportamiento(comportamientos);
						if(!ignorar_representacion) b.establecer_tipo_representacion(representacion);
						if(!ignorar_plano) b.establecer_plano_representacion(plano);
						if(!ignorar_id) b.establecer_id(id);
					}
				}
	}
}

void Cargador_datos::interpretar_info_bloques_rangos(const std::string& cadena, unsigned int& ini, unsigned int& fin)
{
	const char rango='|';
	std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(cadena, rango);
	ini=valores[0];
	fin=valores[1]+1;
}

void Cargador_datos::interpretar_info_bloques_simple(const std::string& cadena, unsigned int& ini, unsigned int& fin)
{
	ini=DLibH::Herramientas::cadena_a_entero(cadena);
	fin=ini+1;
}

void Cargador_datos::interpretar_linea_como_actores(const std::string& linea, std::vector<Actor *>& v)//Sala& sala)
{
	const char separador='\t';
	std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);

	if(valores.size() < 4)
	{
		LOG<<"ERROR: ["<<numero_linea<<"] Al generar actor hay menos de 4 parametros en linea "<<linea<<std::endl;
	}
	else
	{
		unsigned int tipo=valores[0];
		unsigned int x=valores[1];
		unsigned int y=valores[2];
		unsigned int z=valores[3];

		Factoria_actores::Parametros_factoria params(x, y, z);
		std::vector<unsigned int>::iterator 	ini=valores.begin()+4,
							fin=valores.end();

		while(ini < fin)
		{
			params.insertar_param(*ini);
			++ini;
		}

		Actor * a=Factoria_actores::fabricar(tipo, params);

		if(!a)
		{
			LOG<<"ERROR: ["<<numero_linea<<"] Tipo de actor incorrecto : "<<tipo<<" en "<<linea<<std::endl;
		}
		else
		{
			v.push_back(a);
		}
	}	
}

bool Cargador_datos::inicializar()
{
	std::ifstream archivo(FICHERO_DATOS.c_str(), std::ios::in);
	LOG<<"Inicializando cargados datos con "<<FICHERO_DATOS.c_str()<<std::endl;

	if(!archivo)
	{
		return false;
	}
	else
	{
		unsigned int indice=0;
		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				

			if(linea.size() && linea[0]!='#' )
			{
				switch(indice)
				{
					case 0: nombre_fichero_datos_persistentes=linea; break;
					case 1: nombre_fichero_cadenas_acciones=linea; break;
					case 2: nombre_fichero_datos_temporizadores=linea; break;
					case 3: nombre_fichero_datos_bloques=linea; break;
					case 4: nombre_fichero_datos_actores=linea; break;
					case 5: nombre_fichero_salas=linea; break;
					case 6: nombre_fichero_patrones_disparos=linea; break;
					case 7: nombre_fichero_datos_proyectiles=linea; break;
				}

				++indice;
			}
		}	

		archivo.close();

		return true;
	}
}

Controlador_patrones_disparo Cargador_datos::generar_datos_patrones_disparo()
{
	Controlador_patrones_disparo resultado;
	std::ifstream archivo(nombre_fichero_patrones_disparos.c_str(), std::ios::in);

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de datos de patrones de disparo"<<std::endl;
	}
	else
	{
		numero_linea=0;
		const char separador='\t';

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				
			++numero_linea;

			if(linea.size())
			{
				std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);
				if(valores.size()!=3) LOG<<"ERROR: ["<<numero_linea<<"] No hay 3 parametros para datos patrones disparo, en su lugar "<<valores.size()<<std::endl;
				else 
				{
					unsigned int id=valores[0];
					unsigned int estado=valores[1];
					unsigned int pausa=valores[2];

					if(!resultado.existe_entrada_por_id(id)) resultado.insertar(id, Datos_patron_disparo(id));
					resultado.insertar_paso(id, Info_patron_disparo(estado, pausa));
				}
			}
		}	

		archivo.close();
	}

	LOG<<"Cargados "<<resultado.size()<<" patrones de disparo."<<std::endl;

	return resultado;
}

Controlador_datos_temporizador Cargador_datos::generar_datos_temporizador()
{
	Controlador_datos_temporizador resultado;
	std::ifstream archivo(nombre_fichero_datos_temporizadores.c_str(), std::ios::in);

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de datos temporizadores"<<std::endl;
	}
	else
	{
		numero_linea=0;
		const char separador='\t';

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				
			++numero_linea;

			if(linea.size())
			{
				std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);
				if(valores.size()!=4) LOG<<"ERROR: ["<<numero_linea<<"] No hay 4 parametros para datos temporizador, en su lugar "<<valores.size()<<std::endl;
				else resultado.insertar(valores[0], Datos_temporizador(valores[1], valores[2], valores[3]));
			}
		}	

		archivo.close();
	}

	LOG<<"Cargados "<<resultado.size()<<" temporizadores."<<std::endl;

	return resultado;
}

Controlador_datos_persistentes Cargador_datos::generar_datos_persistentes()
{
	Controlador_datos_persistentes resultado;
	std::ifstream archivo(nombre_fichero_datos_persistentes.c_str(), std::ios::in);

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de datos persistentes"<<std::endl;
	}
	else
	{
		const char separador='\t';
		numero_linea=0;

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				
			++numero_linea;

			if(linea.size())
			{
				std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);
				if(valores.size()!=5) LOG<<"ERROR: ["<<numero_linea<<"] No hay 5 parametros para datos persistentes, en su lugar "<<valores.size()<<" en "<<linea<<std::endl;
				else 
				{
					if(resultado.existe_entrada_por_id(valores[0])) LOG<<"ERROR: ["<<numero_linea<<"] Para datos persistentes, se repite la entrada "<<valores[0]<<std::endl;
					else resultado.insertar(valores[0], Dato_persistente(valores[0], valores[1], valores[2], valores[3], valores[4]));
				}
			}
		}	

		archivo.close();
	}

	LOG<<"Cargados "<<resultado.size()<<" datos persistentes."<<std::endl;

	return resultado;
}

Controlador_cadenas_acciones Cargador_datos::generar_cadenas_acciones()
{
	Controlador_cadenas_acciones resultado;
	std::ifstream archivo(nombre_fichero_cadenas_acciones.c_str(), std::ios::in);

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de cadenas de acciones"<<std::endl;
	}
	else
	{
		const char separador='\t';
		numero_linea=0;

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				
			++numero_linea;

			if(linea.size())
			{
				std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);
				if(valores.size()!=4) LOG<<"ERROR: ["<<numero_linea<<"] No hay 4 parametros para cadena de acciones, en su lugar "<<valores.size()<<" en linea "<<linea<<std::endl;
				else
				{
					if(resultado.existe_entrada_por_id(valores[0])) LOG<<"ERROR: ["<<numero_linea<<"] Para cadenas acciones, se repite la entrada "<<valores[0]<<std::endl;
					else resultado.insertar(valores[0], Cadena_acciones(valores[1], valores[2], valores[3]));
				}
			}
		}	

		archivo.close();
	}

	LOG<<"Cargadas "<<resultado.size()<<" cadenas de acciones."<<std::endl;

	return resultado;
}

Controlador_datos_representacion_bloque Cargador_datos::generar_datos_representacion_bloques()
{
	Controlador_datos_representacion_bloque resultado;
	std::ifstream archivo(nombre_fichero_datos_bloques.c_str(), std::ios::in);
	const char separador='\t';
	const std::string delimitador_linea="[F]";

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de representacion de bloques"<<std::endl;
	}
	else
	{

		bool linea_iniciada=false;
		unsigned int id_tipo=0;
		Datos_representacion_animada temp;
		numero_linea=0;

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				
			++numero_linea;

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
					if(valores.size()!=4) LOG<<"ERROR: ["<<numero_linea<<"] No hay 4 parametros para cadena de representacion de bloques, en su lugar "<<valores.size()<<" en linea "<<linea<<std::endl;
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

Controlador_datos_actores_genericos Cargador_datos::generar_datos_actores_genericos()
{
	Controlador_datos_actores_genericos resultado;
	std::ifstream archivo(nombre_fichero_datos_actores.c_str(), std::ios::in);
	const char separador='\t';
	const std::string delimitador_linea="[F]";

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de datos de actores genéricos"<<std::endl;
	}
	else
	{
		numero_linea=0;
		bool linea_iniciada=false;
		unsigned int id_tipo=0;
		Datos_actor_generico temp;

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				
			++numero_linea;

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
					if(valores.size()!=9) LOG<<"ERROR: ["<<numero_linea<<"] No hay 9 parametros para cadena de actores genéricos, en su lugar "<<valores.size()<<" en linea "<<linea<<std::endl;
					else 
					{
						id_tipo=valores[0];
						temp.asignar_indice_recurso(valores[1]);
						temp.asignar_total_frames(valores[2]);
						temp.asignar_duracion(valores[3]);
						temp.asignar_w(valores[4]);
						temp.asignar_h(valores[5]);
						temp.asignar_d(valores[6]);
						temp.asignar_flags(valores[7]);
						temp.asignar_efecto_colision(valores[8]);

						linea_iniciada=true;
					}
				}
				//Si la línea es iniciada y no es fin, sabemos que es un frame.
				else
				{
					temp.insertar_frame(obtener_frame_de_linea(linea));
				}
			}
		}

		archivo.close();
	}

	LOG<<"Cargados "<<resultado.size()<<" actores genéricos."<<std::endl;

	return resultado;
}

Controlador_datos_proyectiles_genericos Cargador_datos::generar_datos_proyectiles_genericos()
{
	Controlador_datos_proyectiles_genericos resultado;
	std::ifstream archivo(nombre_fichero_datos_proyectiles.c_str(), std::ios::in);
	const char separador='\t';
	const std::string delimitador_linea="[F]";

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de datos de proyectiles genéricos"<<std::endl;
	}
	else
	{

		bool linea_iniciada=false;
		unsigned int id_tipo=0;
		const unsigned int c_params=9;
		Datos_proyectil_generico temp;
		numero_linea=0;

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				
			++numero_linea;

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
					if(valores.size()!=c_params) LOG<<"ERROR: ["<<numero_linea<<"] No hay "<<c_params<<" parametros para cadena de proyectiles genéricos, en su lugar "<<valores.size()<<" en linea "<<linea<<std::endl;
					else 
					{
						id_tipo=valores[0];
						temp.asignar_w(valores[1]);
						temp.asignar_h(valores[2]);
						temp.asignar_d(valores[3]);
						temp.asignar_desaparece_colision(valores[4]);
						temp.asignar_tipo_letal(valores[5]);
						temp.asignar_indice_recurso(valores[6]);
						temp.asignar_total_frames(valores[7]);
						temp.asignar_duracion(valores[8]);

						linea_iniciada=true;
					}
				}
				//Si la línea es iniciada y no es fin, sabemos que es un frame.
				else
				{
					temp.insertar_frame(obtener_frame_de_linea(linea));
				}
			}
		}

		archivo.close();
	}

	LOG<<"Cargados "<<resultado.size()<<" proyectiles genéricos."<<std::endl;

	return resultado;
}


Frame_representacion Cargador_datos::obtener_frame_de_linea(std::string linea)
{
	const char separador='\t';
	std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);
	if(valores.size()!=7) 
	{
		LOG<<"ERROR: ["<<numero_linea<<"] No hay 7 parametros para cadena de datos de frame, en su lugar "<<valores.size()<<" en linea "<<linea<<std::endl;
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

/*Carga de sala completa para servirla al controlador de juego.*/

bool Cargador_datos::cargar_sala_desde_id(Sala& sala, unsigned int id)
{
	if(!ids_archivos.count(id))
	{
		return false;
	}
	else
	{
		cargar_sala_desde_archivo(sala, ids_archivos.at(id), E_PROPIEDADES | E_BLOQUES | E_ACTORES);
		return true;
	}	
}

/*Abre el archivo de salas y hace corresponder cada id con el nombre de archivo
de turno. Aunque no sea necesario comprobar que existe, si que la cargamos
para sacar el id, que está dentro del archivo.*/

void Cargador_datos::localizar_archivos_salas()
{
	ids_archivos.clear();
	std::ifstream archivo(nombre_fichero_salas.c_str(), std::ios::in);
	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de salas"<<std::endl;
	}
	else
	{
		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				
			if(linea.size())
			{
				Sala * temp=new Sala(0,0,0,0,false);
				cargar_sala_desde_archivo(*temp, linea, E_PROPIEDADES);
				const unsigned int id_sala=temp->acc_id();
				if(id_sala) 
				{
					if(ids_archivos.count(id_sala))
					{
						LOG<<"ERROR: El id de sala "<<id_sala<<" se repite"<<std::endl;
					}
					else
					{
						ids_archivos.insert(std::pair<unsigned int, std::string>(id_sala, linea));
					}
				}
				delete temp;
			}
		}	

		archivo.close();
	}

	LOG<<"Cargado indice de "<<ids_archivos.size()<<" salas."<<std::endl;
}

/*Lee todas las salas que ya tenemos y guarda la información sobre cada una de
ellas, si es secreta o no y cosas por el estilo.*/

void Cargador_datos::cargar_control_salas(Control_salas& cs)
{
	for(std::pair<unsigned int, std::string> par : ids_archivos)
	{
		Sala * temp=new Sala(0,0,0,0,false);
		cargar_sala_desde_archivo(*temp, par.second, E_PROPIEDADES);
		const unsigned int tp=!temp->es_secreta() ? Control_salas::TP_NORMAL : Control_salas::TP_SECRETO;
		const unsigned int id_sala=par.first;

		cs.insertar_info_sala(id_sala, tp);
		delete temp;
	}
}

/*Lee todas las salas que hay en el diccionario y carga sólo los actores
persistentes... La carga se hace mediante el método de cargar sala desde archivo
pasando el id sala, que ya lo conocemos.*/

void Cargador_datos::precargar_actores_persistentes(Control_actores_persistentes& cap)
{
	for(std::pair<unsigned int, std::string> par : ids_archivos)
	{
		Sala * temp=new Sala(0,0,0,0,false);
		extraer_actores_persistentes_de_sala(cap, *temp, par.second, par.first);
		delete temp;
	}
}

void Cargador_datos::extraer_actores_persistentes_de_sala(
	Control_actores_persistentes& cap,
	Sala& sala, 
	const std::string& nombre_archivo, unsigned int id_sala)
{
	sala.establecer_nombre_archivo(nombre_archivo);
	std::ifstream archivo(nombre_archivo.c_str(), std::ios::in);
	const std::string delimitador_estado="[F]";
	unsigned int estado=E_PROPIEDADES;		

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo "<<nombre_archivo<<std::endl;
	}
	else
	{
		std::vector<Actor *> persistentes;

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
			if(linea.find(delimitador_estado)!=std::string::npos)
			{
				estado*=2; //Los valores están como flags.
			}
			else 
			{
				if(linea.size() && estado==E_PERSISTENTES) interpretar_linea_como_actores(linea, persistentes);
			}
		}
	
		cap.insertar_actores(id_sala, persistentes);
		archivo.close();
	}
}

void Cargador_datos::cargar_sala_desde_archivo(Sala& sala, const std::string& nombre_archivo, unsigned int flags)
{
	sala.establecer_nombre_archivo(nombre_archivo);
	std::ifstream archivo(nombre_archivo.c_str(), std::ios::in);
	const std::string delimitador_estado="[F]";
	unsigned int estado=E_PROPIEDADES;

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo "<<nombre_archivo<<std::endl;
	}
	else
	{
		std::vector<Actor *> actores;

		while(!archivo.eof())
		{
			numero_linea=0;

			//Leer línea en cadena.
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
			++numero_linea;

			if(linea.find(delimitador_estado)!=std::string::npos)
			{
				estado*=2; //Los valores están como flags.
			}
			else if(linea.size())
			{
				if(flags & estado) switch(estado)
				{
					case E_PROPIEDADES: interpretar_linea_como_propiedades_sala(linea, sala); break;
					case E_BLOQUES: interpretar_linea_como_bloques(linea, sala, nombre_archivo); break;
					case E_ACTORES: interpretar_linea_como_actores(linea, actores); break;
				}
			}
		}

		//Se transfiere aquí la propiedad de los actores a la sala.
		if(flags & E_ACTORES) for(Actor * a :actores) sala.insertar_actor(a);

		archivo.close();
	}
}

void Cargador_datos::ejecutar_comprobacion_sintaxis_salas()
{
	ids_archivos.clear();
	std::ifstream archivo(nombre_fichero_salas.c_str(), std::ios::in);
	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de salas"<<std::endl;
	}
	else
	{
		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				
			if(linea.size())
			{
				Sala * temp=new Sala(0,0,0,0,false);
				comprobacion_sintaxis_sala(*temp, linea);
				delete temp;
			}
		}	

		archivo.close();
	}

	LOG<<"Finalizada comprobacion sintaxis."<<std::endl;
}

void Cargador_datos::comprobacion_sintaxis_sala(Sala& sala, const std::string& nombre_archivo)
{
	sala.establecer_nombre_archivo(nombre_archivo);
	std::ifstream archivo(nombre_archivo.c_str(), std::ios::in);
	const std::string delimitador_estado="[F]";
	unsigned int estado=E_PROPIEDADES;		

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo "<<nombre_archivo<<std::endl;
	}
	else
	{
		LOG<<"Iniciar comprobacion sintaxis de archivo de sala "<<nombre_archivo<<std::endl;

		std::vector<Actor *> actores;

		while(!archivo.eof())
		{
			//Leer línea en cadena.
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
			if(linea.find(delimitador_estado)!=std::string::npos) 
			{
				estado*=2;
			}
			else if(linea.size())
			{
				switch(estado)
				{
					case E_PROPIEDADES: interpretar_linea_como_propiedades_sala(linea, sala); break;
					case E_BLOQUES: interpretar_linea_como_bloques(linea, sala, nombre_archivo); break;
					case E_ACTORES:
					case E_PERSISTENTES:
						interpretar_linea_como_actores(linea, actores); 
					break;
				}
			}
		}

		for(Actor * a :actores) delete a;
		actores.clear();
		archivo.close();
	}
}
