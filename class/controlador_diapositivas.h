#ifndef CONTROLADOR_DIAPOSITIVAS_H
#define CONTROLADOR_DIAPOSITIVAS_H

#include "input.h"
#include "localizador.h"
#include "datos_sistema.h"
#include "cargador_diapositivas.h"
#include "herramientas_graficas.h"
#include "herramientas_calculos.h"
#include "../libdan_headers/libDanSDL.h"

/*Toma el control mientras se pasan diapositivas...*/

class Controlador_diapositivas
{
	private:

	static const int W_IMG=300;
	static const int H_IMG=100;
	static const int X_IMG=0;
	static const int Y_IMG=50;

	static const int W_TXT=400;
	static const int H_TXT=100;
	static const int X_TXT=100;
	static const int Y_TXT=300;

	static const int W_MARCO_IMG=608;
	static const int H_MARCO_IMG=208;
	static const int X_MARCO_IMG=-4;
	static const int Y_MARCO_IMG=46;

	static const int X_TXT_SALTAR=8;
	static const int Y_TXT_SALTAR=8;

	static const int X_TXT_CONTINUAR=400;
	static const int Y_TXT_CONTINUAR=380;

	static const int INDICE_IMG=6;
	static const int INDICE_CADENA_SALTAR=1;
	static const int INDICE_CADENA_SIGUIENTE=2;

	static const float T_FADE;
	static const float T_PARPADEO_SIGUIENTE;
	static const std::string RUTA_LOCALIZACION;

	//Referencias prestadas.
	Datos_sistema& datos_sistema;
	DLibV::Pantalla &pantalla;
	const Input& input;
	//DLibI::Controles_SDL &controles_sdl;
//	DLibH::Controlador_fps_SDL &control_frames;
	const int W_PANTALLA;
	const int H_PANTALLA;

	//Esta será la imagen que se esté mostrando.
	DLibV::Imagen * recurso_ampliado;
	DLibV::Representacion_bitmap_estatica rep; //(400, 200);

	//Esto será el cuadrado que usaremos para los fade.
	DLibV::Primitiva_grafica_caja_estatica caja_trans; //=obtener_caja_transparencia();

	Localizador loc_textos;

	enum estados {E_APARECER, E_ESPERA, E_DESAPARECER, E_MAX, E_FIN};
	unsigned int estado;
	unsigned int diapositiva_actual;
	unsigned int indice_texto_actual;
	unsigned int total_diapositivas;
	unsigned int total_textos;

	std::string cadena_actual;

	float tiempo;
	float tiempo_siguiente;
	
	//DLibV::Representacion_bitmap_estatica preparar_actual(Pase_diapositivas&);
	//DLibV::Primitiva_grafica_caja_estatica obtener_caja_transparencia();
	DLibV::Representacion_bitmap_estatica preparar_actual(Pase_diapositivas&);
	void dibujar(float, Pase_diapositivas&); //, const std::string&, DLibV::Representacion_pixeles_estatica& rep, DLibV::Primitiva_grafica_caja_estatica&);
	void siguiente();
	void controlar_transparencia(); //DLibV::Primitiva_grafica_caja_estatica&);
	std::string obtener_texto_actual(Pase_diapositivas&);
	void calcular_opacidad_letrero_siguiente(DLibV::Representacion&);
	void limpiar_recurso_ampliado();

	public:

	Controlador_diapositivas(Datos_sistema&, DLibV::Pantalla&, 
	const Input&,
	/*DLibI::Controles_SDL&,*/ /*DLibH::Controlador_fps_SDL&,*/ int, int);

	~Controlador_diapositivas();

	Pase_diapositivas obtener_y_preparar_pase(unsigned int);
	bool loop(float, Pase_diapositivas&);
	void reinicializar(Pase_diapositivas&);
};
#endif
