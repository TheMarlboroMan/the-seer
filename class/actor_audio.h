#ifndef ACTOR_AUDIO_H
#define ACTOR_AUDIO_H

/*Un reproductor de audio.*/

#include "audio.h"
#include "actor.h"
#include "activable.h"
#include "visitante.h"
//#include "actor_generico.h"
//#include "datos_legibles.h"

class Actor_audio:public Actor, public Activable
{
	/////////////////////
	// Propio.

	private:

	enum tipos {T_UNICO=1, T_REPETIR, T_REPETIR_ALEATORIO, T_REPETIR_INTERVALO};

	DLibA::Canal_audio canal;
	unsigned int tipo_reproduccion;
	unsigned int id_sonido;
	unsigned int volumen;
	unsigned int pan;
	unsigned int param1;
	unsigned int param2;

	float tiempo_actual;
	float tiempo_objetivo;
	bool iniciada_reproduccion;

	void liberar_canal();
	void iniciar_reproduccion();
	void calcular_tiempos();		

	public:

	Actor_audio(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int pt, unsigned int pson, unsigned int pvol, 
		unsigned int ppan, unsigned int pp1, unsigned int pp2, bool pa,
		unsigned int pid);
	~Actor_audio();

	void turno(float p_delta);
	unsigned int acc_tipo_reproduccion() const {return tipo_reproduccion;}
	unsigned int acc_id_sonido() const {return id_sonido;}
	unsigned int acc_volumen() const {return volumen;}
	unsigned int acc_pan() const {return pan;}
	unsigned int acc_param1() const {return param1;}
	unsigned int acc_param2() const {return param2;}


	////////////////////
	// Representable

	public:

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "actor_audio";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br) {}

	///////////////////
	// Espaciable

	public:

	void actualizar_volumen(){establecer_volumen(1, 1, 1);}
	virtual bool es_colisionable() const {return false;}
	virtual bool es_solido() const {return false;}
	virtual bool es_recibe_sombra() const {return false;}

	///////////////////
	// Actor.

	public:

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}

	///////////////////
	//Activable...

	/*void activar() 
	{
		Activable::activar();
		actualizar_estado_actor();
	}*/

	void detener_sonido() 
	{
		liberar_canal();
	}

	void desactivar() 
	{
		Activable::desactivar();
		liberar_canal();
	}	
};

#endif
