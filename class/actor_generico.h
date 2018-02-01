#ifndef ACTOR_GENERICO_H
#define ACTOR_GENERICO_H

/*Un obstáculo cuya posición, dimensiones, atributos y representación son 
libres y dependen de valores establecidos en un fichero de configuración. De 
este modo no necesitamos recompilar cada vez que queramos crear un tipo 
de actor nuevo.

El sistema de representación sería similar al de los bloques, por no decir
calcado, aunque en el futuro podemos añadir, por ejemplo, la "orientación".

La información más básica se guarda mediante un sistema de flags en 8 bits como 
sigue:

1 -> Representable
2 -> Colisionable
4 -> Sólido
8 -> Recibe sombra
16 -> Recogible
32 -> Sin uso
64 -> Sin uso
128 -> Sin uso.

La información sobre el efecto que tiene al colisionar con ellos se guarda en 
"efecto_colision", que se puede usar para hacer trastos letales, bonus o 
simplemente obstáculos

Para definir cada tipo en su archivo... 

#tipo	frames	dur_s	w	h	d	flags	efect.
#1	3	300	32	32	32	7	0

#Definición de frames...
#dur	x	y	w	h	cx	cy
0	0	0	0	0	0	0
100	0	0	0	0	0	0
200	0	0	0	0	0	0

#Separador de nuevo tipo.
[F]
*/

#include "actor.h"
#include "visitante.h"
#include "temporizador_animaciones.h"
#include "datos_legibles.h"

class Actor_generico:public Actor
{
	private:

	enum FLAGS{
		F_REPRESENTABLE=1,
		F_COLISIONABLE=2,
		F_SOLIDO=4,
		F_RECIBE_SOMBRA=8,
		F_RECOGIBLE=16};


	static Controlador_datos_actores_genericos controlador_datos;

	unsigned int tipo_actor_generico;
	unsigned int flags;
	unsigned int efecto_colision;

	////////////////////////////////////
	//Métodos propios...
	
	private:

	void actualizar_volumen();
	void actualizar_propiedades();

	public:

	Actor_generico(int px, int py, int pz, int pt);
	~Actor_generico();
	void static asignar_datos_representacion(Controlador_datos_actores_genericos v) {controlador_datos=v;}
	bool es_recogible() const {return flags & F_RECOGIBLE;}
	bool es_llave() const {return efecto_colision==Datos_colision::TC_LLAVE;}
	bool es_bonus() const {return efecto_colision==Datos_colision::TC_BONUS;}

	unsigned int acc_efecto_colision() const {return efecto_colision;}
	unsigned int acc_tipo_actor_generico() const {return tipo_actor_generico;}
	
	//Esta parte de la interfaz pública es, en particular, para que los
	//obstaculos puedan estar presentes o no presentes.

	void hacer_presente()
	{
		if(!es_representable()) flags=flags | F_REPRESENTABLE | F_COLISIONABLE;
	}

	void hacer_ausente()
	{
		if(es_representable())
		{
			flags=flags ^ F_REPRESENTABLE;
			flags=flags ^ F_COLISIONABLE;
		}
	}

	////////////////////////////////////
	//Implementación de representable...
	
	public:

	virtual bool es_representable() const {return flags & F_REPRESENTABLE;}
	virtual void transformar_representacion(Bloque_transformacion_representable& br);

	////////////////////////////////////
	//Implementación de espaciable.

	virtual bool es_colisionable() const {return flags & F_COLISIONABLE;}
	virtual bool es_solido() const {return flags & F_SOLIDO;}
	virtual bool es_recibe_sombra() const {return flags & F_RECIBE_SOMBRA;}

	virtual std::string string_me_about_yourself() const {return "obstaculo generico tipo:"+
		DLibH::Herramientas::entero_a_cadena(tipo_actor_generico)+
		" flags:"+DLibH::Herramientas::entero_a_cadena(flags);}

	////////////////////////////////////	
	//Implementación de actor.

	virtual void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
