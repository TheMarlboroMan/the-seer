#include "factoria_actores.h"

Factoria_actores::Factoria_actores(){}
Factoria_actores::~Factoria_actores(){}

Actor * Factoria_actores::fabricar_entrada_sala(const Parametros_factoria& p)
{
	unsigned int id_entrada=0;

	try
	{
		if(p.params.size()!=1) {throw std::out_of_range("No params");}
		id_entrada=p.params.at(0);
	}
	catch(const std::out_of_range& e) {error("Entrada sala");}

	Entrada_sala * resultado=new Entrada_sala(p.x, p.y, p.z, id_entrada);
	return resultado;
}

Actor * Factoria_actores::fabricar_salida_sala(const Parametros_factoria& p)
{
	unsigned int w=0;
	unsigned int h=0;
	unsigned int d=0;
	unsigned int id_entrada=0;
	unsigned int id_sala=0;

	try
	{
		if(p.params.size()!=5) throw std::out_of_range("No params");
		w=p.params.at(0);
		h=p.params.at(1);
		d=p.params.at(2);
		id_entrada=p.params.at(3);
		id_sala=p.params.at(4);
	}
	catch(const std::out_of_range& e) {error("Salida sala");}

	Salida_sala * resultado=new Salida_sala(p.x, p.y, p.z, w, h, d, id_entrada, id_sala);
	return resultado;
}

Actor * Factoria_actores::fabricar_interruptor_repetible(const Parametros_factoria& p)
{
	unsigned int id_eslabon=0;
	unsigned int id_eslabon_off=0;
	unsigned int tipo_condicion=0;
	unsigned int id_eslabon_error_condicion=0;
	unsigned int encendido=0;

	try
	{
		switch(p.params.size())
		{
			case 5: 
				encendido=p.params.at(4);
			case 4:
				id_eslabon=p.params.at(0);
				id_eslabon_off=p.params.at(1);
				tipo_condicion=p.params.at(2);
				id_eslabon_error_condicion=p.params.at(3);
			break;

			default: throw std::out_of_range("No params"); break;
		}
	}
	catch(const std::out_of_range& e) {error("Interruptor repetible");}

	Interruptor_repetible * resultado=new Interruptor_repetible(p.x, p.y, p.z, id_eslabon, 
		id_eslabon_off, tipo_condicion, id_eslabon_error_condicion, encendido);

	return resultado;
}

Actor * Factoria_actores::fabricar_interruptor_no_repetible(const Parametros_factoria& p)
{
	unsigned int id_eslabon=0;
	unsigned int id_interruptor=0;
	unsigned int tipo_condicion=0;
	unsigned int id_eslabon_error_condicion=0;
	unsigned int usado=0;

	try
	{
		switch(p.params.size())
		{
			case 5:
				usado=p.params.at(4);
			case 4:
				id_eslabon=p.params.at(0);
				id_interruptor=p.params.at(1);
				tipo_condicion=p.params.at(2);
				id_eslabon_error_condicion=p.params.at(3);
			break;

			default: throw std::out_of_range("No params"); break;
		}
	}
	catch(const std::out_of_range& e) {error("Interruptor no repetible");}

	Interruptor_no_repetible * resultado=new Interruptor_no_repetible(p.x, p.y, p.z, id_eslabon, 
		id_interruptor, tipo_condicion, id_eslabon_error_condicion, usado);

	return resultado;
}

Actor * Factoria_actores::fabricar_interruptor_invisible_repetible(const Parametros_factoria& p)
{
	unsigned int w=0;
	unsigned int d=0;
	unsigned int h=0;
	unsigned int id_eslabon=0;
	unsigned int id_eslabon_off=0;
	unsigned int tipo_condicion=0;
	unsigned int id_eslabon_error_condicion=0;
	unsigned int estado=0;

	try
	{
		switch(p.params.size())
		{
			case 8:
				estado=p.params.at(7);
			case 7:
				w=p.params.at(0);
				h=p.params.at(1);
				d=p.params.at(2);
				id_eslabon=p.params.at(3);
				id_eslabon_off=p.params.at(4);
				tipo_condicion=p.params.at(5);
				id_eslabon_error_condicion=p.params.at(6);
			break;

			default: throw std::out_of_range("No params"); break;
		}
	}
	catch(const std::out_of_range& e) {error("Interruptor invisible repetible");}

	Interruptor_invisible_repetible * resultado=new Interruptor_invisible_repetible(
		p.x, p.y, p.z, 
		w, h, d,
		id_eslabon, id_eslabon_off, 
		tipo_condicion, id_eslabon_error_condicion, estado);
	return resultado;
}

