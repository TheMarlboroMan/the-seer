#include "jugador.h"

const float Jugador::MAX_VELOCIDAD_CAMINAR=40.0;
const float Jugador::MAX_VELOCIDAD_CORRER=80.0;
const float Jugador::MIN_VELOCIDAD=20.0;
const float Jugador::ACELERACION=80.0;
const float Jugador::TIEMPO_PAUSA_CAIDA=0.7;
const float Jugador::TIEMPO_PERDER_VIDA=1.2;
const float Jugador::TIEMPO_SUBIR_BAJAR_ESCALERA=0.1;
const float Jugador::T_DURACION_ANIMACION_CAMINAR=1.05;
const float Jugador::T_DURACION_PASO_CAMINAR=0.150;
const float Jugador::T_DURACION_CAIDA_0=0.50;
const float Jugador::T_DURACION_CAIDA_1=0.60;

extern DLibH::Log_base LOG;

float Jugador::obtener_vector_movimiento_contrario(unsigned int eje)
{
	if(eje==Definiciones::X) return acc_vector().y;
	else if(eje==Definiciones::Y) return acc_vector().x;
	else return 0.0;
}

/*
Esto no es lo que parece... Era una limitación hard para los vectores, no para
el movimiento. El método "activar movimiento" del actor móvil hace que no puedas
moverte más rápido de tu propio tamaño.*

float Jugador::vector_limite(unsigned int flags)
{
	if(flags & Definiciones::X || flags & Definiciones::Y) 
	{
		return W;
	} 
	else return H;

//	return 0.0;
}*/

void Jugador::activar_habilidad_impulso(float c)
{
	establecer_estado(E_IMPULSO);

	switch(direccion)
	{
		case Definiciones::POS_S:
			establecer_eje_vector(c, Definiciones::Y);
		break;
		
		case Definiciones::POS_N:
			establecer_eje_vector(-c, Definiciones::Y);
		break;

		case Definiciones::POS_E:
			establecer_eje_vector(c, Definiciones::X);
		break;

		case Definiciones::POS_O:
			establecer_eje_vector(-c, Definiciones::X);
		break;
	}
}

Jugador::Jugador(int px, int py, int pz):
	Actor_movil(px, py, pz), 
	estado(E_AIRE),
	direccion(Definiciones::POS_S), 
	direccion_aire(Definiciones::POS_S), max_velocidad_aire(0.0),
	corriendo(false), tiempo_animacion(0.0), tiempo_turno(0.0)
{
	actualizar_volumen();
}

Jugador::~Jugador()
{

}

