#ifndef CONTROL_SALAS_H
#define CONTROL_SALAS_H

#include <map>
#include <vector>

/*Esta clase lleva la cuenta de qué salas han sido ya descubiertas. Se obtiene
la información desde el cargador de datos de modo que damos por supuesto que
la información está sincronizada.*/

class Control_salas
{
	/**************
	Constantes
	**************/

	public:

	enum tipos {TP_NORMAL, TP_SECRETO};

	/**************
	Implementación
	**************/

	private:

	struct Info_sala
	{
		unsigned int id;
		unsigned int tipo;
		bool descubierta;
		bool con_gemas;

		Info_sala(): id(0), tipo(0), descubierta(false), con_gemas(false) {}
		Info_sala(unsigned int pid, unsigned int pt):
			id(pid), tipo(pt), descubierta(false), con_gemas(false) {}
	};

	//Todas las salas y su info...
	std::map<unsigned int, Info_sala> diccionario;

	//Mapas por tipo: clave es tipo, valor es cantidad.
	std::map<unsigned int, unsigned int> totales;
	std::map<unsigned int, unsigned int> descubiertas;

	/*************
	Interfaz pública.
	**************/

	public:

	Control_salas();
	~Control_salas();

	bool es_con_gemas(unsigned int) const;
	bool marcar_gemas(unsigned int pid, bool v);

	bool descubrir_sala(unsigned int pid);
	bool es_sala_descubierta(unsigned int pid) const;
	bool es_sala_secreta(unsigned int pid) const;

	void insertar_info_sala(unsigned int pid, unsigned int pt);

	unsigned int obtener_total_salas_por_tipo(unsigned int pt) const;
	unsigned int obtener_total_salas_descubiertas_por_tipo(unsigned int pt) const;

	std::vector<unsigned int> obtener_vector_salas_descubiertas() const;
	std::vector<unsigned int> obtener_vector_salas_con_gemas() const;

	void reiniciar();

};

#endif
