#ifndef ACTOR_MOVIL_H
#define ACTOR_MOVIL_H

#include "actor.h"
#include "definiciones.h"

class Actor_movil:public Actor
{
	//////////////////////
	// Propio...

	private:

	Vector_3d vector;

	protected:

	//Define la limitación de "no puedes moverte más rápido de esto en un 
	//sólo paso". No debería tener relevancia para la lógica del juego.

	//Ya no tiene sentido: "activar_movimiento" controla esto.

	//virtual float vector_limite(unsigned int flags)=0;


	public:

	Actor_movil(int px, int py, int pz);
	virtual ~Actor_movil();
	virtual void activar_movimiento(float p_delta, unsigned int flags=Definiciones::X | Definiciones::Y | Definiciones::Z);
	void establecer_vector(float vx, float vy, float vz);
	void establecer_eje_vector(float v, unsigned int eje);
	void invertir_vector(unsigned int pflags=Definiciones::X | Definiciones::Y | Definiciones::Z);
	void sumar_vector(float val, unsigned int flags=0);
	void cancelar_vector(unsigned int pflags);
	virtual void decelerar_vector(float valor, unsigned int flags=0);

	const Vector_3d& acc_vector() const {return vector;} 
	void establecer_vector(Vector_3d v) {vector=v;}

	float vector_para_eje(unsigned int p_eje) const;
	virtual bool recibir_datos_colision(const Datos_colision& datos_colision);
};

#endif
