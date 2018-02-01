#ifndef ACTIVABLE_H
#define ACTIVABLE_H

/*La interfaz que define a algo del mapa que puede ser encendido o
apagado. Lo implementarían cosas como obstáculos letales y cosas
así.

La activación se llevaría a cabo por medio de un visitante específico
que cambiaría el estado.*/

class Activable
{
	private:

	bool activable_activo;

	public:

	Activable(bool a): activable_activo(a){}
	~Activable(){}

	virtual void activar() {activable_activo=true;}
	virtual void desactivar() {activable_activo=false;}
	bool es_activo() const {return activable_activo;}
};

#endif
