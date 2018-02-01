#ifndef DATOS_COLISION_H
#define DATOS_COLISION_H

#include "geometria.h"

/*Representa una colisión de un actor con alguna otra cosa. Guarda información
sobre el espacio de la colisión, como eje, cantidad, posición o el cubo con el
que se ha colisionado.

La información que se guarda se asume en un principio que viene de una colisión 
legal, es decir: dos objetos estaban separados y ahora están colisionando. Esto
es especialmente cierto para la "posicion" de la colisión.
*/

struct Datos_colision
{
	public:

	//Tipos especiales de colisión, cuando los tocas hacen algo.
	enum tipos_colision {
		TC_NADA=0,
		TC_SALIDA_SALA=1,
		TC_LETAL=2,
		TC_TANQUE_ENERGIA=3,
		TC_BONUS=4,
		TC_LLAVE=5,
		TC_AMULETO=6,
		TC_BONUS_TIEMPO=7,
		TC_HABILIDAD_LAVA=100,
		TC_HABILIDAD_AGUA=101,
		TC_HABILIDAD_TELETRANSPORTE=102,
		TC_HABILIDAD_IMPULSO=103,
		TC_HABILIDAD_PARAR_TIEMPO=104,
		TC_HABILIDAD_INSPECTOR_SECRETOS=105,
		TC_HABILIDAD_RESISTENCIA_DARDOS=106,
		TC_HABILIDAD_SUPER_SALTO=107
};

	unsigned int tipo_colision;	//Según lo que está arriba.
	unsigned int eje;		//Como en "Definiciones::ejes".
	bool con_solido;		//Indica si la metermos a ordenarse con otras que sean sólidas, para corregir el movimiento.
	float cantidad;			//Cantidad que se entra... Sería equivalente a la corrección a aplicar para dejar los dos cubos rozándose por el borde.
	float vector;			//Vector que de movimiento en el eje.
	unsigned int posicion; 		//Como en Definiciones::posicion. NO SE CONTEMPLAN "dentro" o "error".
	float posicion_corregida; 	//Como en "ponme en este valor en el eje al finalizar".
	int correccion_z;
//	bool habilita_snap;

	Cubo cubo_a_original;		//Cubo de actor en la posición justo anterior (el jugador antes de moverse...).
	Cubo cubo_a;			//Cubo de actor original (como el jugador).
	Cubo cubo_b;			//Cubo de actor comparado (como el bloque con el que chocó el jugador).

	Datos_colision(unsigned int pe=0, bool p_con_solido=false);
	Datos_colision(const Datos_colision& dc);
	Datos_colision& operator=(const Datos_colision& dc);
	float obtener_cantidad_con_signo() const;
	void obtener_posicion_por_vector();

	bool es_con_correccion_z() const {return correccion_z!=-1;}
	bool tiene_efecto() const {return tipo_colision!=TC_NADA;}
	bool es_considerable() const {return (cantidad && con_solido) || es_con_correccion_z();}
	bool operator<(const Datos_colision& otro) const 
	{
		if(con_solido < otro.con_solido) return false;
		else return cantidad > otro.cantidad;
	}
};

#endif
