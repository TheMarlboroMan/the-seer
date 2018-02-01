#ifndef CONTROLADOR_AUDIO_H
#define CONTROLADOR_AUDIO_H

/*Clase proxy con el controlador de audio. Añadimos alguna utilidad para el 
tema de pausar y despausar sonidos.*/

#include "../libdan_headers/libDanSDL.h"

struct Excepcion_audio
{
	std::string mensaje;

	Excepcion_audio(std::string p_m)
		:mensaje(p_m) {}

	Excepcion_audio(const Excepcion_audio& e)
		:mensaje(e.mensaje) {}

	Excepcion_audio& operator=(const Excepcion_audio& e)
	{
		mensaje=e.mensaje;
		return *this;
	}
};

class Audio
{
	public:
	
	//Sonidos que son usados por el código en si, en contraste con los
	//sonidos que se usan en los niveles.
	enum sonidos {
			S_MIN=0,
		S_CAIDA=1, 
		S_ITEM=2, 
		S_COFRE=3,
		S_SALTO=4,
		S_CAIDA_LARGA=5,
		S_PERDER_VIDA=6,
		S_DETENER_TIEMPO=7,
		S_REANUDAR_TIEMPO=8,
		S_IMPULSO=9,
		S_PROTECCION_LAVA_AGUA=10,
			S_MAX};

	private:

	Audio();

	static DLibA::Controlador_audio_SDL * controlador_audio;
	static DLibA::Cola_sonido cola_sonido;
	static std::vector<unsigned int> canales_detenidos;

	public:

	static void pausar();
	static void pausar_canales_activos();

	static void despausar();
	static void despausar_excepto_activos();
	static void despausar_canales_activos();

	static void debug_estado_canales() {if(controlador_audio) controlador_audio->debug_estado_canales();}
	static void insertar_sonido(DLibA::Estructura_sonido es) {cola_sonido.insertar(es);}
	static void procesar_cola_sonido() {cola_sonido.procesar_cola();}
	static DLibA::Canal_audio obtener_canal_libre();
	static bool inicializar_entorno_audio(int pratio, int psalidas, int pbuffers, int pcanales);
	static void establecer_volumen(int v) {if(controlador_audio) controlador_audio->establecer_volumen(v);}
	static void establecer_volumen_musica(int v) {if(controlador_audio) controlador_audio->establecer_volumen_musica(v);}
	static int acc_volumen_general_sonido()
	{
		if(controlador_audio) return controlador_audio->acc_volumen_general_sonido();
		else return -1;
	}
};

#endif
