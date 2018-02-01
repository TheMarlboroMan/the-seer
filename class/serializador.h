#ifndef SERIALIZADOR_H
#define SERIALIZADOR_H

/*Usaremos el serializador para guardar el estado del juego en un archivo y 
luego recuperarlo. No se guardarían todos los datos: por ejemplo, las salas
se guardarían en sus archivos respectivos aunque si que guardaríamos los
actores persistentes y su estado.

En lugar de indicar a cada clase cómo se serializa, el serializador conoce la
interfaz pública de las mismas y cuenta con suficiente información como para
extraer la información y guardarla a disco.

Iría acompañada del "deserializador", que serviría para extraer la información
de un archivo y traspasarla a la clase de turno.

El orden de llamar a los métodos, por supuesto, importa mucho.
*/

#include <fstream>
#include "definiciones_serializador_deserializador.h"
#include "visitante_serializacion.h"
#include "info_juego.h"
#include "tiempo_juego.h"
#include "control_salas.h"
#include "control_objetivos.h"
#include "control_puzzle.h"
#include "control_energia.h"
#include "control_habilidades.h"
#include "datos_legibles.h"
#include "control_actores_persistentes.h"

class Serializador
{
	private:

	static const int VERSION=1;

//	static const std::string SEPARADOR_SECCION;
//	static const std::string SEPARADOR_SECCION;

	std::ofstream archivo;

	void nueva_seccion();
	
	public:

	bool es_valido() const;

	Serializador(unsigned int);
	~Serializador();

	static void realizar_copia_seguridad(unsigned int);
	void iniciar(unsigned int, unsigned int, const std::string&);
	void serializar(const Info_juego&);
	void serializar(const Tiempo_juego&);
	void serializar(const Control_salas&);
	void serializar(const Control_objetivos&);
	void serializar(const Control_puzzle&);
	void serializar(const Control_energia&);
	void serializar(const Control_habilidades&);
	void serializar(const Controlador_datos_persistentes&);
	void serializar(const Control_actores_persistentes&); 
	

	void finalizar();
};

#endif
