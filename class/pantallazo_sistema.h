#ifndef PANTALLAZO_SISTEMA
#define PANTALLAZO_SISTEMA

/*Cuando hay que mostrar cualquier tipo de mensaje usamos esta clase,
dependiente del sistema, para tomar un pantallazo del estado actual y 
mostrarlo como fondo del mensaje.

La hemos creado puesto que en varios controladores se estaba usando la
misma técnica.

El pantallazo se usa para generar el fondo de los pop ups, puzzles y 
diálogo de confirmación. Como el diálogo puede invocarse desde cualquier
otro estado es posible que se monte el pantallazo cuando ya está montado.
Por eso incorporamos un contador de montajes, que actúa como un contador
de referencias. Sólo cuando está realmente sin montar se monta, y sólo
cuando no quedan montajes se desmonta.
*/

#include "herramientas_graficas.h"
#include "../libdan_headers/libDanSDL.h"

class Pantallazo_sistema
{
	private:

	DLibV::Imagen * imagen;  //Aquí meteremos el pantallazo.
	unsigned int montado;

	void descartar_imagen();

	public:

	void dibujar(DLibV::Pantalla&) const;
	void preparar(DLibV::Pantalla&);
	void desmontar();

	bool es_montado() const {return montado;}

	Pantallazo_sistema();
	~Pantallazo_sistema();
};


#endif