void Jugador::transformar_representacion(Bloque_transformacion_representable& br)
{
	br.usar_bitmap();
	br.establecer_recurso_sin_recortar(5);
	Punto_2d p=obtener_coordenadas_origen_representacion();
	p.x-=8;
	p.y-=16;

	unsigned int y=0;
	unsigned int x=0;
	unsigned int h=0;
	unsigned int w=W_REP;
	Vector_3d v=acc_vector();

	switch(direccion)
	{
		case Definiciones::POS_N: x=X_REP_N; break;
		case Definiciones::POS_S: x=X_REP_S; break;
		case Definiciones::POS_E: x=X_REP_E; break;
		case Definiciones::POS_O: x=X_REP_O; break;
	}

	switch(estado)
	{
		case E_AIRE:
			//Animaciones de salto o caer.
			if(v.z > 0.0) y=Y_REP_SALTANDO_SUBIR;
			else y=Y_REP_SALTANDO_BAJAR;

			h=H_REP_SALTANDO;
		break;

		case E_ESCALERA:
		case E_SUBIENDO_ESCALERA:
		{
			//Cada tres píxeles cambia el paso.
			int pos_z=acc_z();
			unsigned int paso=floor(pos_z % 12);
			
			paso/=3;

			if(paso==2) paso=0;
			else if(paso==3) paso=2;

			y=Y_REP_ESCALERA + (paso * H_REP_ESCALERA);
			h=H_REP_ESCALERA;
		}
		break;
 
		case E_IMPULSO:
		case E_FIN_IMPULSO:
			y=Y_REP_IMPULSO;
			h=H_REP_IMPULSO;
		break;

		case E_TIERRA:
		case E_BAJANDO_ESCALERA:
		{
			//Animación caminar o correr...
			if(v.x || v.y)
			{
				float tc=fmod(tiempo_animacion, T_DURACION_ANIMACION_CAMINAR);
				unsigned int paso=floor(tc / T_DURACION_PASO_CAMINAR);
		
				if(paso==3) paso=1;
				else if(paso==4) paso=0;
				else if(paso==5 || paso==7) paso=3;
				else if(paso==6) paso=4;

				y=Y_REP_PARADO + (paso * H_REP_PARADO);
			}
			else y=Y_REP_PARADO;

			h=H_REP_PARADO;
		}
		break;

		case E_PAUSA_CAIDA:

			if(tiempo_turno <= T_DURACION_CAIDA_0)
			{
				y=Y_REP_CAIDA_0;
				h=H_REP_CAIDA_0;
			}
			else if(tiempo_turno <= T_DURACION_CAIDA_1)
			{
				y=Y_REP_CAIDA_1;
				h=H_REP_CAIDA_1;
			}
			else
			{
				y=Y_REP_CAIDA_2;
				h=H_REP_CAIDA_2;
			}

			//Un pequeño ajuste manual.
			p.y+=(32-h) / 2;
		break;

		/*El sprite se va disolviendo... según el tiempo pasado. 0 de
		tiempo es 26px de altura y TIEMPO_PERDER_VIDA es cero... Esto
		es una ecuación lineal or something. Podríamos precomputar 
		los valores si hiciera falta.*/	
		case E_PERDIENDO_VIDA:
		{
			x=0;
			y=Y_REP_PERDER_VIDA;
			//La pendiente: m=(yb-ya)/(xb-xa)...            
			float m=-(H_REP_PERDER_VIDA) / TIEMPO_PERDER_VIDA; //(0.0-26.0)/(TIEMPO_PERDER_VIDA-0.0);

			//La constante b... b=y-(mx)
			float b=H_REP_PERDER_VIDA; //32.0-(m*0.0);

			//y=mx+b...
			h=(int) ((m*tiempo_turno)+b);
		}
		break;

		case E_FIN_PERDER_VIDA:
			w=0;
			h=0;
		break;
	}

	br.establecer_recorte(x, y, w, h);
	br.establecer_posicion(p.x, p.y, 16, 32);
}

void Jugador::establecer_estado(unsigned  v)
{
	estado=v;

	switch(estado)
	{
		case E_TIERRA:
//Nope... El vector será cancelado por el flujo normal
//de colisiones.
//			cancelar_vector(Definiciones::Z);
		break;

		case E_AIRE:
			direccion_aire=direccion;
		break;

		case E_IMPULSO:
			cancelar_vector(Definiciones::X);
			cancelar_vector(Definiciones::Y);
			cancelar_vector(Definiciones::Z);
		break;

		case E_SUBIENDO_ESCALERA:
		case E_BAJANDO_ESCALERA:
		case E_ESCALERA:
			cancelar_vector(Definiciones::X);
			cancelar_vector(Definiciones::Y);
			cancelar_vector(Definiciones::Z);
			tiempo_turno=0.0;
		break;

		case E_PERDIENDO_VIDA:
			cancelar_vector(Definiciones::X);
			cancelar_vector(Definiciones::Y);
			cancelar_vector(Definiciones::Z);
			tiempo_turno=0.0;
		break;
		case E_PAUSA_CAIDA:
			cancelar_vector(Definiciones::X);
			cancelar_vector(Definiciones::Y);
			//cancelar_vector(Definiciones::Z);
			tiempo_turno=0.0;
		break;
	}
}

/*Esto apluca el movimiento al jugador según el input: por ejemplo, para andar,
acelerar o frenar en salto, correr o moverse por las escaleras.
Los movimientos automáticos como el impulso no se cuentan aquí.

Controla también el freno de estos movimientos.*/

