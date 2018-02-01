#include "calculo_colisiones.h"

/*
Se asume que los bloques son sólidos o no. Si un bloque no es sólido no puede
tener ningún tipo de efecto especial: simplemente es espacio vacío. Un bloque
sólido puede tener efecto especial o no. Tanto si tiene efecto como si no lo 
tiene se devolverá. Luego se separarían desde un controlador para aplicar los 
efectos que fueran necesarios.

El sistema de colisión va a ser relativamente simplista: no va a haber bloques 
móviles ni nada, así que le pasamos el actor, una sala completa y un eje para 
comparar. Cogerá los bloques posibles, comprobará si hay colisión y si la hay 
rellenará el objeto colisión del resultado con la cantidad de corrección y la 
posición del choque. 

Viendo que no podemos estar en dos sitios a la vez sólo podríamos estar en 
colisión con un bloque y desde un lugar.

Se asume que los actores no son mayores que los bloques. Si lo fueran tendríamos
que hacer cambios en calcular el vector de bloques en los que el actor "está".

En el código hay muchos const comentados en los métodos que pasan vectores de 
actores y bloques. En un principio tiene todo el sentido que sean "const"
puesto que los estamos mirando, pero luego entramos en el tema de almacenarlos
para procesar las colisiones y se pierde completamente el norte en ese sentido.

Hay que tener en cuenta que estos métodos devuelven estructuras completamente
separadas y sin forma de trazar a los actores y bloques que las han generado. 
Te dirá que se ha producido una colisión tal y cual con un cubo así, pero no 
te dirá a partir de qué colisionable se generó (antes lo hacía, pero me parece
super chungo) Si en algún momento se me ocurre una solución mejor veré lo que
puedo hacer.
*/

//bool Calculo_colisiones::habilitar_snap=false;
//float Calculo_colisiones::correccion_z_recibida=0;
Cubo Calculo_colisiones::cubo_a_original=Cubo(0,0,0,0,0,0);
//std::vector<Datos_colision> Calculo_colisiones::colisiones_static=std::vector<Datos_colision>();

//Rellena el vector que se pasa como parámetro.

void Calculo_colisiones::info_colision_completa(const Cubo& c, 
		const Cubo& cubo_sala, 
		/*const*/ std::vector<Bloque *>& bloques, 
		/*const*/ std::vector<Actor*>& actores, float v, unsigned int eje, 
		std::vector<Datos_colision>& colisiones,
		std::vector<Bloque *>& bloques_colision,
		std::vector<Actor *>& actores_colision)
{
	if(v)
	{
/*Comprobar en la sala... Si no se sale de los límites comprobamos ya 
con las cajas que sean. Asumimos siempre que la posición anterior era 
legal. Si no está complementamente contenido es que está parcialmente 
fuera (con suerte, claro. Asumimos movimientos legales.*/

		if(!Cubo::dentro_de(c, cubo_sala, true))
		{
			Datos_colision en_sala=obtener_datos_colision_con_cubo(c, cubo_sala, eje, v, false);
			colisiones.push_back(en_sala);
		}

		//Vamos a mirar primero con los actores...
		Datos_colision con_actores=comprobar_colisiones_cubo_con_actores(c, actores, actores_colision, eje, v);
		if(con_actores.con_solido) colisiones.push_back(con_actores);

		//Y si por ahí no tenemos nada, entonces con los bloques fijos.
		Datos_colision con_bloques=procesar_colisiones_cubo_con_bloques(c, bloques, bloques_colision, eje, v);
		if(con_bloques.con_solido) colisiones.push_back(con_bloques);
	}
}

/*Este saca la información de colisiones con todos los actores que hay
en el nivel. OJO: Se devuelve la colisión que tenga una mayor corrección de 
todas las posibles.*/

