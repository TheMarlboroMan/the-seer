#ifndef CONTROL_SESIONES_GUARDADAS_H
#define CONTROL_SESIONES_GUARDADAS_H

#include <fstream>
#include "deserializador.h"

/*Una clase simple para identificar las partidas guardadas.*/

class Control_sesiones_guardadas
{
	public:

	class Info_sesion_guardada
	{
		private:

		unsigned int indice;
		std::string nombre;
		unsigned int segundos_restantes;

		public:

		Info_sesion_guardada(unsigned int i, const std::string& c, unsigned int s)
			:indice(i), nombre(c), segundos_restantes(s) {}

		Info_sesion_guardada(unsigned int i)
			:indice(i), nombre(""), segundos_restantes(0) {}

		bool es_libre() const {return !nombre.size() && !segundos_restantes;}

		unsigned int acc_indice() const {return indice;}

		std::string como_cadena() const 
		{
			return nombre+" - "+Tiempo_juego::como_cadena(segundos_restantes);
		}

		void anular()
		{
			nombre="";
			segundos_restantes=0;
		}
	};

	static const unsigned int MAX_SESIONES=3;

	private:

	std::vector<Info_sesion_guardada> sesiones;

	//Métodos...

	void iniciar();

	public:

	Control_sesiones_guardadas();
	bool es_sesion_usada(unsigned int) const;
	void eliminar_sesion(unsigned int);
	void refrescar();
//	const Info_sesion_guardada& obtener_sesion(unsigned int) const;
	std::vector<Info_sesion_guardada> obtener_sesiones() const {return sesiones;}
};

#endif
