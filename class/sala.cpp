#include "sala.h"

extern DLibH::Log_base LOG;

//void Sala::preparar_para_recarga()
//{
//	vaciar_actores();
//	iniciar_bloques();
//}

void Sala::vaciar_actores()
{
	for(Actor * a : actores) delete a;
	actores.clear();
	entradas.clear();
	interactuables.clear();
	controles_direccion.clear();
	con_turno.clear();
	recogibles.clear();
	escaleras.clear();
	overlays.clear();
	teletransportadores.clear();
	actores_persistentes.clear();
	actores_union.clear();
}

void Sala::vaciar_bloques()
{
	//El vector se graba en el orden en que se representaría.
	for(Bloque * bl : bloques) delete bl;
	bloques.clear();

}

void Sala::iniciar_bloques()
{
	vaciar_bloques();
	unsigned int x=0, y=0, z=0;

	for(; z<h; ++z)	
		for(y=0; y<d; ++y)
			for(x=0; x<w; ++x)
				bloques.push_back(new Bloque(x, y, z));
}

Punto Sala::obtener_punto_central() const
{
	float x=(w * Bloque::W_BLOQUE) / 2;
	float y=(d * Bloque::D_BLOQUE) / 2;
	float z=(h * Bloque::H_BLOQUE) / 2;

	return Punto(x,y,z);
}

unsigned int Sala::obtener_indice(unsigned int px, unsigned int py, unsigned int pz) const
{
	unsigned int indice=(pz * (w * d)) + (py * w) + px;
	return indice;
}

//Ninguno de estos puede ser const porque se pueden usar para cambiar bloques...

Bloque& Sala::obtener_bloque(unsigned int px, unsigned int py, unsigned int pz)
{		
	if(px < 0 || px >= w || py < 0 || py >= d || pz < 0 || pz >= h)
	{
		//LOG<<"ERROR: Solicitado bloque erroneo "<<px<<" "<<py<<" "<<pz<<" en sala "<<id<<std::endl;
		return bloque_error;
	}
	else
	{
		unsigned int indice=obtener_indice(px, py, pz);
		return obtener_bloque_por_indice(indice);
	}
}

Bloque& Sala::obtener_bloque_por_coordenadas_real(unsigned int px, unsigned int py, unsigned pz)
{
	unsigned int x=px / Bloque::W_BLOQUE;
	unsigned int y=py / Bloque::D_BLOQUE;
	unsigned int z=pz / Bloque::H_BLOQUE;
	return obtener_bloque(x, y, z);
}

Bloque& Sala::obtener_bloque_por_indice(unsigned int pi) 
{
	try 
	{
		return *(bloques.at(pi));
	}
	catch(const std::out_of_range& e) 
	{
		LOG<<"ERROR: Solicitado bloque erroneo "<<pi<<" en sala "<<id<<std::endl;
		return bloque_error;
	}
}

void Sala::insertar_actor(Actor * actor, bool persistente)
{
	if(!persistente) actores.push_back(actor);
	else actores_persistentes.push_back(actor);

	Visitante_insercion_sala v(con_turno, entradas, interactuables, 
		controles_direccion, recogibles, escaleras, overlays, 
		teletransportadores);

	actor->aceptar_visitante(v);

	if(v.es_sumar_gema()) ++cuenta_gemas;
}

void Sala::rellenar_vector_plano(std::vector<Representable *>& v, unsigned int plano)
{
	v.clear();
	for(Representable * b : bloques)
	{
		if(b->es_representable() && b->obtener_plano_representable()==plano) 
		{
			v.push_back(b);
		}
	}
}

void Sala::rellenar_vector_representables(std::vector<Representable *>& v)
{
	unsigned int plano=Representable::PL_NORMAL;
	auto insertar=[&v, plano](Representable * b)
	{
		if(b->es_representable() && b->obtener_plano_representable()==plano)
		{ 
			v.push_back(b);
		}
	};

	std::for_each(bloques.begin(), bloques.end(), insertar);
	std::for_each(actores.begin(), actores.end(), [insertar](Representable * r){insertar(r);});
	std::for_each(actores_persistentes.begin(), actores_persistentes.end(), [insertar](Representable * r){insertar(r);});
}

Sala::Sala(unsigned int pw, unsigned int ph, unsigned int pd, unsigned int pid, bool p_cam)
	:w(pw), h(ph), d(pd), id(pid), camara_estatica(p_cam), cuenta_secreto(false),
	cuenta_gemas(0),
	bloque_error(Bloque::X_ERROR, Bloque::Y_ERROR, Bloque::Z_ERROR, 0)
{
	bloque_error.establecer_volumen(0,0,0);
}

