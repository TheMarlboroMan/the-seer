#ifndef CONTROLADOR_INFO_JUEGO_H
#define CONTROLADOR_INFO_JUEGO_H

/*Se encarga de mostrar información acerca de los artefactos
que se hayan recogido durante el juego. Es un controlador aparte
del resto aunque lo que haremos será preparar un pantallazo de lo
que se muestra en el juego para simular continuidad.

Los textos que mostrará los sacaremos de su propio archivo de
localización. Aunque esto pueda pisarse un poco con el archivo
del inventario conseguimos poder obtener descripciones distintas 
con otros espaciados.

Para conectar estos textos con el controlador del juego usaremos
una clase independiente de estas dos: diccionario_info_juego,
con las que podremos obtener el indice de lo que vamos a mostrar
y sacar los datos de texto.
*/

#include "diccionario_info_juego.h"
#include "input.h"
#include "localizador.h"
#include "datos_sistema.h"
#include "../libdan_headers/libDanSDL.h"

class Controlador_info_juego
{
	private:

	static const std::string RUTA_LOCALIZACION;

	static const int X_TEXTO=120;
	static const int Y_TEXTO=180;

	static const int X_CAJA=110;
	static const int Y_CAJA=160;
	static const int W_CAJA=380;
	static const int H_CAJA=120;
	static const int BORDE_CAJA=2;

	Datos_sistema& datos_sistema;
	const Input& input;
	Localizador loc;

	DLibV::Primitiva_grafica_caja_estatica  caja_fondo;
	DLibV::Primitiva_grafica_caja_estatica  caja_borde;
	DLibV::Representacion_texto_auto_estatica txt_texto;
	
	bool montado;

	void dibujar(DLibV::Pantalla&); 
	virtual bool procesar_input(const Input&);

	public:

	void preparar(unsigned int);
	void desmontar();
	bool loop(float, DLibV::Pantalla&);
	bool es_montado() const {return montado;}

	Controlador_info_juego(Datos_sistema&, const Input&);
};

#endif
