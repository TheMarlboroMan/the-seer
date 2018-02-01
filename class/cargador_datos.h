#ifndef CARGADOR_DATOS_H
#define CARGADOR_DATOS_H

#include "utilidades_cargadores.h"
#include "control_actores_persistentes.h"
#include "control_salas.h"
#include "sala.h"
#include "datos_legibles.h"
#include "factoria_actores.h"
#include "control_actores_persistentes.h"

extern DLibH::Log_base LOG;

class Cargador_datos
{
	private:

	static const std::string FICHERO_DATOS;
	std::map<unsigned int, std::string> ids_archivos;
	unsigned int numero_linea;

	std::string nombre_fichero_datos_persistentes;
	std::string nombre_fichero_cadenas_acciones;
	std::string nombre_fichero_datos_temporizadores;
	std::string nombre_fichero_datos_bloques;
	std::string nombre_fichero_salas;
	std::string nombre_fichero_datos_actores;
	std::string nombre_fichero_patrones_disparos;
	std::string nombre_fichero_datos_proyectiles;

	enum estados_lectura{E_PROPIEDADES=1, E_BLOQUES=2, E_ACTORES=4, E_PERSISTENTES=8};

	void interpretar_linea_como_propiedades_sala(const std::string& linea, Sala& sala);
	void interpretar_linea_como_bloques(const std::string& linea, Sala& sala, const std::string& nombre_archivo);
	void interpretar_info_bloques_rangos(const std::string& cadena, unsigned int& ini, unsigned int& fin);
	void interpretar_info_bloques_simple(const std::string& cadena, unsigned int& ini, unsigned int& fin);
	void interpretar_linea_como_actores(const std::string& linea, std::vector<Actor *>&v);//Sala& sala);
	void cargar_sala_desde_archivo(Sala& sala, const std::string& nombre_archivo, unsigned int flags);
	void extraer_actores_persistentes_de_sala(Control_actores_persistentes& cap, Sala& sala, const std::string& nombre_archivo, unsigned int id_sala);
	void vaciar_actores_persistentes();
	void comprobacion_sintaxis_sala(Sala &, const std::string&);

	public:
	
	Cargador_datos();
	~Cargador_datos();

	bool inicializar();
	Controlador_datos_temporizador generar_datos_temporizador();
	Controlador_datos_persistentes generar_datos_persistentes();
	Controlador_cadenas_acciones generar_cadenas_acciones();
	Controlador_datos_representacion_bloque generar_datos_representacion_bloques();
	Controlador_datos_actores_genericos generar_datos_actores_genericos();
	Controlador_datos_proyectiles_genericos generar_datos_proyectiles_genericos();
	Controlador_patrones_disparo generar_datos_patrones_disparo();
	Frame_representacion obtener_frame_de_linea(std::string linea);

	void localizar_archivos_salas();
	void precargar_actores_persistentes(Control_actores_persistentes&);
	void cargar_control_salas(Control_salas&);

	bool cargar_sala_desde_id(Sala& sala, unsigned int id);
	bool existe_sala_por_id(unsigned int id) {return ids_archivos.count(id);}
	void ejecutar_comprobacion_sintaxis_salas();
};

#endif
