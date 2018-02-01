#ifndef TIEMPO_JUEGO_H
#define TIEMPO_JUEGO_H

#include <string>

class Tiempo_juego
{
	private:

	float segundos_restantes;	//Tiempo de juego restante (a nivel de juego, el tiempo antes de game over).
	float multiplicador_tiempo;	//Valor por el que se multiplica el tiempo.

	const static float VAL_MULTIPLICADOR_TIEMPO_NORMAL;
	const static float VAL_MULTIPLICADOR_TIEMPO_PARADO;

	public:

	Tiempo_juego();

	//Utilidades y métodos a nivel de juego.
	void turno(float);
	unsigned int acc_segundos_restantes() const {return segundos_restantes;}
	void sumar_segundos(int);
	void establecer_segundos_restantes(unsigned int);
	bool es_finalizado() const {return segundos_restantes <= 0.0;}
//	void reiniciar();
	void detener() {multiplicador_tiempo=VAL_MULTIPLICADOR_TIEMPO_PARADO;}
	void arrancar() {multiplicador_tiempo=VAL_MULTIPLICADOR_TIEMPO_NORMAL;}

	static std::string como_cadena(unsigned int);
};

#endif
