#ifndef DATOS_DIAPOSITIVAS_H
#define DATOS_DIAPOSITIVAS_H

#include <string>
#include "gestor_mapa_datos_legibles.h"
#include <vector>

/*Conjunto de datos legibles en los pases de diapositivas...
Luego irán los gestores. Se acompaña de ejemplo de archivo de texto.
*/

/*
#id	indice_en_archivo_localizacion
1	1
2	2
3	3
4	4
*/

struct Diapositiva_texto
{
	unsigned int id;
	std::string texto;

	Diapositiva_texto(unsigned int pid, const std::string& ptxt):
		id(pid), texto(ptxt)
	{}
};

/*
#id	x	y
1	0	0
2	100	0
3	200	0
4	0	50
*/

struct Diapositiva_imagen
{
	unsigned int id;
	unsigned int x;
	unsigned int y;

	Diapositiva_imagen() : id(0), x(0), y(0)
	{}

	Diapositiva_imagen(unsigned int pid, unsigned int px, unsigned int py):
		id(pid), x(px), y(py)
	{}
};

/*
#id	id_img	ids_textos...
1	1	1	2	3
2	2	4	5
3	6
4	7
*/

struct Diapositiva
{
	unsigned int id;
	Diapositiva_imagen img;
	std::vector<Diapositiva_texto> textos;

	unsigned int longitud() const {return textos.size();}
	Diapositiva_texto& obtener_texto(unsigned int pindice) {return textos.at(pindice);}

	void asignar_imagen(const Diapositiva_imagen& pimg) {img=pimg;}
	void insertar_texto(const Diapositiva_texto& ptxt) {textos.push_back(ptxt);}

	Diapositiva(unsigned int pid): id(pid)
	{}
};

/*
#id	ids_diapos
1	1	2	3
2	4
*/

struct Pase_diapositivas
{
	unsigned int id;
	std::vector<Diapositiva> diapositivas;

	Diapositiva& obtener_diapositiva(unsigned int pindice) {return diapositivas.at(pindice);}
	unsigned int longitud() const {return diapositivas.size();}

	void insertar_diapositiva(const Diapositiva& pd) {diapositivas.push_back(pd);}

	Pase_diapositivas(unsigned int pid): id(pid)
	{}
};

/*Los gestores...*/

class Gestor_diapositiva_texto:public Gestor_mapa<unsigned int, Diapositiva_texto>
{
};

class Gestor_diapositiva_imagen:public Gestor_mapa<unsigned int, Diapositiva_imagen>
{
};


class Gestor_diapositiva:public Gestor_mapa<unsigned int, Diapositiva>
{
};

class Gestor_pase_diapositivas:public Gestor_mapa<unsigned int, Pase_diapositivas>
{
};

#endif
