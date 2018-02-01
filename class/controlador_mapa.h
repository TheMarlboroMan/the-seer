#ifndef CONTROLADOR_MAPA_H
#define CONTROLADOR_MAPA_H

#include "localizador.h"
#include "datos_mapa.h"
#include "datos_sistema.h"
#include "cargador_mapa.h"
#include "control_salas.h"
#include "../libdan_headers/libDanSDL.h"

/*Se encarga del mapa en la pantalla de mapa pero no toma el 
control: es una propiedad del controlador_auxiliar, que 
compagina el mapa con el inventario...*/

class Controlador_mapa
{
	private:

	static const std::string RUTA_NOMBRE_ZONAS;

	static const unsigned int REJILLA_X=16;
	static const unsigned int REJILLA_Y=16;

	static Uint32 COLOR_NORMAL;
	static Uint32 COLOR_ACTUAL;
	static Uint32 COLOR_SECRETO;
	static Uint32 COLOR_NORMAL_SIN_GEMAS;
	static Uint32 COLOR_ACTUAL_SIN_GEMAS;
	static Uint32 COLOR_SECRETO_SIN_GEMAS;
	static Uint32 COLOR_FONDO_MAPA;
	static Uint32 COLOR_SEPARADOR;

	static const int W_MAPA=400;
	static const int H_MAPA=380;
	static const int X_MAPA=200;
	static const int Y_MAPA=0;
	static const int W_SEPARADOR=8;
	static const int H_PANTALLA=400;
	//Referencias prestadas.
	Datos_sistema& datos_sistema;
	const Control_salas& control_salas;

	//Información propia...
	Localizador loc;
	Cargador_mapa cargador_mapa;	//El cargador de información.
	DLibV::Representacion_bitmap_estatica bmp; //Bitmap que siempre usaremos.
	std::map<unsigned int, bool> zonas_visibles;	//Un array de id zonas visibles.
	std::vector<Mapa_sala> mostrando;	//Un array de las salas en la zona actual

	unsigned int id_sala_actual;
	unsigned int id_zona_actual;

	int max_x;
	int min_x;
	int max_y;
	int min_y;

	int offset_x;
	int offset_y;

	private:

	//Métodos...
	
	void preparar_mostrado();
	void dibujar_sala(DLibV::Pantalla&, float, const Mapa_sala&);
	void dibujar_celda(DLibV::Pantalla&, float, const Mapa_celda&, unsigned int, unsigned int, unsigned int);
	void dibujar_flags(DLibV::Pantalla&, DLibV::Representacion_bitmap_estatica&, unsigned int, unsigned int);
	void dibujar_nombre_zona(DLibV::Pantalla&);

	void controlar_zonas_visibles(const Mapa_sala&);
	void calcular_limites(const Mapa_sala&);
	void calcular_offsets();

	public:

	Controlador_mapa(Datos_sistema&, const Control_salas&);
	void escoger_zona(int);
	void configurar(unsigned int);
	void loop(float);
	void dibujar(DLibV::Pantalla&, float);
};

#endif