Datos_colision Calculo_colisiones::comprobar_colisiones_cubo_con_actores(const Cubo& c, 
	/*const*/ std::vector<Actor*>& actores, 
	std::vector<Actor *>& actores_colision, unsigned int eje, float vector)
{
	Datos_colision patron(eje);	//Se crea en el eje, pero no es sólida!!!.

	for(Actor * a : actores)
	{
		Actor& otro=*a;

		//Aquí se comprueba si hay una colisión de los cubos y si el "otro" es colisionable.
		//Si no lo es (por ejemplo, es un adorno o una sombra), no entrará aquí.
		//Controla la colisión consigo mismo.

		if(otro.es_colisionable() && otro.en_colision_con(c))
		{	
			Datos_colision temp=obtener_datos_colision_con_cubo(c, otro.como_cubo(), eje, vector);
			temp.con_solido=otro.es_solido(); //Lo guardamos, luego cuando vayamos a separar nos interesará saber los que son sólidos y los que no.

//Se almacena para ser procesado por el visitante.
			actores_colision.push_back(&otro);

//Para aquellos que no tienen efecto especial sólo lo almacenamos si 
//tiene una mayor corrección que el resto. Para eso usamos el "patrón". 
//Sólo pasaremos por aquí si la colisión es con un sólido.

			if(temp.con_solido && temp.cantidad > patron.cantidad) 
			{
				//Se copian aquí todos los datos... Es triposo, pero cierto.

				patron.con_solido=true;
				patron.cantidad=temp.cantidad;
				patron.vector=temp.vector;							
				patron.cubo_a_original=temp.cubo_a_original;
				patron.cubo_b=otro.como_cubo();
				patron.posicion_corregida=temp.posicion_corregida;
			}
		}
	}

	//TODO: Nada de esto es necesario ya... Se puede sacar de los dos cubos.
	patron.obtener_posicion_por_vector();
	return patron;
}
		
/*Ojo: no nos llevemos a engaños con esto: si el bloque es colisionable entrará
aquí. Esto incluye bloques que por lo que fuera no sean sólidos pero si
colisionables, puesto que el objetivo es sacar toda la información posible.

Se asume que los bloques realmente están en colisión...
*/

Datos_colision Calculo_colisiones::procesar_colisiones_cubo_con_bloques(
	const Cubo& c, 
	/*const*/ std::vector<Bloque *>& v, 
	std::vector<Bloque *>& bloques_colision, 
	unsigned int eje, 
	float vector)
{
	Datos_colision resultado;

	bool hay_rampas=false;

	for(/*const*/ Bloque * pbl : v)
	{
		if(pbl->es_tipo_rampa()) 
		{	
			hay_rampas=true;
			break;
		}
	}

//LOG<<Definiciones::traducir_eje(eje)<<" - "<<v.size()<<" - rampas "<<hay_rampas<<std::endl;

	for(/*const*/ Bloque * pbl : v)
	{
		Bloque& bl=*pbl;
		Cubo cb=bl.como_cubo();

//LOG<<"Comprobando bloque : "<<bl.acc_tipo_comportamiento()<<std::endl;

		if(bl.es_colisionable()) // && Cubo::en_colision_con(c, cb))
		{
			if(hay_rampas && bl.es_solido_rampa()) continue;
			
			//Esta comprobación no se ha realizado.
			else if(bl.es_tipo_rampa() && bl.cubo_libre_en_rampa(c))
			{
//LOG<<"Determinado cubo libre en rampa"<<std::endl;
				continue;
			} 

			bloques_colision.push_back(&bl);

			Datos_colision temp=obtener_datos_colision_con_cubo(c, cb, eje, vector);

			//Esto es para rampas... Si es letal o no se hará en el visitante.
			bl.rellenar_info_colision(temp);

	/*Para el eje Z nos vale igual una corrección que la colisión real.*/

			if(eje==Definiciones::Z)
			{
				if(temp.es_con_correccion_z() && temp.correccion_z >= resultado.correccion_z) resultado=temp;
				else if(temp.cantidad >= resultado.cantidad) resultado=temp;
			}
			else
			{
	//			if(correccion_z_acum < temp.correccion_z) correccion_z_acum=temp.correccion_z;	

				if(temp.cantidad >= resultado.cantidad)
				{
					if(temp.es_con_correccion_z())
					{
						if(temp.correccion_z >= resultado.correccion_z)
							resultado=temp;
					}
					else resultado=temp;
				}
		 	 }
		}	
	}

	return resultado;
}