Actor * Factoria_actores::fabricar_interruptor_invisible_no_repetible(const Parametros_factoria& p)
{
	unsigned int id_eslabon=0;
	unsigned int w=0;
	unsigned int h=0;
	unsigned int d=0;
	unsigned int secreto=0;
	unsigned int tipo_condicion=0;
	unsigned int id_eslabon_error_condicion=0;
	unsigned int usado=0;

	try
	{
		switch(p.params.size())
		{
			case 8:
				usado=p.params.at(7);
			case 7:
				w=p.params.at(0);
				h=p.params.at(1);
				d=p.params.at(2);
				id_eslabon=p.params.at(3);
				secreto=p.params.at(4);
				tipo_condicion=p.params.at(5);
				id_eslabon_error_condicion=p.params.at(6);
			break;
			default: throw std::out_of_range("No params"); break;
		}
	}
	catch(const std::out_of_range& e) {error("Interruptor no repetible");}

	Interruptor_invisible_no_repetible * resultado=new Interruptor_invisible_no_repetible(
		p.x, p.y, p.z, w, h, d, 
		id_eslabon, secreto, tipo_condicion, id_eslabon_error_condicion, usado);

	return resultado;
}

Actor * Factoria_actores::fabricar_cofre(const Parametros_factoria& p)
{
	unsigned int id_pieza=0;
	unsigned int abierto=0;

	try
	{
		switch(p.params.size())
		{
			case 2: 
				abierto=p.params.at(1);
			case 1:	
				id_pieza=p.params.at(0);
			break;
			default: throw std::out_of_range("No params"); break;
		}
	}
	catch(const std::out_of_range& e) {error("Cofre");}

	Cofre * resultado=new Cofre(p.x, p.y, p.z, id_pieza, abierto);
	return resultado;
}


Actor * Factoria_actores::fabricar_proyectil_obstaculo(const Parametros_factoria& p)
{
	unsigned int tipo=0;
	unsigned int dir=0;
	unsigned int vel=0;

	try
	{
		if(p.params.size()!=3) throw std::out_of_range("No params");
		tipo=p.params.at(0);
		dir=p.params.at(1);
		vel=p.params.at(2);
	}
	catch(const std::out_of_range& e) {error("Proyectil obstaculo");}

	Proyectil_obstaculo * resultado=new Proyectil_obstaculo(p.x, p.y, p.z, tipo, dir, vel);
	return resultado;
}

Actor * Factoria_actores::fabricar_disparador_proyectiles(const Parametros_factoria& p)
{
	unsigned int tipo_proyectil=0;
	unsigned int patron=1;
	unsigned int direccion=0;
	unsigned int velocidad=0;
	unsigned int paso_actual=0;
	unsigned int activo=0;
	unsigned int id=0;
	unsigned int id_enlace=0;

	try
	{
		if(p.params.size()!=8) throw std::out_of_range("No params");
		tipo_proyectil=p.params.at(0);
		patron=p.params.at(1);
		direccion=p.params.at(2);
		velocidad=p.params.at(3);
		paso_actual=p.params.at(4);
		activo=p.params.at(5);
		id=p.params.at(6);
		id_enlace=p.params.at(7);
	}
	catch(const std::out_of_range& e) {error("Disparador proyectiles");}

	Disparador_proyectiles * resultado=new Disparador_proyectiles(
		p.x, p.y, p.z, 
		tipo_proyectil, patron, direccion, velocidad, 
		id_enlace, paso_actual, activo, id);
	return resultado;
}

Actor * Factoria_actores::fabricar_control_direccion(const Parametros_factoria& p)
{
	unsigned int dir=0;

	try
	{
		if(p.params.size()!=1) throw std::out_of_range("No params");
		dir=p.params.at(0);
	}
	catch(const std::out_of_range& e) {error("Control direccion");}

	Actor_control_direccion * resultado=new Actor_control_direccion(p.x, p.y, p.z, dir);
	return resultado;
}

