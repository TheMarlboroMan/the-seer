#include "pantallazo_sistema.h"

Pantallazo_sistema::Pantallazo_sistema():imagen(NULL), montado(0)
{

}

Pantallazo_sistema::~Pantallazo_sistema()
{
	descartar_imagen();
}

void Pantallazo_sistema::descartar_imagen()
{
	if(imagen)
	{
		delete imagen;
		imagen=NULL;
	}
}

void Pantallazo_sistema::dibujar(DLibV::Pantalla& p) const
{
	DLibV::Representacion_bitmap_estatica bmp;
	bmp.establecer_recurso(imagen);
	bmp.volcar(p);
}

void Pantallazo_sistema::preparar(DLibV::Pantalla& p)
{
	//Generamos la imagen de fondo.

	if(!montado)
	{
		descartar_imagen();

		SDL_Surface * superficie=Herramientas_graficas::pantallazo_alterando_canales(p, -64, -64, -64);

		//En este momento la superficie pasa a ser propiedad de la imagen.
		//Cuando liberemos la imagen, se liberará la superficie.
		//No confundir imagen (recurso_grafico) con bitmap (representacion).
		imagen=new DLibV::Imagen(superficie);
	}

	++montado;
}

void Pantallazo_sistema::desmontar()
{
	--montado;

	if(!montado) descartar_imagen();
}