void Jugador::recibir_input_movimiento(float v, float delta, unsigned int eje)
{
	bool sumar=false, prohibir_cambio_direccion=false;
	float factor_suma=1.0;

	switch(estado)
	{
		case E_TIERRA:
			//Para sumar movimiento deberiamos tener una intención
			//de movimiento del mismo signo que el vector actual
			//o, en su defecto, que no haya vector actual.

			if(v &&
				(!vector_para_eje(eje) || v * vector_para_eje(eje) >= 0.0f))
			{
				sumar=true;
			}
			else
			{
				if(fabs(vector_para_eje(eje)) >= MAX_VELOCIDAD_CAMINAR) decelerar_vector(delta * 600, eje);
				else cancelar_vector(eje);
			}
		break;

	//Si está en el aire hay que considerar que pueda aumentar un poco
	//su vector o acortarlo. Esto es para, por ejemplo, saltar contra
	//una plataforma y poder subirse encima (el vector se cancela al
	//chocar) o para controlar mejor los saltos. Si no se recibe
	//input se continúa como siempre. Esto no debe verse afectado
	//por si estamos corriendo o no. Por otro lado no debe permitir
	//cambios de eje ni dirección en el aire.

		case E_AIRE:
			if(v && Definiciones::posicion_corresponde_con_eje(direccion_aire, eje))
			{
				sumar=true;
				prohibir_cambio_direccion=true;
				factor_suma=0.5;	//La capacidad de afectar se reduce a la mitad.		
			}
		break;

	//En la escalera la velocidad es fija. No hay mucho más que hacer excepto
	//controlarla. Si hay input se mueve, si no se detiene en seco.

		case E_ESCALERA:
			if(v)
			{
				v=-v; //Invertir valor para que arriba sea subir.
				establecer_eje_vector(v * MAX_VELOCIDAD_CAMINAR, Definiciones::Z);
				sumar=true;
			}
			else
			{		
				cancelar_vector(Definiciones::Z);
			}
		break;

		default:
			sumar=false;
		break;
	}

	//Mucho ojo!!!!!. Esto es para caminar, impulso... Para escalera
	//está arriba, en el caso de la escalera.

	if(sumar)
	{
		//A la hora de calcular el movimiento tenemos una 
		//aceleración definida y además podemos coger parte
		//del vector de movimiento en el otro eje (para no empezar
		//desde el mínimo a cada giro).

		float abs_contrario=fabs(obtener_vector_movimiento_contrario(eje));
//		float factor=v * ( (ACELERACION * delta)  + (abs_contrario / 2 ));
		float factor=v * ( (ACELERACION * delta)  + (abs_contrario));

		sumar_vector(factor * factor_suma, eje);
		float vactual=vector_para_eje(eje);

		//Ahora nos ventilaríamos el vector contrario, para impedir 
		//movimientos en diagonal. Dado que el controlador procesa 
		//primero el eje x, este tiene prioridad sobre el eje y, que
		//no se procesaría.

		if(abs_contrario)
		{
			if(eje==Definiciones::X) establecer_eje_vector(0.0, Definiciones::Y);
			else if(eje==Definiciones::Y) establecer_eje_vector(0.0, Definiciones::X);
		}

		//Control de velocidad máxima y mínima... La velocidad
		//mínima se tiene en cuenta como punto de partida del
		//movimiento pero no debe influir al cambiar de dirección.

		float abs_vactual=fabs(vactual);

		//La velocidad máxima es un poco hack: en el aire es la máxima. 
		//En el suelo depende de si corres o no.

		float v_max=0.0;
		float v_min=0.0;

		switch(estado)
		{
			case E_AIRE: 
				v_max=max_velocidad_aire; 
				v_min=0.0; 
			break;
			case E_TIERRA: 
				v_max=corriendo ? MAX_VELOCIDAD_CORRER : MAX_VELOCIDAD_CAMINAR; 
				v_min=MIN_VELOCIDAD;
			break;
			case E_ESCALERA: 
				v_max=MAX_VELOCIDAD_CAMINAR; 
				v_min=MIN_VELOCIDAD;
			break;
		}

		if(abs_vactual > v_max)
		{
			if(vactual >= 0) vactual=v_max;
			else vactual=-v_max;
		}
		else if(abs_vactual < v_min)
		{
			if(vactual >= 0) vactual=v_min;
			else vactual=-v_min;
		}

		establecer_eje_vector(vactual, eje);
		
		//Si el resultado implica un cambio de dirección 
		//y está prohibido, lo bloqueamos...
		if(prohibir_cambio_direccion)
		{
			float veje=vector_para_eje(eje);
			if(
				(eje==Definiciones::X && (
					(veje <  0.0 && direccion==Definiciones::POS_E)
					|| (veje > 0.0 && direccion==Definiciones::POS_O)
				))
				||
				(eje==Definiciones::Y && (
					(veje < 0.0 && direccion==Definiciones::POS_S)
					|| (veje > 0.0 && direccion==Definiciones::POS_N)
				))

			)
			{
				establecer_eje_vector(0.0, eje);
			}
		}
	}
}

