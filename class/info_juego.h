#ifndef INFO_JUEGO_H
#define INFO_JUEGO_H

/*Guarda información muy concreta sobre el desarrollo del juego. Por
alguna razón arcana las llaves están guardadas aquí: esto antes llevaba el 
control de si podías ver los secretos, las vidas que te quedaban y cosas así.
Como hemos puesto aquí las llaves, pondremos también otros consumibles.
*/

class Info_juego
{
	public:

	static const int LLAVES_INICIO=0;
	static const int SEGUNDOS_INICIO=5400;
	static const int SEGUNDOS_POR_TESORO=5;
	static const int SEGUNDOS_POR_TIEMPO=60;

	static const int SEGUNDOS_POR_COFRE=0;
	static const int SEGUNDOS_POR_VIDA=-120;

	private:

	unsigned int vidas_perdidas;
	unsigned int llaves;
	unsigned int bonus_tiempo;	//Cantidad de bonus tiempo que tienes.

	public:

	Info_juego():vidas_perdidas(0), llaves(LLAVES_INICIO), bonus_tiempo(0) {}

	unsigned int acc_vidas_perdidas() const {return vidas_perdidas;}
	unsigned int acc_llaves() const {return llaves;}
	unsigned int acc_bonus_tiempo() const {return bonus_tiempo;}

	void perder_vida() {++vidas_perdidas;}

	void actualizar_vidas_perdidas(unsigned int v) {vidas_perdidas=v;}
	void actualizar_llaves(unsigned int v) {llaves=v;}
	void restar_llave() {--llaves;}
	void vaciar_bonus_tiempo() {bonus_tiempo=0;}
	void actualizar_bonus_tiempo(unsigned int v) {bonus_tiempo=v;}
	void conceder_bonus_tiempo(unsigned int v) {bonus_tiempo+=v;}
	void conceder_llaves(unsigned int v) {llaves+=v;}
};

#endif