Datos_colision Calculo_colisiones::obtener_datos_colision_con_cubo(const Cubo& cubo_actor, const Cubo& cubo, unsigned int eje, float vector, bool desde_fuera)
{
	Datos_colision resultado(eje, true); //Como asumimos que hay colisión, la marcamos como sólido.
	resultado.cantidad=cubo_actor.obtener_correccion_con_cubo(cubo, eje);
	resultado.vector=vector;

	//TODO: Nada de esto es necesario ya... Se puede sacar de los dos cubos.
	resultado.obtener_posicion_por_vector();
	resultado.posicion_corregida=obtener_posicion_corregida_cubo(cubo_actor, cubo, resultado.cantidad, resultado.posicion, desde_fuera);
	resultado.cubo_a_original=cubo_a_original;
	resultado.cubo_a=cubo_actor;
	resultado.cubo_b=cubo;
	return resultado;
}

bool Calculo_colisiones::cubo_en_colision_con_actores(const Cubo& c, const std::vector<Actor*>& v, bool verbose)
{
	for(Actor * a : v)
	{
		Actor& otro=*a;
		//Al no estar envuelto en la estructura de actores, tenemos que comprobarlo todo.
		if(otro.es_colisionable() && otro.es_solido() && otro.en_colision_con(c)) 
		{
			if(verbose) std::cout<<"colision con actor"<<std::endl;
			return true;
		}		
	}

	return false;
}

bool Calculo_colisiones::cubo_en_colision_con_bloques(const Cubo& c, const std::vector<Bloque *>& v, bool verbose)
{
	//Hay colisión en el eje con alguno de esos???

	for(const Bloque * pbl : v)
	{	
		const Bloque& bl=*pbl;
		if(bl.es_colisionable() && bl.es_solido() && Cubo::en_colision_con(c, bl.como_cubo())) 
		{
			if(bl.es_completo()) 
			{
				if(verbose) 
				{
					LOG<<"colision con cubo completo "<<
bl.acc_matriz_x()<<","<<bl.acc_matriz_y()<<","<<bl.acc_matriz_z()<<" ("<<
bl.como_cubo().origen.x<<","<<bl.como_cubo().origen.y<<","<<bl.como_cubo().origen.z<<")"<<std::endl;
				}

				return true;
			}
			else if(bl.es_tipo_rampa() && !bl.cubo_libre_en_rampa(c)) 
			{
				if(verbose) 
				{
					float z_encima=bl.calcular_z_encima(c);
	
					LOG<<"colision con rampa "<<
bl.acc_matriz_x()<<","<<bl.acc_matriz_y()<<","<<bl.acc_matriz_z()<<" ("<<
bl.como_cubo().origen.x<<","<<bl.como_cubo().origen.y<<","<<bl.como_cubo().origen.z<<") Z en posicion cubo como :"<<z_encima<<std::endl;


				}

				return true;
			}
		}
	}
	
	return false;
}

bool Calculo_colisiones::cubo_en_colision_con_bordes_cubo(const Cubo& cubo, const Cubo & grande)
{
	return !Cubo::dentro_de(cubo, grande, true);
}

//Podemos asumir que c2 no se ha movido y c1 si... Por tanto, sólo con la
//posición de la colisión ya podemos calcular la nueva posición de origen de C1:
//el borde de turno :D.	

//TODO: Esto debería ser parte del cubo???.

