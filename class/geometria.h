#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include "definiciones.h"

struct Vector_3d
{
	float x;
	float y;
	float z;

	Vector_3d(): x(0.f), y(0.f), z(0.f) {}
	Vector_3d(const Vector_3d& o): x(o.x), y(o.y), z(o.z) {}
	Vector_3d(float p_x, float p_y, float p_z):x(p_x), y(p_y), z(p_z) {}

	void reiniciar();
};

/*Un punto en el espacio se almacena con floats, pero se convierte a enteros
cuando estamos haciendo representaciones o sacándolo de algún sitio.*/

struct Punto
{
	float x;
	float y;
	float z;
	Punto(float px, float py, float pz):x(px), y(py), z(pz){}
	Punto(const Punto& p_otro):x(p_otro.x), y(p_otro.y), z(p_otro.z){}
	Punto& operator=(const Punto& p_otro);
};

struct Punto_2d
{
	int x;
	int y;
	Punto_2d(int px, int py):x(px), y(py) {}
	Punto_2d(const Punto_2d& otro):x(otro.x), y(otro.y) {}
	Punto_2d& operator=(const Punto_2d& p_otro);
};

struct Cubo
{
	Punto origen;
	
	//unsigned int w;	//Relación con eje x... Siempre desde el punto de vista de la representación.
	//unsigned int h; //Relación con eje z.
	//unsigned int d;	//Relación con eje y.

	float w;	//Relación con eje x... Siempre desde el punto de vista de la representación.
	float h; //Relación con eje z.
	float d;	//Relación con eje y.
	
	Cubo(const Punto& p, float pw, float ph, float pd)
		:origen(p), w(pw), h(ph), d(pd) {}

	Cubo(unsigned int px, unsigned int py, unsigned int pz, float pw, float ph, float pd)
		:origen(px, py, pz), w(pw), h(ph), d(pd) {}

	Cubo(const Cubo& p_otro):	
		origen(p_otro.origen), w(p_otro.w), h(p_otro.h), d(p_otro.d) {}

	Cubo& operator=(const Cubo& p_otro);

	static bool dentro_de(const Cubo& contenido, const Cubo& contenedor, bool roce_es_colision=false);
	static bool en_colision_con(const Cubo& plantilla, const Cubo& comparado, bool roce_es_colision=false);
	float obtener_correccion_con_cubo(const Cubo& b, unsigned int eje) const;
	static float obtener_correccion_cubos(const Cubo& a, const Cubo& b, unsigned int eje);
	void crecer(float c);
	float posicion_para_eje(unsigned int p_eje) const;
	float dimension_para_eje(unsigned int p_eje) const;
	float final_para_eje(unsigned int p_eje) const;
	static unsigned int posicion_relativa_en_eje(const Cubo& a, const Cubo& b, unsigned int eje);
};

#endif
