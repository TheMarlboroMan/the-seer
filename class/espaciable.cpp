#include "espaciable.h"

Espaciable::Espaciable(int px, int py, int pz, int pw, int pd, int ph)
	:posicion(Punto(px, py, pz), pw, pd, ph), cambio_posicion(true)
{
}

Espaciable::Espaciable(const Espaciable& otro)
	:posicion(otro.posicion), cambio_posicion(otro.cambio_posicion)
{
}

Espaciable& Espaciable::operator=(const Espaciable& otro)
{
	posicion=otro.posicion;
	cambio_posicion=otro.cambio_posicion;
	return *this;
}

void Espaciable::sumar_posicion(float px, float py, float pz)
{
	posicion.origen.x+=px;
	posicion.origen.y+=py;
	posicion.origen.z+=pz;

	if(px || py || pz) 
	{
		cambio_posicion=true;
	}
}

void Espaciable::desplazar_posicion(float pv, unsigned int pflags)
{
	if(pflags & Definiciones::X) posicion.origen.x+=pv;
	if(pflags & Definiciones::Y) posicion.origen.y+=pv;
	if(pflags & Definiciones::Z) posicion.origen.z+=pv;

	if(pv && pflags) cambio_posicion=true;
}

void Espaciable::establecer_posicion(float px, float py, float pz)
{
	auto procesar=[this](float val, float &pos)
	{
		if(val >=0 && val!=pos)
		{
			pos=val;
			cambio_posicion=true;			
		}
	};

	procesar(px, posicion.origen.x);
	procesar(py, posicion.origen.y);
	procesar(pz, posicion.origen.z);
}

void Espaciable::establecer_posicion(const Punto& p)
{
	auto procesar=[this](float val, float &pos)
	{
		if(val >=0 && val!=pos)
		{
			pos=val;
			cambio_posicion=true;			
		}
	};

	procesar(p.x, posicion.origen.x);
	procesar(p.y, posicion.origen.y);
	procesar(p.z, posicion.origen.z);
}


void Espaciable::establecer_posicion_por_eje(float pv, unsigned int pflags)
{
	auto procesar=[this](float val, float &pos)
	{
		if(val!=pos)
		{
			pos=val;
			cambio_posicion=true;			
		}
	};

	if(pflags & Definiciones::X) procesar(pv, posicion.origen.x);
	if(pflags & Definiciones::Y) procesar(pv, posicion.origen.y);
	if(pflags & Definiciones::Z) procesar(pv, posicion.origen.z);
}

void Espaciable::establecer_volumen(int pw, int pd, int ph)
{
	if(pw >= 0) posicion.w=pw;
	if(pd >= 0) posicion.d=pd;
	if(ph >= 0) posicion.h=ph;
}

//Después de una colisión podemos llamar a esto para eliminar los 
//decimales que se han podido quedar por el camino y que harían temblar
//las representaciones.

void Espaciable::normalizar_posicion(unsigned int eje)
{
	float x=eje & Definiciones::X ? acc_x() : posicion.origen.x;
	float y=eje & Definiciones::Y ? acc_y() : posicion.origen.y;
	float z=eje & Definiciones::Z ? acc_z() : posicion.origen.z;
	establecer_posicion(x, y, z);
}