Sala::~Sala()
{
	vaciar_actores();	//Se hace delete...
	vaciar_bloques();
}

void Sala::establecer_propiedades(unsigned int pw, unsigned int ph, unsigned int pd, 
	unsigned int pid, unsigned int pcam, unsigned int psec)
{
	w=pw;
	h=ph;
	d=pd;
	id=pid;
	camara_estatica=(bool) pcam;
	cuenta_secreto=(bool) psec;
}

Entrada_sala Sala::obtener_entrada_sala_por_id(unsigned int id)
{
	for(Entrada_sala * e : entradas)
	{
		if(e->acc_id_entrada()==id) 
		{
			return *e; //Devolvería una copia.
		}
	}

	//Si no se encuentra, se da una salida inválida.
	return Entrada_sala(0,0,0,0);
}

/*Recibe los datos persistentes marcados como activables al entrar en la sala.
El resto los deja tal cual.*/

void Sala::actualizar_datos_persistentes(const Controlador_datos_persistentes& c)
{
	std::vector<unsigned int> ids=c.obtener_ids_para_sala(id);

	for(unsigned int id : ids)
	{
		if(c.existe_entrada_por_id(id))
		{
			Dato_persistente datos=c.obtener_por_id(id);
			if(datos.es_aplicable_al_iniciar_sala()) 
			{
				recibir_bloque_datos_persistentes(datos);
			}
		}
	}
}

//Recibe un sólo bloque de datos persistentes.

void Sala::recibir_bloque_datos_persistentes(const Dato_persistente& datos)
{
	if(datos.id_sala!=id || !datos.id_elemento || !datos.tipo_dato)
	{
LOG<<"ERROR: Rechazado bloque de datos persistentes"<<std::endl;
		return;
	}
	else 
	{
		switch(datos.tipo_dato)
		{
			case Dato_persistente::TDP_BLOQUE_COMPORTAMIENTO:

				if(!datos.es_aplicable()) return;

				for(Bloque * bl : bloques)
				{
					if(bl->acc_id()==datos.id_elemento) 
					{
LOG<<"CAMBIANDO COMPORTAMIENTO BLOQUE "<<bl->acc_matriz_x()<<","<<bl->acc_matriz_y()<<","<<bl->acc_matriz_z()<<" CON ID "<<datos.id_elemento<<" A "<<datos.acc_valor_actual()<<std::endl;

						bl->establecer_tipo_comportamiento(datos.acc_valor_actual());
					}
				}				
			break;

			case Dato_persistente::TDP_BLOQUE_REPRESENTACION:

				if(!datos.es_aplicable()) return;

				for(Bloque * bl : bloques)
				{
					if(bl->acc_id()==datos.id_elemento) 
					{
LOG<<"CAMBIANDO ASPECTO BLOQUE "<<datos.id_elemento<<" A "<<datos.acc_valor_actual()<<std::endl;

						bl->establecer_tipo_representacion(datos.acc_valor_actual());
					}
				}
			break;

			case Dato_persistente::TDP_INTERCAMBIAR_BLOQUE:

				if(!datos.es_aplicable()) return;

				for(Bloque * bl : bloques)
				{
					if(bl->acc_id()==datos.id_elemento) 
					{
						if(bl->es_colisionable())
						{
LOG<<"INTERCAMBIANDO BLOQUE OFF"<<std::endl;

							bl->establecer_tipo_representacion(0);
							bl->establecer_tipo_comportamiento(Bloque::TC_LIBRE);
						}
						else
						{
LOG<<"INTERCAMBIANDO BLOQUE "<<datos.id_elemento<<" A "<<datos.acc_valor_actual()<<std::endl;

							bl->establecer_tipo_representacion(datos.acc_valor_actual());
							bl->establecer_tipo_comportamiento(Bloque::TC_SOLIDO);
						}
					}
				}
			break;

			case Dato_persistente::TDP_REACTIVAR_INTERACTUABLE:
				for(Interactuable * i : interactuables)
				{
					if(i->acc_id_localizador()==datos.id_elemento)
					{
LOG<<"REACTIVAR "<<datos.id_elemento<<std::endl;
						i->recibir_senal_interactuable_reactivar();
					}
					
				}
			break;

			case Dato_persistente::TDP_ACTIVAR_ACTIVABLE:
			case Dato_persistente::TDP_DESACTIVAR_ACTIVABLE:
			{
				unsigned int estado=datos.tipo_dato==Dato_persistente::TDP_ACTIVAR_ACTIVABLE ?
					Visitante_activacion::E_ACTIVAR :
					Visitante_activacion::E_DESACTIVAR;
LOG<<"ACTIVAR O DESACTIVAR "<<datos.id_elemento<<std::endl;
				activar_desactivar_por_id(datos.id_elemento, estado);
			}
			break;
		}
	}
}