Actor_generico * Factoria_actores::fabricar_actor_generico(const Parametros_factoria& p)
{
	unsigned int subtipo=0;

	try
	{
		if(p.params.size()!=1) throw std::out_of_range("No params");
		subtipo=p.params.at(0);
	}
	catch(const std::out_of_range& e) {error("Actor generico");}
	Actor_generico * resultado=new Actor_generico(p.x, p.y, p.z, subtipo);
	return resultado;
}	

Actor * Factoria_actores::fabricar_alternador_actor_generico(const Parametros_factoria& p)
{
	unsigned int id=0;
	unsigned int tipo_actor=0;
	unsigned int patron=0;
	unsigned int id_enlace_on=0;
	unsigned int id_enlace_off=0;
	unsigned int paso=0;
	unsigned int activo=0;

	try
	{
		if(p.params.size()!=7) throw std::out_of_range("No params");

		id=p.params.at(0);
		tipo_actor=p.params.at(1);
		patron=p.params.at(2);
		paso=p.params.at(3);
	 	activo=p.params.at(4);
		id_enlace_on=p.params.at(5);
		id_enlace_off=p.params.at(6);
	}
	catch(const std::out_of_range& e) {error("Alternador_actor_generico");}
	Alternador_actor_generico * resultado=new Alternador_actor_generico(
		p.x, p.y, p.z, 
		tipo_actor, patron, id_enlace_on, id_enlace_off, 
		activo, paso, id);
	return resultado;
}

Actor * Factoria_actores::fabricar_actor_audio(const Parametros_factoria& p)
{
	unsigned int tipo_reproduccion=0;
	unsigned int id_sonido=0;
	unsigned int vol=0;
	unsigned int pan=0;
	unsigned int param1=0;
	unsigned int param2=0;
	unsigned int activo=0;
	unsigned int id=0;

	try
	{
		if(p.params.size()!=8) throw std::out_of_range("No params");

		tipo_reproduccion=p.params.at(0);
		id_sonido=p.params.at(1);
		vol=p.params.at(2);
		pan=p.params.at(3);
	 	param1=p.params.at(4);
		param2=p.params.at(5);
		activo=p.params.at(6);
		id=p.params.at(7);
	}
	catch(const std::out_of_range& e) {error("Actor_audio");}
	Actor_audio * resultado=new Actor_audio(p.x, p.y, p.z, 
		tipo_reproduccion, id_sonido, vol, 
		pan, param1, param2, activo, id);
	return resultado;
}

Actor * Factoria_actores::fabricar_plataforma_flexible(const Parametros_factoria& p)
{
	unsigned int w=0;
	unsigned int h=0;
	unsigned int d=0;
	unsigned int letal=0;
	unsigned int solido=0;

	try
	{
		if(p.params.size()!=5) throw std::out_of_range("No params");

		w=p.params.at(0);
		h=p.params.at(1);
		d=p.params.at(2);
		letal=p.params.at(3);
		solido=p.params.at(4);
	}
	catch(const std::out_of_range& e) {error("Plataforma_flexible");}
	Plataforma_flexible * resultado=new Plataforma_flexible(p.x, p.y, p.z, 
		w, h, d, letal, solido);
	return resultado;
}

Actor * Factoria_actores::fabricar_escalera(const Parametros_factoria& p)
{
	unsigned int h=0;
	unsigned int orientacion=0;
	unsigned int flags=0;
	unsigned int activa=0;
	unsigned int id=0;

	try
	{
		if(p.params.size()!=5) throw std::out_of_range("No params");

		h=p.params.at(0);
		orientacion=p.params.at(1);
		flags=p.params.at(2);
		activa=p.params.at(3);
		id=p.params.at(4);
	}
	catch(const std::out_of_range& e) {error("Escalera");}
	Escalera * resultado=new Escalera(p.x, p.y, p.z, h, orientacion, flags, activa, id);
	return resultado;
}


