#ifndef DISPARADOR_PROYECTILES_H
#define DISPARADOR_PROYECTILES_H

/*El disparador de proyectiles es un actor no colisionable que indica la 
información de los proyectiles que disparará. El método para disparar el 
proyectil lo vamos a poner realmente en esta clase, para no dejarlo todo en 
manos de los visitantes.*/

#include "actor.h"
#include "visitante.h"
#include "activable.h"
#include "proyectil_obstaculo.h"
#include "datos_legibles.h"

class Disparador_proyectiles:public Actor, public Activable
{
	/////////////////////
	// Propio.

	private:

	enum estados {E_PAUSA=0, E_DISPARO=1, E_ESPERA=100};

	unsigned int tipo_proyectil;
	unsigned int patron;
	unsigned int direccion;
	unsigned int velocidad;
	unsigned int id_enlace_activacion; //Un activable con este id recibirá la señal de activar a la vez.
	
	unsigned int paso_actual;
	unsigned int total_pasos;
	int estado_paso_actual; //Tiene signo. Se inicia como 100, que sería no hacer nada.
	float tiempo_paso_actual;
	float tiempo_max_paso;
	
	void cargar_patron_pasos();
	void cargar_info_paso();

	public:

	Disparador_proyectiles(unsigned int px, unsigned int py, unsigned int pz, 
		unsigned int pt, unsigned int pp, unsigned int pd, 
		unsigned int pvel, unsigned int pid_enl, unsigned int pact, bool pactivo,
		unsigned int pid);
	~Disparador_proyectiles();

	unsigned int acc_tipo_proyectil() const {return tipo_proyectil;}
	unsigned int acc_patron() const {return patron;}
	unsigned int acc_direccion() const {return direccion;}
	unsigned int acc_velocidad() const {return velocidad;}
	unsigned int acc_paso_actual() const {return paso_actual;}
	unsigned int acc_id_enlace_activacion() const {return id_enlace_activacion;}

	void turno(float p_delta);

	bool debe_disparar() const {return es_activo() && estado_paso_actual==E_DISPARO;}

	Actor * generar_proyectil();

	////////////////////
	// Representable

	public:

	virtual bool es_representable() const {return false;}
	virtual std::string string_me_about_yourself() const {return "disparador proyectiles";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br) {}

	///////////////////
	// Espaciable

	public:

	void actualizar_volumen(){establecer_volumen(1, 1, 16);}
	virtual bool es_colisionable() const {return false;}
	virtual bool es_solido() const {return false;}
	virtual bool es_recibe_sombra() const {return false;}

	///////////////////
	// Actor.

	public:

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
