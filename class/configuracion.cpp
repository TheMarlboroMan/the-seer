#include "configuracion.h"

const std::string Configuracion::CLAVE_VERSION_ARCHIVO="v";
const std::string Configuracion::VERSION_ARCHIVO="1";
const std::string Configuracion::RUTA_ARCHIVO="data/config/configuracion.dat";
const std::string Configuracion::SEPARADOR_ARCHIVO=":";
const std::string Configuracion::CLAVE_PANTALLA_COMPLETA="pantalla_completa";
const std::string Configuracion::CLAVE_MODO_HARDWARE="modo_hardware";
const std::string Configuracion::CLAVE_PANTALLA_DOBLE_BUFFER="pantalla_doble_buffer";
const std::string Configuracion::CLAVE_PANTALLA_ANYFORMAT="pantalla_anyformat";
const std::string Configuracion::CLAVE_VOLUMEN_AUDIO="volumen_audio";
const std::string Configuracion::CLAVE_VOLUMEN_MUSICA="volumen_musica";
const std::string Configuracion::CLAVE_IDIOMA="idioma";
const std::string Configuracion::CLAVE_AUDIO_RATIO="audio_ratio";
const std::string Configuracion::CLAVE_AUDIO_SALIDAS="audio_salidas";
const std::string Configuracion::CLAVE_AUDIO_BUFFERS="audio_buffers";
const std::string Configuracion::CLAVE_AUDIO_CANALES="audio_canales";


Configuracion::Configuracion()
{
	this->pantalla_completa=PANTALLA_COMPLETA_DEFECTO;
	this->modo_hardware=MODO_HARDWARE_DEFECTO;
	this->pantalla_doble_buffer=PANTALLA_DOBLE_BUFFER_DEFECTO;
	this->pantalla_anyformat=PANTALLA_ANYFORMAT_DEFECTO;
	this->volumen_audio=VOLUMEN_AUDIO_DEFECTO;
	this->volumen_musica=VOLUMEN_MUSICA_DEFECTO;
	this->idioma=IDIOMA_DEFECTO;
	this->audio_ratio=AUDIO_RATIO_DEFECTO;
	this->audio_salidas=AUDIO_SALIDAS_DEFECTO;
	this->audio_buffers=AUDIO_BUFFERS_DEFECTO;
	this->audio_canales=AUDIO_CANALES_DEFECTO;

	this->cargar();
}

Configuracion::~Configuracion()
{

}

void Configuracion::cargar()
{
	std::ifstream archivo(RUTA_ARCHIVO.c_str(), std::ios::in);

	if(!archivo.is_open())
	{
		this->grabar();
		this->cargar();
		return;
	}

	while(!archivo.eof())
	{
		std::string cadena=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo);

		if(cadena.size())
		{
			int pos=cadena.find(SEPARADOR_ARCHIVO);
	
			if(pos!=-1)
			{
				const std::string clave=cadena.substr(0, pos);
				const std::string valor=cadena.substr(pos+1);

				if(clave.compare(CLAVE_PANTALLA_COMPLETA)==0)
				{
					this->pantalla_completa=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
				else if(clave.compare(CLAVE_MODO_HARDWARE)==0)
				{
					this->modo_hardware=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
				if(clave.compare(CLAVE_PANTALLA_DOBLE_BUFFER)==0)
				{
					this->pantalla_doble_buffer=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
				if(clave.compare(CLAVE_PANTALLA_ANYFORMAT)==0)
				{
					this->pantalla_anyformat=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
				else if(clave.compare(CLAVE_VOLUMEN_AUDIO)==0)
				{
					this->volumen_audio=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
				else if(clave.compare(CLAVE_VOLUMEN_MUSICA)==0)
				{
					this->volumen_musica=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
				else if(clave.compare(CLAVE_IDIOMA)==0)
				{
					this->idioma=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
				else if(clave.compare(CLAVE_AUDIO_RATIO)==0)
				{
					this->audio_ratio=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
				else if(clave.compare(CLAVE_AUDIO_SALIDAS)==0)
				{
					this->audio_salidas=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
				else if(clave.compare(CLAVE_AUDIO_BUFFERS)==0)
				{
					this->audio_buffers=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
				else if(clave.compare(CLAVE_AUDIO_CANALES)==0)
				{
					this->audio_canales=DLibH::Herramientas::cadena_a_entero(valor.c_str());
				}
			}						
		}
	}
}

void Configuracion::grabar()
{
	std::ofstream archivo(RUTA_ARCHIVO.c_str(), std::ios::out);

	if(!archivo.is_open()) return;	//Fallo catastrófico...

	archivo<<CLAVE_VERSION_ARCHIVO<<SEPARADOR_ARCHIVO<<VERSION_ARCHIVO<<std::endl;
	archivo<<CLAVE_PANTALLA_COMPLETA<<SEPARADOR_ARCHIVO<<this->pantalla_completa<<std::endl;
	archivo<<CLAVE_MODO_HARDWARE<<SEPARADOR_ARCHIVO<<this->modo_hardware<<std::endl;
	archivo<<CLAVE_PANTALLA_DOBLE_BUFFER<<SEPARADOR_ARCHIVO<<this->pantalla_doble_buffer<<std::endl;
	archivo<<CLAVE_PANTALLA_ANYFORMAT<<SEPARADOR_ARCHIVO<<this->pantalla_anyformat<<std::endl;
	archivo<<CLAVE_VOLUMEN_AUDIO<<SEPARADOR_ARCHIVO<<this->volumen_audio<<std::endl;
	archivo<<CLAVE_VOLUMEN_MUSICA<<SEPARADOR_ARCHIVO<<this->volumen_musica<<std::endl;
	archivo<<CLAVE_IDIOMA<<SEPARADOR_ARCHIVO<<this->idioma<<std::endl;
	archivo<<CLAVE_AUDIO_RATIO<<SEPARADOR_ARCHIVO<<this->audio_ratio<<std::endl;
	archivo<<CLAVE_AUDIO_SALIDAS<<SEPARADOR_ARCHIVO<<this->audio_salidas<<std::endl;
	archivo<<CLAVE_AUDIO_BUFFERS<<SEPARADOR_ARCHIVO<<this->audio_buffers<<std::endl;
	archivo<<CLAVE_AUDIO_CANALES<<SEPARADOR_ARCHIVO<<this->audio_canales<<std::endl;

	archivo.close();	
}
