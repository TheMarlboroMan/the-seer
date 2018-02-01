#ifndef DESERIALIZADOR_H
#define DESERIALIZADOR_H

/*Complemento del serializador, toma los datos de una partida guardada
en disco y transforma los elementos del controlador para que se
restaure el estado original.*/

#include <fstream>
#include "definiciones_serializador_deserializador.h"
#include "utilidades_cargadores.h"
#include "info_juego.h"
#include "tiempo_juego.h"
#include "control_salas.h"
#include "control_objetivos.h"
#include "control_puzzle.h"
#include "control_energia.h"
#include "control_habilidades.h"
#include "datos_legibles.h"
#include "control_actores_persistentes.h"
#include "factoria_actores.h"

class Deserializador
{
	private:

	std::ifstream archivo;
	unsigned int version;
	unsigned int id_sala;
	unsigned int id_entrada;
	std::string nombre;	
	bool error;

	bool es_fin_seccion(const std::string&);
	bool comprobar_inicio_seccion(const std::string&, const char, bool=true);
	std::string extraer_cadena_seccion(const std::string&);

	public:

	Deserializador(unsigned int);
	~Deserializador();

	bool es_valido() const;
	bool es_error() const {return error;}

	unsigned int acc_id_sala() const {return id_sala;}
	unsigned int acc_id_entrada() const {return id_entrada;}
	const std::string& acc_nombre() const {return nombre;}

	void iniciar();
	void deserializar(Info_juego&);
	void deserializar(Tiempo_juego&);
	void deserializar(Control_salas&);
	void deserializar(Control_objetivos&);
	void deserializar(Control_puzzle&);
	void deserializar(Control_energia&);
	void deserializar(Control_habilidades&);
	void deserializar(Controlador_datos_persistentes&);
	void deserializar(Control_actores_persistentes&);

	bool localizar_seccion(const char clave);
	void finalizar();
};

#endif
