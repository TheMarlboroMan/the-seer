#include "cargador_recursos.h"

const std::string Cargador_recursos::FICHERO_GRAFICOS="data/info/recursos_graficos.txt";
const std::string Cargador_recursos::FICHERO_AUDIO="data/info/recursos_audio.txt";

Cargador_recursos::Cargador_recursos()
{

}

Cargador_recursos::~Cargador_recursos()
{

}

void Cargador_recursos::generar_recursos_graficos()
{
	std::ifstream archivo(FICHERO_GRAFICOS.c_str(), std::ios::in);

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de recursos_graficos"<<std::endl;
	}
	else
	{
		//Esto supone que no debemos almacenar referencias a los recursos,
		//sino obtnerlos siempre del controlador.
		DLibV::Gestor_recursos_graficos::vaciar();

		const char separador='\t';

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.

			if(linea.size())
			{

				std::vector<std::string> valores=DLibH::Herramientas::explotar(linea, separador);
				if(valores.size()!=6) LOG<<"ERROR: No hay 6 parametros para recursos graficos, en su lugar "<<valores.size()<<std::endl;
				else
				{
					unsigned int indice=DLibH::Herramientas::cadena_a_entero(valores[0]);
					std::string ruta=valores[1];
					unsigned int transparencia=DLibH::Herramientas::cadena_a_entero(valores[2]);

					DLibV::Imagen * img=NULL;
					img=new DLibV::Imagen(ruta.c_str());

					if(DLibV::Gestor_recursos_graficos::insertar(indice, img)==-1)
					{
						LOG<<"ERROR: No se ha podido insertar recurso grafico "<<indice<<" en "<<ruta<<std::endl;
					}
					else
					{
						if(transparencia)
						{
							unsigned int r=DLibH::Herramientas::cadena_a_entero(valores[3]);
							unsigned int g=DLibH::Herramientas::cadena_a_entero(valores[4]);
							unsigned int b=DLibH::Herramientas::cadena_a_entero(valores[5]);

							DLibV::Gestor_recursos_graficos::obtener(indice)->establecer_transparencia(r, g, b);
						}
					}
				}
			}
		}

		archivo.close();
	}
}

void Cargador_recursos::generar_recursos_audio()
{
	std::ifstream archivo(FICHERO_AUDIO.c_str(), std::ios::in);

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de recursos_audio"<<std::endl;
	}
	else
	{
		//Esto supone que no debemos almacenar referencias a los recursos,
		//sino obtnerlos siempre del controlador.

		//OJO: Vacia sonido y música.

		DLibA::Gestor_recursos_audio::liberar();

		const char separador='\t';

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.

			if(linea.size())
			{
				std::vector<std::string> valores=DLibH::Herramientas::explotar(linea, separador);
				if(valores.size()!=2) LOG<<"ERROR: No hay 6 parametros para recursos audio, en su lugar "<<valores.size()<<std::endl;
				else
				{
					unsigned int indice=DLibH::Herramientas::cadena_a_entero(valores[0]);
					std::string ruta=valores[1];

					if(DLibA::Gestor_recursos_audio::insertar_sonido(indice, ruta.c_str())==-1)
					{
						LOG<<"ERROR: No se ha podido insertar recurso audio "<<indice<<" en "<<ruta<<std::endl;
					}
				}
			}
		}

		archivo.close();
	}
}