void Jugador::activar_movimiento(float p_delta, unsigned int flags)
{
	tiempo_animacion+=p_delta;

	//Cuando esta en impulso o escalera no tiene gravedad.
	if( (flags & Definiciones::Z) && (estado!=E_ESCALERA && estado!=E_IMPULSO) )
	{
		//Integración de verlet.
		float ov=acc_vector().z;
		sumar_vector(VECTOR_GRAVEDAD * p_delta, Definiciones::Z);
		float z=(ov + acc_vector().z) * 0.5 * p_delta;
		if(z > acc_w()) z=acc_h();
		sumar_posicion(0.0, 0.0, z);			

		Actor_movil::activar_movimiento(p_delta, flags ^ Definiciones::Z);
	}
	else
	{
		Actor_movil::activar_movimiento(p_delta, flags);
	}

	Vector_3d v=acc_vector();
	if(v.y > 0) direccion=Definiciones::POS_S;
	else if(v.y < 0) direccion=Definiciones::POS_N;
	else if(v.x > 0) direccion=Definiciones::POS_E;
	else if(v.x < 0) direccion=Definiciones::POS_O;
}

void Jugador::procesar_turno(float delta)
{
	tiempo_turno+=delta;

	switch(estado)
	{
		case E_PAUSA_CAIDA:
			if(tiempo_turno >= TIEMPO_PAUSA_CAIDA)
			{
				tiempo_turno=0.0;
				establecer_en_tierra();
			}
		break;

		case E_PERDIENDO_VIDA:
			if(tiempo_turno >= TIEMPO_PERDER_VIDA)
			{
				tiempo_turno=0.0;
				establecer_estado(E_FIN_PERDER_VIDA);
			}
		break;

		case E_SUBIENDO_ESCALERA:
			if(tiempo_turno >= TIEMPO_SUBIR_BAJAR_ESCALERA)
			{
				tiempo_turno=0.0;
				establecer_estado(E_ESCALERA);
			}
		break;

		case E_BAJANDO_ESCALERA:
			if(tiempo_turno >= TIEMPO_SUBIR_BAJAR_ESCALERA)
			{
				tiempo_turno=0.0;
				establecer_estado(E_TIERRA);
			}
		break;

		case E_IMPULSO:
		{
			Vector_3d v=acc_vector();
			if(abs(v.x + v.y) <= MAX_VELOCIDAD_CORRER) 
			{
				establecer_estado(E_FIN_IMPULSO);
			}
			else 
			{
				decelerar_vector(delta * 600, Definiciones::X);
				decelerar_vector(delta * 600, Definiciones::Y);
			}
		}
		break;
	}
}

/*Al saltar se aplica un vector Z y también se anula el vector de dirección
en el plano que sea distinto de la dirección. */

