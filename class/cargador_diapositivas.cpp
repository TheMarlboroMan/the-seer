#include "cargador_diapositivas.h"

const std::string Cargador_diapositivas::FICHERO_DATOS="data/info/datos_diapositivas.txt";
const std::string Cargador_diapositivas::DELIMITADOR="[F]";

Cargador_diapositivas::Cargador_diapositivas(Localizador& loc):
	loc_textos(loc)
{

}

void Cargador_diapositivas::inicializar()
{
	std::ifstream archivo(FICHERO_DATOS);

	if(archivo)
	{
		marco_lectura(archivo, &Cargador_diapositivas::inicializar_textos);
		marco_lectura(archivo, &Cargador_diapositivas::inicializar_imagenes);
		marco_lectura(archivo, &Cargador_diapositivas::inicializar_diapositivas);
		marco_lectura(archivo, &Cargador_diapositivas::inicializar_pases);
		archivo.close();
	}		
}

void Cargador_diapositivas::marco_lectura(std::ifstream& archivo, void (Cargador_diapositivas::*metodo)(std::vector<unsigned int>&))
{
	while(!archivo.eof())
	{
		std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);
		if(linea.find(DELIMITADOR)!=std::string::npos) return;
		else if(linea.size())
		{
			std::vector<unsigned int> datos=Utilidades_cargadores::explotar_linea_a_enteros(linea, SEPARADOR);
			(this->*metodo)(datos);
		}
	}
}

void Cargador_diapositivas::inicializar_textos(std::vector<unsigned int>& datos)
{
	if(datos.size()!=2)
	{
		LOG<<"ERROR: diapositivas_inicializar_textos debe tener 2 parametros"<<std::endl;
	}
	else
	{
		unsigned int id=datos[0];
		unsigned int idtxt=datos[1];

		if(textos.existe_entrada_por_id(id)) LOG<<"ERROR: diapositivas_inicializar_textos repetida "<<id<<std::endl;

		Diapositiva_texto texto(id, loc_textos.obtener(idtxt));
		textos.insertar(id, texto);
	}
}

void Cargador_diapositivas::inicializar_imagenes(std::vector<unsigned int>& datos)
{
	if(datos.size()!=3)
	{
		LOG<<"ERROR: diapositivas_inicializar_imagenes debe tener 3 parametros"<<std::endl;
	}
	else
	{
		unsigned int id=datos[0];
		unsigned int x=datos[1];
		unsigned int y=datos[2];

		if(imagenes.existe_entrada_por_id(id)) LOG<<"ERROR: diapositivas_inicializar_imagenes repetida "<<id<<std::endl;

		Diapositiva_imagen img(id, x, y);
		imagenes.insertar(id, img);
	}
}

void Cargador_diapositivas::inicializar_diapositivas(std::vector<unsigned int>& datos)
{
	if(datos.size() < 3)
	{
		LOG<<"ERROR: diapositivas_inicializar_diapositicas debe tener al menos 3 parametros"<<std::endl;
	}
	else
	{
		unsigned int id=datos[0];

		if(diapositivas.existe_entrada_por_id(id)) LOG<<"ERROR: diapositivas_inicializar_diapositivas repetida "<<id<<std::endl;

		Diapositiva dp(id);

		unsigned int id_imagen=datos[1];
		if(!imagenes.existe_entrada_por_id(id_imagen))
		{
			LOG<<"ERROR: diapositivas_inicializar_diapositivas imagen no disponible "<<id<<" "<<id_imagen<<std::endl;
		}
		else
		{
			Diapositiva_imagen img=imagenes.obtener_por_id(id_imagen);
			dp.asignar_imagen(img);
		}

		unsigned int act=2;
		unsigned int max=datos.size();

		while(act < max)
		{
			unsigned int id_txt=datos[act];

			if(id_txt)
			{
				if(!textos.existe_entrada_por_id(id_txt))
				{
					LOG<<"ERROR: diapositivas_inicializar_diapositivas texto no disponible "<<id<<" "<<id_txt<<std::endl;
				}
				else
				{
					Diapositiva_texto txt=textos.obtener_por_id(id_txt);
					dp.insertar_texto(txt);
				}
			}

			++act;
		}

		diapositivas.insertar(id, dp);
	}
}

void Cargador_diapositivas::inicializar_pases(std::vector<unsigned int>& datos)
{
	if(datos.size() < 2)
	{
		LOG<<"ERROR: diapositivas_inicializar_pases debe tener al menos 2 parametros"<<std::endl;
	}
	else
	{
		unsigned int id=datos[0];

		if(pases.existe_entrada_por_id(id)) LOG<<"ERROR: diapositivas_inicializar_pases repetida "<<id<<std::endl;

		Pase_diapositivas p(id);

		unsigned int act=1;
		unsigned int max=datos.size();

		while(act < max)
		{
			unsigned int id_diap=datos[act];
			if(!diapositivas.existe_entrada_por_id(id_diap))
			{
				LOG<<"ERROR: diapositivas_inicializar_pases diapositiva no disponible "<<id<<" "<<id_diap<<std::endl;
			}
			else
			{
				Diapositiva dp=diapositivas.obtener_por_id(id_diap);
				p.insertar_diapositiva(dp);
			}

			++act;
		}

		pases.insertar(id, p);
	}
}

Pase_diapositivas Cargador_diapositivas::obtener_pase_por_id(unsigned int pid) const
{
	if(!pases.existe_entrada_por_id(pid))
	{
		throw Excepcion_diapositivas();
	}
	
	Pase_diapositivas resultado=pases.obtener_por_id(pid);
	return resultado;
}