float Calculo_colisiones::obtener_posicion_corregida_cubo(const Cubo& c1, const Cubo& c2, float cantidad, unsigned int pos, bool desde_fuera)
{
	float resultado=0.0;
	unsigned int eje=Definiciones::obtener_eje_para_posicion(pos);

	if(desde_fuera)
	{
		switch(pos)
		{
			case Definiciones::POS_ABA:
			case Definiciones::POS_O:
			case Definiciones::POS_N: resultado=c2.final_para_eje(eje);	
			break;

			case Definiciones::POS_ARR:
			case Definiciones::POS_E:
			case Definiciones::POS_S: resultado=c2.posicion_para_eje(eje)-c1.dimension_para_eje(eje);
			break;
		}
	}
	else
	{
		switch(pos)
		{
			case Definiciones::POS_ABA:
			case Definiciones::POS_O:
			case Definiciones::POS_N: 
				resultado=c2.posicion_para_eje(eje);
			break;

			case Definiciones::POS_ARR:
			case Definiciones::POS_E:
			case Definiciones::POS_S: 
				resultado=c2.final_para_eje(eje)-c1.dimension_para_eje(eje);
			break;
		}
	}

	return resultado;
}

/*Durante este método se procesa el movimiento del actor en un eje
concreto y se buscan todas las colisiones posibles que tengan con la
propia sala, todos los bloques y todos los actores que haya. A cada
colisión se guardan para usar la mayor de todas al final del eje 
(esto es así porque se hace eje por eje y no podemos hacer un eje sin 
corregir el anterior) y se guarda en las colisiones activas para este 
turno, que procesarían efectos especiales a posteriori.*/ 

Datos_colision Calculo_colisiones::obtener_info_colision_completa(Actor& actor, 
			const Cubo& cubo_sala, 
			/*const*/ std::vector<Bloque*>& bloques, 
			/*const*/ std::vector<Actor*>& actores,	
			float v, unsigned int eje, 
			std::vector<Bloque *>& bloques_colision,
			std::vector<Actor *>& actores_colision)
{

	Datos_colision resultado;
	Cubo c=actor.como_cubo();

	//Si hay vector hacemos todo lo que hay que hacer.
	if(v)
	{
		std::vector<Datos_colision> colisiones;
		info_colision_completa(c, cubo_sala, bloques, actores, v, eje, colisiones, bloques_colision, actores_colision);

/*Aquí lo ordenamos por los valores absolutos de la cantidad y recibimos 
el primero: la colisión mayor, que es el que se contará. El resto de 
colisiones se descartan. Puede que la que entre aquí tuviera, además, 
efecto, pero también se descartaría: el efecto lo usaríamos en otro método.*/

		if(colisiones.size())
		{
			std::sort(colisiones.begin(), colisiones.end());
			resultado=colisiones.at(0);
		}
	}
	//Si no hay vector vamos a estar atentos sólo a las correcciones.
	else	
	{
		std::vector<Bloque*> dummy;
		procesar_colisiones_cubo_con_bloques(c, bloques, dummy, eje, v);
	}

	return resultado;
}

Datos_colision Calculo_colisiones::obtener_info_colision_completa_sin_efectos(
	Actor& actor, 
	const Cubo& cubo_sala, 
	/*const*/ std::vector<Bloque*>& bloques, 
	/*const*/ std::vector<Actor*>& actores,	
	float v, unsigned int eje)
{
	if(v)
	{
		std::vector<Bloque*> dummy;
		std::vector<Actor*> yummy;

		std::vector<Datos_colision> colisiones;
		Cubo c=actor.como_cubo();
		info_colision_completa(c, cubo_sala, bloques, actores, v, eje, colisiones, dummy, yummy);
	
		if(colisiones.size())
		{
			std::sort(colisiones.begin(), colisiones.end());
			return colisiones.at(0);
		}
	}

	return Datos_colision();
}