void Jugador::saltar() 
{
	sumar_vector(VECTOR_SALTO, Definiciones::Z);

	if(estado==E_ESCALERA)
	{
		//Invertimos la dirección y el vector.
		direccion=Definiciones::obtener_posicion_opuesta(direccion);
		establecer_eje_vector(
			(Definiciones::obtener_multiplicador_posicion(direccion) * MAX_VELOCIDAD_CAMINAR), 
			Definiciones::obtener_eje_para_posicion(direccion) );
	}
	else
	{
		if(direccion==Definiciones::POS_N || direccion==Definiciones::POS_S) cancelar_vector(Definiciones::X);
		else cancelar_vector(Definiciones::Y);

		max_velocidad_aire=corriendo ? MAX_VELOCIDAD_CORRER : MAX_VELOCIDAD_CAMINAR;
	}

	establecer_estado(E_AIRE);
}

/*Parecido al salto anterior, pero detiene el movimiento en seco y no puede
hacerse desde una escalera.*/

void Jugador::super_saltar()
{
	sumar_vector(VECTOR_SUPER_SALTO, Definiciones::Z);
	cancelar_vector(Definiciones::X);
	cancelar_vector(Definiciones::Y);
	max_velocidad_aire=MAX_VELOCIDAD_CORRER;
	establecer_estado(E_AIRE);
}

void Jugador::establecer_en_caida()
{
	max_velocidad_aire=corriendo ? MAX_VELOCIDAD_CORRER : MAX_VELOCIDAD_CAMINAR;
	establecer_estado(E_AIRE);
}

void Jugador::decelerar_vector(float valor, unsigned int flags)
{
	if(flags & Definiciones::X && estado!=E_AIRE) Actor_movil::decelerar_vector(valor, Definiciones::X);
	if(flags & Definiciones::Y && estado!=E_AIRE) Actor_movil::decelerar_vector(valor, Definiciones::Y);
	if(flags & Definiciones::Z) Actor_movil::decelerar_vector(valor, Definiciones::Z);
}

Cubo Jugador::obtener_cubo_snap() const
{
	Cubo resultado=como_cubo();
	resultado.origen.z-=SNAP_CUBO;
	if(resultado.origen.z < 0) resultado.origen.z=0;
	resultado.h+=SNAP_CUBO;
	return resultado;
}

Cubo Jugador::obtener_cubo_descenso_escalera() const
{
	Cubo resultado=como_cubo();
	resultado.origen.z-=H_DESCENSO_ESCALERA;
	return resultado;
}


Cubo Jugador::obtener_cubo_interaccion() const
{
	Cubo resultado=como_cubo();

	switch(direccion)
	{
		case Definiciones::POS_N: 
			resultado.origen.y-=DISTANCIA_INTERACCION; 
			resultado.d+=DISTANCIA_INTERACCION; 
		break;
		case Definiciones::POS_S: 
			resultado.d+=DISTANCIA_INTERACCION; 
		break;
		case Definiciones::POS_E:
			resultado.w+=DISTANCIA_INTERACCION;
		break;
		case Definiciones::POS_O: 
			resultado.origen.x-=DISTANCIA_INTERACCION;
			resultado.w+=DISTANCIA_INTERACCION; 
		break;
	}

	resultado.origen.z++;
	resultado.h--;

	return resultado;
}

/*Devuelve true cuando hace snap.*/

bool Jugador::recibir_snap(int cantidad)
{
	if(cantidad > 0) //-1 es lo que dan los erróneos. 0 no es considerable.
	{
		Cubo c=como_cubo();
		int oz=c.origen.z;
		int z=floor(oz);
		int diferencia=abs(cantidad - z);

		//Mayor o igual que cero para aterrizar de un salto
		//con altos FPS y que no se quede con decimales
		//de más.
		if(diferencia >= 0 && diferencia <= MAX_SNAP_Z)
		{
			establecer_posicion_por_eje(cantidad, Definiciones::Z);
			return true;
		}
	}

	return false;
}
