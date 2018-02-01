#include "visitante_serializacion.h"

void Visitante_serializacion::visitar(Actor_audio& a)
{
/*4 - Reproductor_audio, vol 0-128 pan 0-255
#tipo	x	y	z	t.repr.	id_snd	volum	pan	param1	param2	activo	id
4	0	0	0	1	1	127	127	0	0	1	1
*/
	unsigned int activo=a.es_activo();
	
	archivo<<
		Factoria_actores::TF_ACTOR_AUDIO<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_tipo_reproduccion()<<separador<<
		a.acc_id_sonido()<<separador<<
		a.acc_volumen()<<separador<<
		a.acc_pan()<<separador<<
		a.acc_param1()<<separador<<
		a.acc_param2()<<separador<<
		activo<<separador<<
		a.acc_id_localizador()<<std::endl;
}

void Visitante_serializacion::visitar(Disparador_proyectiles& a)
{
/*13 - Disparador
#tipo	x	y	z	t.proy.	patron	direc.	veloc.	paso.	activo	id	id_enlac.
13	120	32	32	1	1	2	64	0	1	0	0
*/
	unsigned int activo=a.es_activo();
	
	archivo<<
		Factoria_actores::TF_DISPARADOR_PROYECTILES<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_tipo_proyectil()<<separador<<
		a.acc_patron()<<separador<<
		a.acc_direccion()<<separador<<
		a.acc_velocidad()<<separador<<
		a.acc_paso_actual()<<separador<<
		activo<<separador<<
		a.acc_id_localizador()<<separador<<
		a.acc_id_enlace_activacion()<<std::endl;		
}

void Visitante_serializacion::visitar(Cofre& a)
{
/*6 - Cofre
#tipo	x	y	z	id_pz.	abierto
6	120	32	32	0	[0]
*/

	unsigned int abierto=a.es_abierto();
	
	archivo<<
		Factoria_actores::TF_COFRE<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_id_pieza()<<separador<<
		abierto<<std::endl;		
}

void Visitante_serializacion::visitar(Actor_generico& a)
{
/*14 - Actor genérico...
#tipo	x	y	z	t.actor
14	0	0	0	1
*/

	archivo<<
		Factoria_actores::TF_ACTOR_GENERICO<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_tipo_actor_generico()<<std::endl;		
}

void Visitante_serializacion::visitar(Alternador_actor_generico& a)
{
/*5 - Alternador actor genérico
#tipo	x	y	z	id	t.actor	patron	paso	activo	id_on	id_off
5	0	0	0	1	1	0	0	1	0	0
*/
	unsigned int activo=a.es_activo();

	archivo<<
		Factoria_actores::TF_ALTERNADOR_ACTOR_GENERICO<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_id_localizador()<<separador<<
		a.acc_tipo_actor()<<separador<<
		a.acc_patron()<<separador<<
		a.acc_paso_actual()<<separador<<
		activo<<separador<<
		a.acc_id_enlace_activacion()<<separador<<
		a.acc_id_enlace_desactivacion()<<std::endl;
}

void Visitante_serializacion::visitar(Escalera& a)
{
/*10 - Escalera (flags: 1 tope escalable. 2 base invertir).
#tipo	x	y	z	h	orient	flags	activa	id
10	32	32	64	40	2	3	1	0
*/

	unsigned int activo=a.es_activo();

	archivo<<
		Factoria_actores::TF_ESCALERA<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_h()<<separador<<
		a.acc_orientacion()<<separador<<
		a.acc_flags()<<separador<<
		activo<<separador<<
		a.acc_id_localizador()<<std::endl;
}

void Visitante_serializacion::visitar(Actor_diapositivas& a)
{
/*15 - Actor diapositiva...
#tipo	x	y	z	id_diap	activo	id
15	0	0	0	1	0	1
*/

	unsigned int activo=a.es_activo();
	unsigned int reproducido=a.es_reproducido();

	archivo<<
		Factoria_actores::TF_ACTOR_DIAPOSITIVA<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_id_diapositiva()<<separador<<
		activo<<separador<<
		a.acc_id_localizador()<<separador<<
		reproducido<<std::endl;
}

void Visitante_serializacion::visitar(Interruptor_contacto& a)
{
/*16 - Interruptor_contacto
#tipo	x	y	z	w	h	d	id_in	id_out	repet.	activo	id	c.cin	c.out
16	16	16	16	16	1	16	1	2	0	1	0	[0]	[0]
*/

	unsigned int activo=a.es_activo();
	unsigned int repetible=a.es_repetible();

	archivo<<
		Factoria_actores::TF_INTERRUPTOR_CONTACTO<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_w()<<separador<<
		a.acc_h()<<separador<<
		a.acc_d()<<separador<<
		a.acc_id_cadena_in()<<separador<<
		a.acc_id_cadena_out()<<separador<<
		repetible<<separador<<
		activo<<separador<<
		a.acc_id_localizador()<<separador<<
		a.acc_cuenta_in()<<separador<<
		a.acc_cuenta_out()<<std::endl;
}