Actor * Factoria_actores::fabricar_actor_diapositiva(const Parametros_factoria& p)
{
	unsigned int id_diapositiva=0;
	unsigned int activa=0;
	unsigned int id=0;
	unsigned int reproducido=0;

	try
	{
		switch(p.params.size())
		{
			case 4:
				reproducido=p.params.at(3);
			case 3:
				id_diapositiva=p.params.at(0);
				activa=p.params.at(1);
				id=p.params.at(2);
			break;
			default: throw std::out_of_range("No params"); break;
		}
	}
	catch(const std::out_of_range& e) {error("Actor_diapositiva");}
	Actor_diapositivas * resultado=new Actor_diapositivas(p.x, p.y, p.z, 
		id_diapositiva, activa, id, reproducido);
	return resultado;
}

Actor * Factoria_actores::fabricar_actor_info_juego(const Parametros_factoria& p)
{
	unsigned int id_info=0;
	unsigned int activo=0;
	unsigned int id=0;

	try
	{
		if(p.params.size()!=3) throw std::out_of_range("No params");

		id_info=p.params.at(0);
		activo=p.params.at(1);
		id=p.params.at(2);
	}
	catch(const std::out_of_range& e) {error("Actor_info_juego");}
	Actor_info_juego * resultado=new Actor_info_juego(p.x, p.y, p.z, id_info, activo, id);
	return resultado;
}


Actor * Factoria_actores::fabricar_interruptor_contacto(const Parametros_factoria& p)
{
	unsigned int w=0;
	unsigned int h=0;
	unsigned int d=0;
	unsigned int id_in=0;
	unsigned int id_out=0;
	unsigned int repetible=0;
	unsigned int activo=0;
	unsigned int id=0;
	unsigned int c_in=0;
	unsigned int c_out=0;

	try
	{
		switch(p.params.size())
		{
			case 10:
				c_in=p.params.at(8);
				c_out=p.params.at(9);
			case 8:
				w=p.params.at(0);
				h=p.params.at(1);
				d=p.params.at(2);
				id_in=p.params.at(3);
				id_out=p.params.at(4);
				repetible=p.params.at(5);
				activo=p.params.at(6);
				id=p.params.at(7);
			break;

			default: throw std::out_of_range("No params"); break;
		}
	}
	catch(const std::out_of_range& e) {error("Interruptor_contacto");}
	Interruptor_contacto * resultado=new Interruptor_contacto(
		p.x, p.y, p.z, 
		w, h, d, 
		id_in, id_out, repetible, activo, id,
		c_in, c_out);
	return resultado;
}

Actor * Factoria_actores::fabricar_teletransportador(const Parametros_factoria& p)
{
	unsigned int id_teletransportador=0;
	unsigned int id_conexion=0;

	try
	{
		if(p.params.size()!=2) throw std::out_of_range("No params");

		id_teletransportador=p.params.at(0);
		id_conexion=p.params.at(1);
	}
	catch(const std::out_of_range& e) {error("Teletransportador");}

	Teletransportador * resultado=new Teletransportador(p.x, p.y, p.z, id_teletransportador, id_conexion);
	return resultado;
}

Actor * Factoria_actores::fabricar_actor_puzzle(const Parametros_factoria& p)
{
	unsigned int id_puzzle=0;
	unsigned int id_cadena=0;
	unsigned int id=0;
	unsigned int estado=0;

	try
	{
		switch(p.params.size())
		{
			case 4: estado=p.params.at(3); 
			case 3:
				id_puzzle=p.params.at(0);
				id_cadena=p.params.at(1);
				id=p.params.at(2);
			break;
			default: throw std::out_of_range("No params"); break;
		}
	}
	catch(const std::out_of_range& e) {error("Actor puzzle");}

	Actor_puzzle * resultado=new Actor_puzzle(p.x, p.y, p.z, id_puzzle, id_cadena, id, estado);
	return resultado;
}

Actor * Factoria_actores::fabricar_actor_punto_guardado(const Parametros_factoria& p)
{
	unsigned int id_entrada=0;

	try
	{
		if(p.params.size()!=1) throw std::out_of_range("No params");
		id_entrada=p.params.at(0);
	}
	catch(const std::out_of_range& e) {error("Actor punto guardado");}

	Actor_punto_guardado * resultado=new Actor_punto_guardado(p.x, p.y, p.z, id_entrada);
	return resultado;
}

