#ifndef BLOQUE_H
#define BLOQUE_H

#include "representable.h"
#include "espaciable.h"
#include "visitante.h"
#include "datos_legibles.h"
#include "datos_colision.h"
#include "temporizador_animaciones.h"

/*El bloque ocupa una celda completa de la Sala. Es inamovible de posición pero
podría cambiar de representación o de estado. 

La información de la representación se almacena en el controlador de datos
de representación de bloque, para poder añadir más información sin tener que
compilar.

*/

class Bloque:public Representable, public Espaciable
{
	private:

	static Controlador_datos_representacion_bloque controlador_datos_representacion;

	struct Info_rampa
	{
		const unsigned int eje_rampa;
		const unsigned int eje_canto;
		const unsigned int pos_rampa_solido;
		const unsigned int pos_rampa_canto;

		Info_rampa(const unsigned int a, const unsigned int b,
			const unsigned int c, const unsigned int d):
			eje_rampa(a), eje_canto(b), pos_rampa_solido(c),
			pos_rampa_canto(d){}

		static const Info_rampa obtener_info_rampa(unsigned int t)
		{
			switch(t)
			{
				case TC_RAMPA_O_E: 
				case TC_RAMPA_NO_ESCALABLE_O_E: 
					return Info_rampa(Definiciones::X, Definiciones::Y, Definiciones::POS_E, Definiciones::POS_O); break;
				case TC_RAMPA_E_O: 
				case TC_RAMPA_NO_ESCALABLE_E_O: 
					return Info_rampa(Definiciones::X, Definiciones::Y, Definiciones::POS_O, Definiciones::POS_E); break;
				case TC_RAMPA_S_N: 
				case TC_RAMPA_NO_ESCALABLE_S_N: 
					return Info_rampa(Definiciones::Y, Definiciones::X, Definiciones::POS_N, Definiciones::POS_S); break;
				case TC_RAMPA_N_S:
				case TC_RAMPA_NO_ESCALABLE_N_S:  
					return Info_rampa(Definiciones::Y, Definiciones::X, Definiciones::POS_S, Definiciones::POS_N); break;
				default: return Info_rampa(0,0,0,0); break;
			}
		}	
	};

	public:

	static const unsigned int X_ERROR=999;
	static const unsigned int Y_ERROR=999;
	static const unsigned int Z_ERROR=999;

	enum tipos_comportamiento {
		TC_LIBRE=0, 
		TC_SOLIDO=1,
		TC_LETAL=2,
		TC_SOLIDO_RAMPA=3, //Bloques que no son sólidos si estamos en rampa.
		TC_LETAL_LAVA=4, //Letal, elemental de lava.
		TC_LETAL_AGUA=5, //Letal, elemental de agua.
		TC_RAMPA_O_E=10, //Rampas escalables: esto es, con tocar el canto te subes en ella.
		TC_RAMPA_E_O=11,
		TC_RAMPA_S_N=12,
		TC_RAMPA_N_S=13,
		TC_RAMPA_NO_ESCALABLE_O_E=14, //Rampas no escalables: hay que tocar el canto a como mucho 2 px de diferencia.
		TC_RAMPA_NO_ESCALABLE_E_O=15,
		TC_RAMPA_NO_ESCALABLE_S_N=16,
		TC_RAMPA_NO_ESCALABLE_N_S=17
};

	//Esto serían las medidas reales de cada bloque en una matriz:
	//cada bloque serían 16 unidades cúbicas.
	static const int W_BLOQUE=16;
	static const int H_BLOQUE=16;
	static const int D_BLOQUE=16;

	private:	

	unsigned int id;
	unsigned int matriz_x;
	unsigned int matriz_y;
	unsigned int matriz_z;
	unsigned short int plano_representacion; //Define el vector de representación de profundidad en el que irá...

	unsigned int tipo_comportamiento;
	unsigned int tipo_representacion;
	bool debug_bloque;
	Punto_2d coordenadas_origen; //Se cachean aquí porque nunca cambian.

	//////////////////////////////
	// Métodos propios...

	private:

	void calcular_solidez_rampa(Datos_colision& datos_colision) const;
	/*float*/ int calcular_z_rampa(const Cubo& c, bool=false) const;
	const Info_rampa obtener_info_rampa() const; 

	public:

	Bloque(int px, int py, int pz, unsigned int ptc=0, unsigned int ptr=0);
	virtual ~Bloque();
	bool operator==(const Bloque& bloque) const;
	//OJO CON ESTO: No está para ordenarlo con la representación ni nada, es para poder hacer uniques con él.
//	bool operator<(const Bloque& bloque) const;