void Visitante_serializacion::visitar(Actor_info_juego& a)
{
/*18 - Actor info juego (para pop ups de información. Id_info_activo según diccionario_info_juego.
#tipo	x	y	z	id_info	activo	id
18	0	0	0	1	0	1
*/
	unsigned int activo=a.es_activo();

	archivo<<
		Factoria_actores::TF_ACTOR_INFO_JUEGO<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_id_info_juego()<<separador<<
		activo<<separador<<
		a.acc_id_localizador()<<std::endl;
}

void Visitante_serializacion::visitar(Actor_puzzle& a)
{
/*20 - Actor puzzle (debe tener un id).
#tipo	x	y	z	id_puz	id_cad	id	resuelto
20	0	0	0	1	100	1	[0]
*/

	unsigned int resuelto=a.es_resuelto();

	archivo<<
		Factoria_actores::TF_ACTOR_PUZZLE<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_id_puzzle()<<separador<<
		a.acc_id_cadena()<<separador<<
		a.acc_id_localizador()<<separador<<
		resuelto<<std::endl;
}

void Visitante_serializacion::visitar(Interruptor_repetible& a)
{
/*3 - Interruptor repetible (dos cadenas, según se encienda o apague).
#tipo	x	y	z	cadena	c.off	t.cond.	c.cond	[estado]
3	120	32	32	1	2	0	0	[0]
*/
	unsigned int encendido=a.es_encendido();

	archivo<<
		Factoria_actores::TF_INTERRUPTOR_REPETIBLE<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
	 	a.acc_id_eslabon_cadena()<<separador<<
		a.acc_id_eslabon_cadena_off()<<separador<<
		a.acc_tipo_condicion()<<separador<<
		a.acc_id_eslabon_cadena_no_condicion()<<separador<<
		encendido<<std::endl;
}

void Visitante_serializacion::visitar(Interruptor_invisible_repetible& a)
{
/*19 - Interruptor invisible repetible
#tipo	x	y	z	w	h	d	id_on	id_off	t.cond	c.cond	estado
19	16	16	32	16	16	1	1	2	0	0	[0]
*/
	unsigned int encendido=a.es_encendido();

	archivo<<
		Factoria_actores::TF_INTERRUPTOR_INVISIBLE_REPETIBLE<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_w()<<separador<<
		a.acc_h()<<separador<<
		a.acc_d()<<separador<<
	 	a.acc_id_eslabon_cadena()<<separador<<
		a.acc_id_eslabon_cadena_off()<<separador<<
		a.acc_tipo_condicion()<<separador<<
		a.acc_id_eslabon_cadena_no_condicion()<<separador<<
		encendido<<std::endl;
}

void Visitante_serializacion::visitar(Interruptor_invisible_no_repetible& a)
{
/*8 - Interruptor invisible no repetible
#tipo	x	y	z	w	h	d	cadena	secreto t.cond	c.cond	usado
8	120	32	32	32	32	1	1	0	0	0	[0]
*/
	unsigned int secreto=a.es_secreto();
	unsigned int usado=a.es_usado();

	archivo<<
		Factoria_actores::TF_INTERRUPTOR_INVISIBLE_NO_REPETIBLE<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
		a.acc_w()<<separador<<
		a.acc_h()<<separador<<
		a.acc_d()<<separador<<
	 	a.acc_id_eslabon_cadena()<<separador<<
		secreto<<separador<<
		a.acc_tipo_condicion()<<separador<<
		a.acc_id_eslabon_cadena_no_condicion()<<separador<<
		usado<<std::endl;
}

void Visitante_serializacion::visitar(Interruptor_no_repetible& a)
{
/*7 - Interruptor no repetible
#tipo	x	y	z	cadena	id	t.cond	c.cond	usado
7	120	32	32	1	1	0	0	[0]
*/
	unsigned int usado=a.es_usado();

	archivo<<
		Factoria_actores::TF_INTERRUPTOR_NO_REPETIBLE<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
	 	a.acc_id_eslabon_cadena()<<separador<<
	 	a.acc_id_localizador()<<separador<<
		a.acc_tipo_condicion()<<separador<<
		a.acc_id_eslabon_cadena_no_condicion()<<separador<<
		usado<<std::endl;
}

void Visitante_serializacion::visitar(Interruptor_auto& a)
{
/*23 - Interruptor auto
#tipo	x	y	z	id_cad	id	activo	usado
23	0	0	0	1	2	1	[0]
*/

	unsigned int usado=a.es_usado();
	unsigned int activo=a.es_activo();

	archivo<<
		Factoria_actores::TF_INTERRUPTOR_AUTO<<separador<<
		a.acc_x()<<separador<<
		a.acc_y()<<separador<<
		a.acc_z()<<separador<<
	 	a.acc_id_cadena()<<separador<<
	 	a.acc_id_localizador()<<separador<<
	 	activo<<separador<<
		usado<<std::endl;
}
