#include "bloque.h"

Controlador_datos_representacion_bloque Bloque::controlador_datos_representacion;

extern DLibH::Log_base LOG;

Bloque::Bloque(int px, int py, int pz, unsigned int ptc, unsigned int ptr):
	Representable(),
	Espaciable(px*W_BLOQUE, py*D_BLOQUE, pz*H_BLOQUE, W_BLOQUE, D_BLOQUE, H_BLOQUE), 
	id(0),
	matriz_x(px), matriz_y(py), matriz_z(pz), 
	plano_representacion(Representable::PL_NORMAL),
	tipo_comportamiento(ptc), tipo_representacion(ptr),
	debug_bloque(false), coordenadas_origen(0,0)
{
	coordenadas_origen=obtener_coordenadas_representacion(
		obtener_x_representable(),
		obtener_y_representable(),
		obtener_z_representable());
}

Bloque::~Bloque() 
{
}

bool Bloque::operator==(const Bloque& bloque) const
{
	return matriz_x==bloque.matriz_x && 
		matriz_y==bloque.matriz_y &&
		matriz_z==bloque.matriz_z; 
}

/*
bool Bloque::operator<(const Bloque& bloque) const
{
	if(matriz_z!=bloque.matriz_z) return matriz_z < bloque.matriz_z;
	else
	{
		if(matriz_y!=bloque.matriz_y) return matriz_y < bloque.matriz_y;
		else return matriz_x < bloque.matriz_x;
	} 
}
*/


bool Bloque::rep_final_es_dentro_de_caja(int dx, int dy, const Bloque_transformacion_representable& bt, const SDL_Rect& caja) const
{
	SDL_Rect bloque=bt.acc_posicion();
	bloque.x+=dx;
	bloque.y+=dy;
	return DLibH::Herramientas_SDL::rectangulos_superpuestos(bloque, caja); 
}


void Bloque::transformar_representacion(Bloque_transformacion_representable& br)
{
	//Obtener coordenadas de origen.
	br.usar_bitmap();
	Punto_2d p=coordenadas_origen;

	unsigned int w=0, h=0;

	if(debug_bloque)
	{
		w=32;
		h=47;
		br.establecer_recurso(0);
		br.establecer_recorte(0, 94, w, h);
		p.x-=16;
		p.y-=17;
	}
	else
	{
		Datos_representacion_animada dr=controlador_datos_representacion.obtener_por_id(tipo_representacion);
		br.establecer_recurso(dr.acc_indice_recurso());
		Frame_representacion fr;

		if(dr.es_estatico())
		{
			fr=dr.obtener_frame_unico();
		}
		else
		{
			fr=dr.obtener_para_tiempo_animacion(Temporizador_animaciones::tiempo_animacion);
		}

		p.x-=fr.correccion_x;
		p.y-=fr.correccion_y;
		w=fr.w;
		h=fr.h;
		br.establecer_recorte(fr.x, fr.y, fr.w, fr.h);

//br.establecer_recurso(0);
//		br.establecer_recorte(0, 0, 32, 32);
	}

	br.establecer_posicion(p.x, p.y, w, h);
}

void Bloque::rellenar_info_colision(Datos_colision& datos_colision) const
{
	switch(tipo_comportamiento)
	{
//Esto se mueve al visitante.
//		case TC_LETAL:
//			datos_colision.tipo_colision=Datos_colision::TC_LETAL;
//		break;

		//Si chocamos con una rampa queremos, en esta fase, ver
		//anulamos la cantidad de la colisión.
		case TC_RAMPA_O_E:
		case TC_RAMPA_E_O:
		case TC_RAMPA_S_N:
		case TC_RAMPA_N_S:
		case TC_RAMPA_NO_ESCALABLE_O_E:
		case TC_RAMPA_NO_ESCALABLE_E_O:
		case TC_RAMPA_NO_ESCALABLE_S_N:
		case TC_RAMPA_NO_ESCALABLE_N_S:
			calcular_solidez_rampa(datos_colision);
			datos_colision.tipo_colision=Datos_colision::TC_NADA;
		break;

		default:
			datos_colision.tipo_colision=Datos_colision::TC_NADA;
		break;
	}
}

/*En parado, calcula si un cubo está libre en un bloque de rampa.*/

bool Bloque::cubo_libre_en_rampa(const Cubo& c, bool verbose) const
{
	int z=calcular_z_rampa(c, verbose);

//if(verbose) LOG<<"CUBO LIBRE EN RAMPA DICE QUE "<<c.origen.z<<" >= "<<z<<" PARA DEVOLVER TRUE"<<std::endl;

	return c.origen.z >= z;
}