Actor * Factoria_actores::fabricar_actor_recuperar_tiempo(const Parametros_factoria& p)
{
	unsigned int w=0;
	unsigned int h=0;
	unsigned int d=0;

	try
	{
		if(p.params.size()!=3) throw std::out_of_range("No params");
		w=p.params.at(0);
		h=p.params.at(1);
		d=p.params.at(2);
	}
	catch(const std::out_of_range& e) {error("Actor recuperar tiempo");}

	Actor_recuperar_tiempo * resultado=new Actor_recuperar_tiempo(p.x, p.y, p.z, w, h, d);
	return resultado;
}

Actor * Factoria_actores::fabricar_interruptor_auto(const Parametros_factoria& p)
{
	unsigned int id_cadena=0;
	unsigned int id=0;
	unsigned int activo=0;
	unsigned int usado=0;

	try
	{
		switch(p.params.size())
		{
			case 4:
				usado=p.params.at(3);
			case 3:
				id_cadena=p.params.at(0);
				id=p.params.at(1);
				activo=p.params.at(2);
			break;
			default: throw std::out_of_range("No params"); break;
		}
	}
	catch(const std::out_of_range& e) {error("Interruptor auto");}

	Interruptor_auto * resultado=new Interruptor_auto(p.x, p.y, p.z, id_cadena, id, activo, usado);
	return resultado;
}

Actor * Factoria_actores::fabricar(unsigned int pt, const Parametros_factoria& params)
{
	Actor * resultado=NULL;

	switch(pt)
	{
		case TF_ENTRADA_SALA: 				resultado=fabricar_entrada_sala(params); break;
		case TF_SALIDA_SALA:				resultado=fabricar_salida_sala(params); break;
		case TF_INTERRUPTOR_REPETIBLE:			resultado=fabricar_interruptor_repetible(params); break;
		case TF_INTERRUPTOR_NO_REPETIBLE:		resultado=fabricar_interruptor_no_repetible(params); break;
		case TF_INTERRUPTOR_INVISIBLE_NO_REPETIBLE:	resultado=fabricar_interruptor_invisible_no_repetible(params); break;
		case TF_INTERRUPTOR_INVISIBLE_REPETIBLE:	resultado=fabricar_interruptor_invisible_repetible(params); break;
		case TF_COFRE:					resultado=fabricar_cofre(params); break;
		case TF_CONTROL_DIRECCION:			resultado=fabricar_control_direccion(params); break;
		case TF_DISPARADOR_PROYECTILES:			resultado=fabricar_disparador_proyectiles(params); break;
		case TF_ACTOR_GENERICO:				resultado=fabricar_actor_generico(params); break;
		case TF_ALTERNADOR_ACTOR_GENERICO:		resultado=fabricar_alternador_actor_generico(params); break;
		case TF_PROYECTIL_OBSTACULO:			resultado=fabricar_proyectil_obstaculo(params); break;
		case TF_ACTOR_AUDIO:				resultado=fabricar_actor_audio(params); break;
		case TF_PLATAFORMA_FLEXIBLE:			resultado=fabricar_plataforma_flexible(params); break;
		case TF_ESCALERA:				resultado=fabricar_escalera(params); break;
		case TF_ACTOR_DIAPOSITIVA:			resultado=fabricar_actor_diapositiva(params); break;
		case TF_INTERRUPTOR_CONTACTO:			resultado=fabricar_interruptor_contacto(params); break;
		case TF_TELETRANSPORTADOR:			resultado=fabricar_teletransportador(params); break;
		case TF_ACTOR_INFO_JUEGO:			resultado=fabricar_actor_info_juego(params); break;
		case TF_ACTOR_PUZZLE:				resultado=fabricar_actor_puzzle(params); break;
		case TF_ACTOR_PUNTO_GUARDADO:			resultado=fabricar_actor_punto_guardado(params); break;
		case TF_ACTOR_RECUPERAR_TIEMPO:			resultado=fabricar_actor_recuperar_tiempo(params); break;
		case TF_INTERRUPTOR_AUTO:			resultado=fabricar_interruptor_auto(params); break;
	}

	return resultado;
}
