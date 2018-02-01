#ifndef CONTROL_OBJETIVOS_H
#define CONTROL_OBJETIVOS_H

#include <map>

/*El control de objetivos guarda las cantidades de bonus totales
y pendientes.*/

class Control_objetivos
{
	/****************
	Definiciones
	****************/

	public:

	enum tipos {T_TESORO, T_COFRE, /*T_LLAVE,*/ T_MAX};
	enum mensajes {M_LLAVE, M_TESORO, M_BONUS_TIEMPO, M_MAX};

	/***************
	Implementación
	****************/

	private:

	std::map<unsigned int, unsigned int> totales;
	std::map<unsigned int, unsigned int> contados;
	std::map<unsigned int, bool> mensajes_mostrados;
	
	void preparar();

	/***************
	Interfaz pública
	***************/

	public:

	Control_objetivos();
	~Control_objetivos();

	unsigned int obtener_total(unsigned int pt) const;
	unsigned int obtener_contados(unsigned int pt) const;

	void sumar_total(unsigned int pt, unsigned int pc);
	void contar(unsigned int pt, unsigned int pc);		
	void marcar_mensaje_mostrado(unsigned int);
	void desmarcar_mensaje_mostrado(unsigned int);
	bool es_mensaje_mostrado(unsigned int) const;
	void reiniciar();

};

#endif