void Bloque::calcular_solidez_rampa(Datos_colision& datos_colision) const
{
	//En función de la posición anterior del cubo a de colisión podemos saber 
	//desde dónde venimos.

	unsigned int pos=Cubo::posicion_relativa_en_eje(datos_colision.cubo_a_original, datos_colision.cubo_b, datos_colision.eje);
	unsigned int z=calcular_z_rampa(datos_colision.cubo_a);

	const Info_rampa ir=Info_rampa::obtener_info_rampa(tipo_comportamiento);

	/*Este define la colision por el eje en el que está la rampa. 
	Que en un lado tiene rampa y por el otro es sólido.*/

	if(datos_colision.eje==ir.eje_rampa)
	{
		if(pos==ir.pos_rampa_solido)
		{
			//Por aquí es sólido.
		}	
		else if(pos==ir.pos_rampa_canto)
		{

//TODO... A grandes velocidades te comes los mocos. Tendríamos que especificar
//de alguna manera que estamos pasando de una rampa a otra y llevarlo por todo
//el controlador hasta aquí... Esto viene de rellenar_datos_colision, que a 
//su vez viene del calculo de colisiones, que viene del controlador.

//La otra opción es, si el jugador está en tierra, calcular antes de cada step
//si está encima de una rampa, para activar esto o desactivarlo de forma estática
//en el bloque.

			//Ese + 2 es para dar un poco de márgen al pasar de una rampa a otra.

			
//			if(datos_colision.cubo_a.origen.z + 2 >= como_cubo().origen.z)
//			if(datos_colision.cubo_a.origen.z + 16 >= como_cubo().origen.z)
			if(es_rampa_escalable() || datos_colision.cubo_a.origen.z + 2 >= como_cubo().origen.z)
			{
				datos_colision.cantidad=0;
				datos_colision.correccion_z=z;
//				std::cout<<"BOOM ESCALABLE O CERCA"<<std::endl;
			}
			else
			{
//				std::cout<<"BOOM CANTO"<<std::endl;
			}
		}
		else if(pos==Definiciones::POS_DENTRO)
		{
			//Esto representa el estar dentro de la rampa.
			datos_colision.cantidad=0;
			datos_colision.correccion_z=z;
		}
	}
	/*Este define la colisión con el perfil que no tiene rampa,
	que es sólido siempre que estés por debajo de la altura del 
	perfil de la rampa.*/
	else if(datos_colision.eje==ir.eje_canto)
	{
		switch(pos)
		{
			//Por aquí es accesible...
			case Definiciones::POS_DENTRO: 
				datos_colision.cantidad=0;
				datos_colision.correccion_z=z;
			break;
			default: 
				if(datos_colision.cubo_a.origen.z < z) 
				{
					//Por aquí es sólido de nuevo.
				}
				else
				{
					datos_colision.cantidad=0;
				}
			break;
		}
	}
	//Esto se queda igual en cualquier tipo de rampa. 
	else
	{
		switch(pos)
		{
			case Definiciones::POS_ABA: 
				//Por aquí es sólido.
			break; 
			default:
				//Por aquí no.
				datos_colision.cantidad=0;
				if(datos_colision.cubo_a.origen.z < z) 
				{
					datos_colision.correccion_z=z;
				}
			break;
		}
	}
}

int Bloque::calcular_z_encima(const Cubo& c) const
{
	if(es_completo())
	{
		const Cubo& cbl=como_cubo();
		return cbl.origen.z + cbl.h;
	}
	else if(es_tipo_rampa())
	{
		return calcular_z_rampa(c);
	}
	else return -1;
}

/*float*/ int Bloque::calcular_z_rampa(const Cubo& c, bool verbose) const
{
	const Cubo& cbl=como_cubo();
	float pos=0, origen=0, cantidad_entrada=0, corte_z=0;

	switch(tipo_comportamiento)
	{
		case TC_RAMPA_O_E:
		case TC_RAMPA_NO_ESCALABLE_O_E:
			pos=c.origen.x + c.w;
			origen=cbl.origen.x;
			cantidad_entrada=pos - origen;
		break;
		
		case TC_RAMPA_E_O:
		case TC_RAMPA_NO_ESCALABLE_E_O:
			pos=c.origen.x;
			origen=cbl.origen.x + cbl.w;
			cantidad_entrada=origen-pos;
		break;

		case TC_RAMPA_S_N:
		case TC_RAMPA_NO_ESCALABLE_S_N:
			pos=c.origen.y;
			origen=cbl.origen.y + cbl.d;
			cantidad_entrada=origen-pos;
		break;

		case TC_RAMPA_N_S:
		case TC_RAMPA_NO_ESCALABLE_N_S:
			pos=c.origen.y + c.d;
			origen=cbl.origen.y;
			cantidad_entrada=pos - origen;
		break;
	}

	cantidad_entrada=ceil(cantidad_entrada);

	if(cantidad_entrada <= 0) //Prácticamente por fuera.
	{
		corte_z=cbl.origen.z;
	}
	if(cantidad_entrada >= W_BLOQUE) //Por el final.
	{
		corte_z=cbl.origen.z+cbl.h;
	}
	else if(cantidad_entrada > 0 && cantidad_entrada < W_BLOQUE) //Dentro.
	{	
		corte_z=cbl.origen.z+cantidad_entrada;
	}

//if(verbose) LOG<<"SE CALCULA LA Z EN LA RAMPA DE "<<corte_z<<" PARA EL CUBO "<<cbl.origen.x<<","<<cbl.origen.y<<","<<cbl.origen.z<<std::endl;
	
	return corte_z;
}
