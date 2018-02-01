#include "representador_tiempo_restante.h"

/*Una caja negra con borde blanco. Dentro el tiempo, que se pasa ya como
texto. Si queremos trabajar algunas cosas que cambien según pase el tiempo
tendremos que incluir algunas dependencias más.*/

void Representador_tiempo_restante::generar_contador_tiempo(DLibV::Pantalla& pantalla, const std::string& texto)
{
	DLibV::Primitiva_grafica_caja_estatica borde(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA, Y_CAJA, W_CAJA, H_CAJA),
		DLibV::Gestor_color::color(255, 255, 255));

	const int DBORDE=BORDE_CAJA * 2;

	DLibV::Primitiva_grafica_caja_estatica fondo(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA+BORDE_CAJA, Y_CAJA+BORDE_CAJA, W_CAJA-DBORDE, H_CAJA-DBORDE),
		DLibV::Gestor_color::color(0, 0, 0));

	DLibV::Representacion_texto_fija_estatica txt(DLibV::Gestor_recursos_graficos::obtener(3), texto, W_CAJA, H_CAJA);
	txt.establecer_posicion(X_TEXTO, Y_TEXTO);
	txt.mut_interletra(1);

	borde.volcar(pantalla);	
	fondo.volcar(pantalla);	
	txt.volcar(pantalla);	
}
