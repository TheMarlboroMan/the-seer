#include "geometria.h"

void Vector_3d::reiniciar()
{
	x=0.0;
	y=0.0;
	z=0.0;
}

/*Un punto en el espacio se almacena con floats, pero se convierte a enteros
cuando estamos haciendo representaciones o sacándolo de algún sitio.*/

Punto& Punto::operator=(const Punto& p_otro)
{
	x=p_otro.x;
	y=p_otro.y;
	z=p_otro.z;
	return *this;
}


Punto_2d& Punto_2d::operator=(const Punto_2d& p_otro)
{
	x=p_otro.x;
	y=p_otro.y;
	return *this;
}

Cubo& Cubo::operator=(const Cubo& p_otro)
{
	origen=p_otro.origen;
	w=p_otro.w;
	h=p_otro.h;
	d=p_otro.d;
	return *this;
}

bool Cubo::dentro_de(const Cubo& contenido, const Cubo& contenedor, bool roce_es_colision)
{
	bool dentro_x;
	bool dentro_y;
	bool dentro_z;

	if(roce_es_colision)
	{
	        dentro_x=contenido.origen.x >= contenedor.origen.x && contenido.origen.x+contenido.w <= contenedor.origen.x+contenedor.w;
	        dentro_y=contenido.origen.y >= contenedor.origen.y && contenido.origen.y+contenido.d <= contenedor.origen.y+contenedor.d;
	        dentro_z=contenido.origen.z >= contenedor.origen.z && contenido.origen.z+contenido.h <= contenedor.origen.z+contenedor.h;
	}
	else
	{
	        dentro_x=contenido.origen.x > contenedor.origen.x && contenido.origen.x+contenido.w < contenedor.origen.x+contenedor.w;
	        dentro_y=contenido.origen.y > contenedor.origen.y && contenido.origen.y+contenido.d < contenedor.origen.y+contenedor.d;
	        dentro_z=contenido.origen.z > contenedor.origen.z && contenido.origen.z+contenido.h < contenedor.origen.z+contenido.h;
	}

	return dentro_x && dentro_y && dentro_z;
}

bool Cubo::en_colision_con(const Cubo& plantilla, const Cubo& comparado, bool roce_es_colision)
{
	bool dentro_x;
	bool dentro_y;
	bool dentro_z;

	//Para nuestros cálculos, lo pasamos todo a int...

	float cx=comparado.origen.x;
	float cy=comparado.origen.y;
	float cz=comparado.origen.z;
	float cw=comparado.w;
	float cd=comparado.d;
	float ch=comparado.h;

	float px=plantilla.origen.x;
	float py=plantilla.origen.y;
	float pz=plantilla.origen.z;
	float pw=plantilla.w;
	float pd=plantilla.d;
	float ph=plantilla.h;

        //Comparamos si está fuera y lo negamos...
	if(roce_es_colision)
	{
	        dentro_x=!( (cx+cw < px) || (cx > px+pw) );
	        dentro_y=!( (cy+cd < py) || (cy > py+pd) );
	        dentro_z=!( (cz+ch < pz) || (cz > pz+ph) );
	}
	else
	{
	        dentro_x=!( (cx+cw <= px) || (cx >= px+pw) );
	        dentro_y=!( (cy+cd <= py) || (cy >= py+pd) );
	        dentro_z=!( (cz+ch <= pz) || (cz >= pz+ph) );
	}

	return dentro_x && dentro_y && dentro_z;
}

/*Esto nos daría un error terrible siempre que no hubiera superposición...
Suponemos que la hay en todo momento. Lo que hacemos es coger los cuatro
puntos y ordenarlos. Supuestamente restar los dos del centro harán el
trabajo.*/	

float Cubo::obtener_correccion_con_cubo(const Cubo& b, unsigned int eje) const
{
	return obtener_correccion_cubos(*this, b, eje);
}

float Cubo::obtener_correccion_cubos(const Cubo& a, const Cubo& b, unsigned int eje)
{
	std::vector<float> v;

	switch(eje)
	{
		case Definiciones::X:
			v.push_back(a.origen.x);
			v.push_back(a.origen.x+a.w);
			v.push_back(b.origen.x);
			v.push_back(b.origen.x+b.w);
		break;

		case Definiciones::Y:
			v.push_back(a.origen.y);
			v.push_back(a.origen.y+a.d);
			v.push_back(b.origen.y);
			v.push_back(b.origen.y+b.d);
		break;

		case Definiciones::Z:
			v.push_back(a.origen.z);
			v.push_back(a.origen.z+a.h);
			v.push_back(b.origen.z);
			v.push_back(b.origen.z+b.h);
		break;

		default: return 0.0; break;
	}

	std::sort(v.begin(), v.end());
	return v[2]-v[1];
}


void Cubo::crecer(float c)
{
	origen.x-=c;
	origen.y-=c;
	origen.z-=c;
	w+=c*2;
	h+=c*2;
	d+=c*2;
}


float Cubo::posicion_para_eje(unsigned int p_eje) const
{
	switch(p_eje)
	{
		case Definiciones::X: return origen.x; break;
		case Definiciones::Y: return origen.y; break;
		case Definiciones::Z: return origen.z; break;
		default: return 0; break;
	}
}

float Cubo::dimension_para_eje(unsigned int p_eje) const
{
	switch(p_eje)
	{
		case Definiciones::X: return w; break;
		case Definiciones::Y: return d; break;
		case Definiciones::Z: return h; break;
		default: return 0; break;
	}
}

float Cubo::final_para_eje(unsigned int p_eje) const
{
	switch(p_eje)
	{
		case Definiciones::X: return origen.x+w; break;
		case Definiciones::Y: return origen.y+d; break;
		case Definiciones::Z: return origen.z+h; break;
		default: return 0; break;
	}
}

/*Se lee como "el cubo a está a ##RESULTADO## del cubo b".*/

unsigned int Cubo::posicion_relativa_en_eje(const Cubo& a, const Cubo& b, unsigned int eje)
{
	switch(eje)
	{
		case Definiciones::X:
			if(a.origen.x + a.w <= b.origen.x) return Definiciones::POS_O;
			else if(a.origen.x >= b.origen.x + b.w) return Definiciones::POS_E;
			else return Definiciones::POS_DENTRO;
		break;

		case Definiciones::Y:
			if(a.origen.y + a.d <= b.origen.y) return Definiciones::POS_N;
			else if(a.origen.y >= b.origen.y + b.d) return Definiciones::POS_S;
			else return Definiciones::POS_DENTRO;
		break;

		case Definiciones::Z:
			if(a.origen.z + a.h <= b.origen.z) return Definiciones::POS_ABA;
			else if(a.origen.z >= b.origen.z + b.h) return Definiciones::POS_ARR;
			else return Definiciones::POS_DENTRO;
		break;

		default: return Definiciones::POS_ERROR; break;
	}
}
