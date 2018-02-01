#include "representable.h"

/*Punto_2d Representable::obtener_coordenadas_origen_representacion() const
{
	unsigned int x=obtener_x_representable();
	unsigned int y=obtener_y_representable();
	unsigned int z=obtener_z_representable();

	return obtener_coordenadas_representacion(x, y, z);
}*/

Punto_2d Representable::obtener_coordenadas_representacion(unsigned int x, unsigned int y, unsigned int z)
{
//Oblicua...
	int px=(x - y);
	int py=(x + y)-z;

//Isométrica
//	int px=(x - y);
//	int py=( (x + y) / 2) - z;

	return Punto_2d(px, py);		
}

Representable::Representable():
	debug_ordenacion(false), profundidad_ordenacion(0), 
	visitado_ordenacion(false), rx(0), ry(0), rz(0), rfx(0),
	rfy(0), rfz(0) 
{}

Representable::Representable(const Representable& otro):
	debug_ordenacion(otro.debug_ordenacion), 
	profundidad_ordenacion(otro.profundidad_ordenacion), 
	visitado_ordenacion(otro.visitado_ordenacion), 
	rx(otro.rx), ry(otro.ry), rz(otro.rz), rfx(otro.rfx),
	rfy(otro.rfy), rfz(otro.rfz) 
{}

Representable& Representable::operator=(const Representable& otro)
{
	debug_ordenacion=otro.debug_ordenacion;
	profundidad_ordenacion=otro.profundidad_ordenacion;
	visitado_ordenacion=otro.visitado_ordenacion;
	rx=otro.rx;
	ry=otro.ry;
	rz=otro.rz;
	rfx=otro.rfx;
	rfy=otro.rfy;
	rfz=otro.rfz;
	return *this;
}

void Representable::preparar_representable() 
{
	profundidad_ordenacion=0;
	visitado_ordenacion=0;

	rx=obtener_x_representable();
	ry=obtener_y_representable();
	rz=obtener_z_representable();
	rfx=rx+obtener_w_representable(); 
	rfy=ry+obtener_d_representable();
	rfz=rz+obtener_h_representable();
}

void Ordenador_representables::visitar_nodo(Representable * item)
{
	if(!item->visitado_ordenacion)
	{
		item->visitado_ordenacion=true;
		const unsigned int total=item->otros_por_detras.size();
		unsigned int i=0;

		for(; i<total; i++)
		{
			Representable * temp=item->otros_por_detras[i];
			if(temp==NULL) 
			{
				break;
			}
			else
			{
				visitar_nodo(temp);
				item->otros_por_detras[i]=NULL;
			}
		}
	
		item->profundidad_ordenacion=indice_profundidad++;
	}

}

Ordenador_representables::Ordenador_representables()
	:indice_profundidad(0)
{

}

//Del manual: "The value returned indicates whether the element passed 
//as first argument is considered to go before the second in the 
//specific strict weak ordering it defines.

bool Ordenador_representables::operator()(const Representable* a, const Representable* b) const
{
	return a->profundidad_ordenacion < b->profundidad_ordenacion;
}

//Al contrario que un sort normal, aquí nos aseguramos de comparar todo
//con todo. Al coste de muuchas iteraciones, por supuesto.

void Ordenador_representables::procesar(std::vector<Representable *>& v)
{
	//Comparar cada elemento con todos los otros elementos, 
	//aumentando su nivel de profundidad si está por delante
	//de otros.		

	for(Representable * r : v) r->preparar_representable();

	for(Representable * item : v)
	{
		for(Representable * comparado : v)
		{
			if(item==comparado) continue;

			if(comparado->rx < item->rfx
			&& comparado->ry < item->rfy
			&& comparado->rz < item->rfz)
		        {
				item->otros_por_detras.push_back(comparado);
			}
		}
	}

	for(Representable * r : v) visitar_nodo(r);
	for(Representable * r : v) r->otros_por_detras.clear();

	return;
}