void Sala::activar_desactivar_por_id(unsigned int pid, unsigned int pestado)
{
	Visitante_activacion v(pestado);
	for(Actor * a : actores_union)
		if(a->acc_id_localizador()==pid)
			a->aceptar_visitante(v);
}

unsigned int Sala::actualizar_borrado_actores()
{	
	unsigned int resultado=0;

	std::vector<Actor *>::iterator	ini=actores.begin(),
					fin=actores.end();

	while(ini < fin)
	{
		Actor* a=*ini;

		if(a->es_marcado_para_borrar())
		{
			Visitante_eliminado_sala v(con_turno, entradas, 
				interactuables, controles_direccion, recogibles, 
				escaleras, overlays, teletransportadores);
			a->aceptar_visitante(v);
//			delete *ini;
			delete a;
			a=NULL;
			ini=actores.erase(ini);
			fin=actores.end();
			++resultado;
		}
		else ++ini;
	}

	//De los persistentes, realmente no borramos.
	ini=actores_persistentes.begin(), fin=actores_persistentes.end();

	while(ini < fin)
	{
		Actor* a=*ini;

		if(a->es_marcado_para_borrar())
		{
			Visitante_eliminado_sala v(con_turno, entradas, 
				interactuables, controles_direccion, 
				recogibles, escaleras, overlays, 
				teletransportadores);

			a->aceptar_visitante(v);
			ini=actores_persistentes.erase(ini);
			fin=actores_persistentes.end();
			++resultado;
		}
		else ++ini;
	}

	if(resultado) unificar_actores();
	
	return resultado;
} 

/*Obtener todos los bloques de la sala que estén en colisión con un cubo.*/

std::vector<Bloque*> Sala::bloques_en_colision_con_cubo(const Cubo& c)
{
	std::vector<Bloque *> resultado;

	//Calculamos el inicio y fin de los rangos según la matriz.

	unsigned int ini_rango_x=floor(c.origen.x / Bloque::W_BLOQUE);
	unsigned int fin_rango_x=floor( (c.origen.x+c.w) / Bloque::W_BLOQUE);

	unsigned int ini_rango_y=floor(c.origen.y / Bloque::D_BLOQUE);
	unsigned int fin_rango_y=floor( (c.origen.y+c.d) / Bloque::D_BLOQUE);

	unsigned int ini_rango_z=floor(c.origen.z / Bloque::H_BLOQUE);
	unsigned int fin_rango_z=floor( (c.origen.z+c.h) / Bloque::H_BLOQUE);

	unsigned int x, y, z;

	//Y ahora es fácil...
	for(x=ini_rango_x; x <= fin_rango_x; ++x)
	{
		for(y=ini_rango_y; y <= fin_rango_y; ++y)
		{
			for(z=ini_rango_z; z <= fin_rango_z; ++z)
			{
				Bloque& bl=obtener_bloque(x, y, z);

				//La comprobación de colisión es necesaria: el
				//algoritmo base añade un bloque de más si los
				//extremos del cubo están alineados con la rejilla.
				
				if(!bl.es_bloque_error() && Cubo::en_colision_con(c, bl.como_cubo()))
				{
					resultado.push_back(&bl);
				}
			}
		}
	}
	
	return resultado;
}

/*Este método unifica los actores propios y ajenos en un mismo vector que
se presentará al mundo. Por cada cambio en estos vectores hay que volverlos a 
unificar.*/

void Sala::unificar_actores()
{
	actores_union.clear();
	actores_union.insert(actores_union.end(), actores.begin(), actores.end());
	actores_union.insert(actores_union.end(), actores_persistentes.begin(), actores_persistentes.end());
}

void Sala::iniciar_salida()
{
	Visitante_salida_sala v;
	unificar_actores();
	for(Actor * a : actores_union) a->aceptar_visitante(v);
}

const Teletransportador * const Sala::obtener_teletransportador_por_id(unsigned int id) const
{
	Teletransportador * t=NULL;

	for(Teletransportador * a : teletransportadores)
	{
		if(a->acc_id_teletransportador() == id)
		{
			t=a;
			break;
		}
	}

	const Teletransportador * const resultado=t;
	return resultado;
}

const Teletransportador * const Sala::obtener_teletransportador_en_colision(const Cubo& c) const
{
	Teletransportador * t=NULL;

	for(Teletransportador * a : teletransportadores)
	{
		Cubo ct=a->como_cubo();

		if(Cubo::en_colision_con(c, ct))
		{
			t=a;
			break;
		}	
	}
	
	const Teletransportador * const resultado=t;
	return resultado;
}
