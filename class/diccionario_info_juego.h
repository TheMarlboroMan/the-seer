#ifndef DICCIONARIO_INFO_JUEGO_H
#define DICCIONARIO_INFO_JUEGO_H

/*Esta clase la comparten el controlador de juego y el controlador
de info juego (el que muestra en pantalla las cosas que has
recogido. Sirve para convertir "Has cogido esto" a "Vas a mostrar
estos mensajes" entre un controlador y otro.

A pesar del nombre no tiene nada que ver con "info_juego".
*/

class Diccionario_info_juego
{
	private:

	static const int OFFSET_TEXTO=300;

	public:

	enum elementos{
//Info sobre cosas que se recogen.
D_TANQUE_ENERGIA=0,
D_LLAVE=1,
D_COFRE=2,
D_TESORO=3,
D_HABILIDAD_LAVA=4,
D_HABILIDAD_AGUA=5,
D_HABILIDAD_TELETRANSPORTE=6,
D_HABILIDAD_IMPULSO=7,
D_HABILIDAD_TIEMPO=8,
D_HABILIDAD_SECRETOS=9,
D_AMULETO=10,
D_HABILIDAD_DARDO=11,
D_BONUS_TIEMPO=12,
D_HABILIDAD_SUPER_SALTO=13,
//Mensajes durante el juego...
D_NECESARIO_AMULETO=50,
D_NECESARIA_PIEZA=51,
D_CONFIRMACION_ABANDONAR_PARTIDA=52,
D_GUARDADO_SESION_OK=53,
D_GUARDADO_SESION_ERROR=54,
D_NECESARIA_LLAVE=55,
D_BONUS_TIEMPO_USADO=56,
D_SIN_BONUS_TIEMPO=57
	};

	static unsigned int obtener_indice_texto(unsigned int i) 
	{
		return i+OFFSET_TEXTO;
	}
};

#endif
