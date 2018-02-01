#ifndef COLISIONES_H
#define COLISIONES_H

#include <vector>
#include "geometria.h"
#include "datos_colision.h"
#include "actor.h"
#include "bloque.h"

/*Clase abstracta que lo único que tiene es un porrón de métodos para el 
cálculo de colisiones.*/

class Calculo_colisiones
{
	private:

	Calculo_colisiones() {}
	~Calculo_colisiones() {}

	static Cubo cubo_a_original;	//Idem.
//	static std::vector<Datos_colision> colisiones_static;

	static Datos_colision comprobar_colisiones_cubo_con_actores(
		const Cubo& c, 
		/*const*/ std::vector<Actor*>& actores, 
		std::vector<Actor *>& actores_colision, 
		unsigned int eje=0, 
		float vector=0.0);

	static Datos_colision procesar_colisiones_cubo_con_bloques(
		const Cubo& cubo, 
		/*const*/ std::vector<Bloque *>& bloques, 
		std::vector<Bloque *>& bloques_colision, 
		unsigned int eje=0, 
		float vector=0.0);

	static Datos_colision obtener_datos_colision_con_cubo(
		const Cubo& cubo_actor, 
		const Cubo& cubo, 
		unsigned int eje, 
		float vector, 
		bool desde_fuera=true);

	static float obtener_posicion_corregida_cubo(const Cubo& c1, const Cubo& c2, float cantidad, unsigned int pos, bool desde_fuera=true);

//	static float correccion_z_recibida;
//	static bool habilitar_snap;


	public:
	static Datos_colision obtener_info_colision_completa(Actor& actor, 
			const Cubo& cubo_sala, 
			/*const*/ std::vector<Bloque *>& bloques, 
			/*const*/ std::vector<Actor*>& actores, 
			float v, 
			unsigned int eje, 
//			std::vector<Datos_colision>& colisiones_efecto,
			std::vector<Bloque *>& bloques_colision,
			std::vector<Actor *>& actores_colision);

	static Datos_colision obtener_info_colision_completa_sin_efectos(Actor& actor, 
			const Cubo& cubo_sala, 
			/*const*/ std::vector<Bloque *>& bloques, 
			/*const*/ std::vector<Actor*>& actores, 
			float v, 
			unsigned int eje);

	static void info_colision_completa(const Cubo& c, 
			const Cubo& cubo_sala, 
			/*const*/ std::vector<Bloque *>& bloques, 
			/*const*/ std::vector<Actor*>& actores, 
			float v, 
			unsigned int eje, 
			std::vector<Datos_colision>& colisiones,
			std::vector<Bloque *>& bloques_colision,
			std::vector<Actor *>& actores_colision);

	static bool cubo_en_colision_con_actores(const Cubo& c, const std::vector<Actor*>& v, bool verbose=false);
	static bool cubo_en_colision_con_bloques(const Cubo& c, const std::vector<Bloque*>& v, bool verbose=false);
	static bool cubo_en_colision_con_bordes_cubo(const Cubo& cubo, const Cubo & grande);
	
	static void establecer_cubo_a_original(const Cubo& c) {cubo_a_original=c;}
	static Cubo obtener_cubo_a_original() {return cubo_a_original;}

	//Esto devuelve un array con todas las colisiones generadas por una llamada a info_colision_completa.
//	static std::vector<Datos_colision>& obtener_colisiones_debug() {return colisiones_static;}
};

#endif