	void establecer_plano_representacion(unsigned int pl) {plano_representacion=pl;}
	void establecer_tipo_comportamiento(unsigned int pt) {tipo_comportamiento=pt;}
	void establecer_tipo_representacion(unsigned int pt) {tipo_representacion=pt;}
	void establecer_id(unsigned int v){id=v;}

	unsigned int acc_matriz_x() const {return matriz_x;}
	unsigned int acc_matriz_y() const {return matriz_y;}
	unsigned int acc_matriz_z() const {return matriz_z;}
	unsigned int acc_tipo_representacion() const {return tipo_representacion;}
	unsigned int acc_tipo_comportamiento() const {return tipo_comportamiento;}
	unsigned int acc_id() const {return id;}
	bool es_en_coordenadas(const unsigned int px, const unsigned int py, const unsigned int pz) const {return matriz_x==px && matriz_y==py && matriz_z==pz;}
//	void cuentame() const {std::cout<<"SOY UN BLOQUE";}
	bool es_bloque_error() const {return es_en_coordenadas(X_ERROR, Y_ERROR, Z_ERROR);}

	//Indica que es letal directamente con sólo tocarlo, da igual
	//desde donde se aproxime el jugador.

	bool es_letal() const {return 
		tipo_comportamiento==TC_LETAL
		|| tipo_comportamiento==TC_LETAL_LAVA
		|| tipo_comportamiento==TC_LETAL_AGUA;}

	bool es_letal_general() const {return tipo_comportamiento==TC_LETAL;}
	bool es_letal_lava() const {return tipo_comportamiento==TC_LETAL_LAVA;}
	bool es_letal_agua() const {return tipo_comportamiento==TC_LETAL_AGUA;}

	//Indica si el bloque debe ser tomado como sólido al colisionar con él para
	//calcular la Z encima o para ver si un cubo colisiona con él.

	bool es_completo() const {return 
		tipo_comportamiento==TC_SOLIDO ||
		tipo_comportamiento==TC_SOLIDO_RAMPA ||
		tipo_comportamiento==TC_LETAL ||
		tipo_comportamiento==TC_LETAL_LAVA ||
		tipo_comportamiento==TC_LETAL_AGUA;}

	void static asignar_datos_representacion(Controlador_datos_representacion_bloque v) {controlador_datos_representacion=v;}
	
//	bool es_debug_bloque() const {return debug_bloque;}
	void establecer_debug_bloque(bool v) {debug_bloque=v;}

	int calcular_z_encima(const Cubo& c) const;
	bool cubo_libre_en_rampa(const Cubo&c, bool=false) const;

	bool es_tipo_rampa() const 
	{
		return tipo_comportamiento >= TC_RAMPA_O_E 
		&& tipo_comportamiento <= TC_RAMPA_NO_ESCALABLE_N_S;
	}

	bool es_rampa_escalable() const 
	{
		return tipo_comportamiento >= TC_RAMPA_O_E 
		&& tipo_comportamiento <= TC_RAMPA_N_S;
	}

	bool es_solido_rampa() const {return tipo_comportamiento==TC_SOLIDO_RAMPA;}

	virtual void aceptar_visitante(Visitante& v){v.visitar(*this);};	

	/////////////////////////////////
	// Implementando representable...

	public:

	unsigned int obtener_plano_representable() const {return plano_representacion;}
	unsigned int obtener_x_representable() const {return acc_x();}
	unsigned int obtener_y_representable() const {return acc_y();}
	unsigned int obtener_z_representable() const {return acc_z();}
	unsigned int obtener_w_representable() const {return acc_w();}
	unsigned int obtener_d_representable() const {return acc_d();}
	unsigned int obtener_h_representable() const {return acc_h();}
	bool es_representable() const {return debug_bloque || tipo_representacion;}
	virtual std::string string_me_about_yourself() const {return "BLOQUE "+
		DLibH::Herramientas::entero_a_cadena(matriz_x)+", "+
		DLibH::Herramientas::entero_a_cadena(matriz_y)+", "+
		DLibH::Herramientas::entero_a_cadena(matriz_z)+" t:"+
		DLibH::Herramientas::entero_a_cadena(tipo_comportamiento);}
	virtual void transformar_representacion(Bloque_transformacion_representable& br);
	bool rep_final_es_dentro_de_caja(int dx, int dy, const Bloque_transformacion_representable& bt, const SDL_Rect& caja) const;


	///////////////////////////////
	// Implementando espaciable...

	//Sólo son colisionables aquellos que no sean "Vacío". Todos son sólidos,
	//aunque eso no vale para nada si no son colisionables.

	virtual bool es_colisionable() const {return tipo_comportamiento!=TC_LIBRE;}
	virtual bool es_solido() const {return true;}	
	virtual bool es_recibe_sombra() const {return true;}
	virtual void rellenar_info_colision(Datos_colision& datos_colision) const;
};

#endif
