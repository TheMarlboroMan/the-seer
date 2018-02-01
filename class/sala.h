#ifndef SALA_H
#define SALA_H

/*La sala en un ancho, alto y profundo relleno con bloques cúbicos formando una
rejilla. Dentro de esta rejilla se mueven los actores.*/

#include <vector>
#include <string>
#include "bloque.h"
#include "actor.h"
#include "interactuable.h"
#include "datos_legibles.h"
#include "actor_control_direccion.h"
#include "entrada_salida_sala.h"
#include "visitante_insercion_sala.h"
#include "visitante_eliminado_sala.h"
#include "visitante_salida_sala.h"

#include "visitante_activacion.h"
#include "../libdan_headers/libDanSDL.h"

class Sala
{
	private:

	Sala& operator=(const Sala&);
	Sala(const Sala&);
		
	unsigned int w; //Eje X
	unsigned int h; //Eje Z.  
	unsigned int d; //Eje Y.
	unsigned int id;
	bool camara_estatica;
	bool cuenta_secreto;

	unsigned int cuenta_gemas; //La sala no lo sabe a priori. Al pasar el visitante de inserción hace un recuento.

	std::string nombre_archivo;

	/*El vector de bloques no representa los bloques efectivos, sino cada
	cuadricula de la sala. Con una sala de 10x10x10 tendremos 1000 bloques.
	No consume demasiada memoria y es muy fácil de manipular así que por una
	vez vamos a hacernos la vida sencilla.*/

	std::vector<Bloque *> bloques;	
	Bloque bloque_error;

	/*En la sala se almacenan también los actores que hay. Permite luego
	pasarlo todo a los métodos de representación de una forma sencilla y 
	también hacer los temas de las colisiones llegado el momento.

	Los actores persistentes NO SON propiedad de la sala, sino del 
	Cargador de datos.

	Para el mundo de fuera, no importa quién tenga qué: se presentan 
	ambos en el vector actores_union.*/

	std::vector<Actor *> actores;	
	std::vector<Actor *> actores_persistentes;	
	std::vector<Actor *> actores_union;
	
	/*Si la sala tiene entradas y salidas mejor guardarlas aparte de los
	actores aunque a la hora de colisionar pudiéramos hacerlas todas juntas.
	Si las guardamos así tenemos un acceso instantáneo a poder decir "de la
	Sala tal dame la entrada con el id cual y ahí colocas al jugador".
	Las podemos guardar también con los actores y dejar referencias sólo
	en un array de salas, que se rellenarían al cargar los actores de la
	sala cuando la factoría diese una de estas.*/

	std::vector<Actor_control_direccion *> controles_direccion;
	std::vector<Entrada_sala *> entradas;
	std::vector<Interactuable *> interactuables;
	std::vector<Actor *> con_turno;
	std::vector<Actor_generico *> recogibles;
	std::vector<Escalera *> escaleras;
	std::vector<Representable_overlay *> overlays;
	std::vector<Teletransportador *> teletransportadores;

	void vaciar_actores();
	void vaciar_bloques();

	public:

	Sala(unsigned int pw, unsigned int ph, unsigned int pd, unsigned int pid, bool p_cam);
	~Sala();

//	void preparar_para_recarga();
	void iniciar_bloques();
	Punto obtener_punto_central() const;
	unsigned int obtener_indice(unsigned int px, unsigned int py, unsigned int pz) const;

	//Ninguno de estos puede ser const porque se pueden usar para cambiar bloques...
	Bloque& obtener_bloque(unsigned int px, unsigned int py, unsigned int pz);
	Bloque& obtener_bloque_por_coordenadas_real(unsigned int px, unsigned int py, unsigned pz);
	Bloque& obtener_bloque_por_indice(unsigned int pi);

	void insertar_actor(Actor * actor, bool persistente=false);
	void rellenar_vector_plano(std::vector<Representable *>& v, unsigned int plano);
	void rellenar_vector_representables(std::vector<Representable *>& v);
	void actualizar_datos_persistentes(const Controlador_datos_persistentes& c);
	void recibir_bloque_datos_persistentes(const Dato_persistente& datos);
	unsigned int actualizar_borrado_actores();
	void poblar_vector_bloques_en_colision_desde_cubo(std::vector<Bloque>& v, const Cubo& c);

	void establecer_propiedades(unsigned int pw, unsigned int ph, unsigned int pd, unsigned int pid, unsigned int pcam, unsigned int psec);
	void establecer_nombre_archivo(const std::string& v) {nombre_archivo=v;}

	Entrada_sala obtener_entrada_sala_por_id(unsigned int id);

	std::vector<Bloque*>& acc_bloques() {return bloques;}
	std::vector<Bloque*> bloques_en_colision_con_cubo(const Cubo&);

	//Devuelve una copia del vector. No debe haber muchas cosas.
	std::vector<Interactuable *>& obtener_interactuables() {return interactuables;}
	std::vector<Actor_control_direccion *>& obtener_controles_direccion() {return controles_direccion;}
	std::vector<Actor *>& obtener_con_turno() {return con_turno;}
	std::vector<Actor *>& obtener_actores() {return actores_union;}
	std::vector<Escalera *>& obtener_escaleras() {return escaleras;}
	std::vector<Representable_overlay *>& obtener_overlays() {return overlays;}

	//Para los teletransportadores...
	const Teletransportador * const obtener_teletransportador_por_id(unsigned int id) const;
	const Teletransportador * const obtener_teletransportador_en_colision(const Cubo&) const;

//	std::vector<Actor *>& obtener_actores_persistentes() {return actores_persistentes;}

	unsigned int obtener_total_bloques() {return bloques.size();}
	unsigned int acc_w() const {return w;}
	unsigned int acc_h() const {return h;}
	unsigned int acc_d() const {return d;}
	unsigned int acc_id() const {return id;}
	bool es_camara_estatica() const {return camara_estatica;}
	bool es_secreta() const {return cuenta_secreto;}
	Entrada_sala obtener_primera_entrada() {return * entradas.at(0);}
	std::string acc_nombre_archivo() const {return nombre_archivo;}

	Cubo como_cubo() const {return Cubo(0, 0, 0, w*Bloque::W_BLOQUE, h*Bloque::H_BLOQUE, d*Bloque::D_BLOQUE);}
	void unificar_actores();
	void iniciar_salida();
	void activar_desactivar_por_id(unsigned int pid, unsigned int pestado);

	unsigned int acc_cuenta_gemas() const {return cuenta_gemas;}
	void recoger_gema() {--cuenta_gemas;}
};

#endif
