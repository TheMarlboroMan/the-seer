#ifndef PROYECTIL_OBSTACULO_H
#define PROYECTIL_OBSTACULO_H

/*El proyectil que se mueve en una sóla dirección hasta que choca
con cualquier bloque de la sala. No se vería afectado por los actores.

La representación cambiará en función de la dirección Norte-Sur, Este-Oeste,
con cuatro casos posibles. El resto se descartan porque no deberían usarse 
así.

Para construirlo sólo pedimos dos cosas: la velocidad y la dirección según 
la clase definiciones. A partir de eso ya generaremos el vector de turno.
..*/

#include "actor_movil.h"
#include "visitante.h"
#include "datos_legibles.h"

class Proyectil_obstaculo:public Actor_movil
{
	///////////////////////////////////////
	// Definiciones y enumeraciones

	public:

	//Define los tipos de daño que hacen los proyectiles.
	enum tipos_letal
	{
		T_NADA=0,	//Hmmm... No se usa.
		T_SIEMPRE=1,	//Siempre te liquida.
		T_DARDO=2,	//Es un dardo, el amuleto de protección contra dardos ayuda.,
		T_CORTANTE=3	//No creo que lo use...
	};

	///////////////////////////////////////
	// Propio.

	private:

	static Controlador_datos_proyectiles_genericos controlador_datos;

	unsigned int tipo;
	unsigned int direccion;
	unsigned int velocidad;
	float tiempo;
	bool desaparece_en_colision;
	unsigned int tipo_letal;

	void configurar_propiedades();

	public:

	Proyectil_obstaculo(int px, int py, int pz, unsigned int pt, unsigned int pdir, unsigned int pvel);
	virtual ~Proyectil_obstaculo();

	void static asignar_datos_proyectiles(Controlador_datos_proyectiles_genericos v) {controlador_datos=v;}
	void turno(float p_delta) {tiempo+=p_delta;}
	bool es_desaparece_en_colision() const {return desaparece_en_colision;}
	unsigned int acc_tipo_letal() const {return tipo_letal;}

	///////////////////////////////////////
	//Actor movil

	protected:

	//virtual float vector_limite(unsigned int flags) {return 64.0;}

	//////////////////////////////////////
	// Espaciable

	public:

	virtual void actualizar_volumen();
	virtual bool es_recibe_sombra() const {return false;}

	//////////////////////////////////////
	// Representable.

	public:

	virtual std::string string_me_about_yourself() const {return "Proyectil obstaculo";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br);

	//////////////////////////////////////
	// Actor.
	
	public:

	virtual bool es_solido() const {return false;}
	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
