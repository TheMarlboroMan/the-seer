#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <fstream>
#include "utilidades_cargadores.h"
#include "../libdan_headers/herramientas/herramientas/herramientas.h"

/*
Esta clase lee y escribe en un archivo los datos de configuración de la 
aplicación. Luego se los sirve al controlador, que los copia en sus propiedades.
*/

class Configuracion
{
	private:

	unsigned short int pantalla_completa;
	unsigned short int modo_hardware;
	unsigned short int pantalla_doble_buffer;
	unsigned short int pantalla_anyformat;
	unsigned short int idioma;
	int volumen_audio;
	int volumen_musica;
	int audio_ratio;
	int audio_salidas;
	int audio_buffers;
	int audio_canales;

	static const std::string CLAVE_VERSION_ARCHIVO;
	static const std::string VERSION_ARCHIVO;
	static const std::string RUTA_ARCHIVO;
	static const std::string SEPARADOR_ARCHIVO;

	static const std::string CLAVE_PANTALLA_COMPLETA;	//0 -> Falso / 1 -> Verdadero
	static const unsigned short int PANTALLA_COMPLETA_DEFECTO=0;

	static const std::string CLAVE_MODO_HARDWARE;
	static const unsigned short int MODO_HARDWARE_DEFECTO=1;	//0 -> No / 1 -> Si

	static const std::string CLAVE_PANTALLA_DOBLE_BUFFER;
	static const unsigned short int PANTALLA_DOBLE_BUFFER_DEFECTO=1;	//0 -> No / 1 -> Si

	static const std::string CLAVE_PANTALLA_ANYFORMAT;
	static const unsigned short int PANTALLA_ANYFORMAT_DEFECTO=1;	//0 -> No / 1 -> Si

	static const std::string CLAVE_VOLUMEN_AUDIO;
	static const unsigned short int VOLUMEN_AUDIO_DEFECTO=64;	//0-128...

	static const std::string CLAVE_VOLUMEN_MUSICA;
	static const unsigned short int VOLUMEN_MUSICA_DEFECTO=64;	//0-128...

	static const std::string CLAVE_IDIOMA;
	static const unsigned short int IDIOMA_DEFECTO=0;	//0-Español, 1-Inglés...

	static const std::string CLAVE_AUDIO_RATIO;
	static const int AUDIO_RATIO_DEFECTO=44100;

	static const std::string CLAVE_AUDIO_SALIDAS;
	static const int AUDIO_SALIDAS_DEFECTO=2;	

	static const std::string CLAVE_AUDIO_BUFFERS;
	static const int AUDIO_BUFFERS_DEFECTO=1024;

	static const std::string CLAVE_AUDIO_CANALES;
	static const int AUDIO_CANALES_DEFECTO=8;

	public:

	unsigned short int acc_pantalla_completa() const {return this->pantalla_completa;}
	unsigned short int acc_modo_hardware() const {return this->modo_hardware;}
	unsigned short int acc_pantalla_doble_buffer() const {return this->pantalla_doble_buffer;}
	unsigned short int acc_pantalla_anyformat() const {return this->pantalla_anyformat;}
	unsigned short int acc_idioma() const {return this->idioma;}
	int acc_volumen_audio() const {return this->volumen_audio;}
	int acc_volumen_musica() const {return this->volumen_musica;}
	int acc_audio_ratio() const {return this->audio_ratio;}
	int acc_audio_salidas() const {return this->audio_salidas;}
	int acc_audio_buffers() const {return this->audio_buffers;}
	int acc_audio_canales() const {return this->audio_canales;}

	void mut_modo_hardware(unsigned short int p_valor) {this->modo_hardware=p_valor;}
	void mut_pantalla_completa(unsigned short int p_valor) {this->pantalla_completa=p_valor;}
	void mut_pantalla_doble_buffer(unsigned short int p_valor) {this->pantalla_doble_buffer=p_valor;}
	void mut_pantalla_anyformat(unsigned short int p_valor) {this->pantalla_anyformat=p_valor;}
	void mut_idioma(unsigned short int p_valor) {this->idioma=p_valor;}
	void mut_volumen_audio(int p_valor) {this->volumen_audio=p_valor;}
	void mut_volumen_musica(int p_valor) {this->volumen_musica=p_valor;}
	void cargar();
	void grabar();
	
	Configuracion();
	~Configuracion();
};

#endif
