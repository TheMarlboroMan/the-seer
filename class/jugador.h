#ifndef JUGADOR_H
#define JUGADOR_H

#include "actor_movil.h"
#include "visitante.h"

class Jugador:public Actor_movil
{
	////////////////////////////
	// Propio.

	private:

	enum estados {E_TIERRA, E_AIRE, 
			E_SUBIENDO_ESCALERA, E_ESCALERA, E_BAJANDO_ESCALERA, 
			E_PAUSA_CAIDA, 
			E_PERDIENDO_VIDA, E_FIN_PERDER_VIDA,
			E_IMPULSO, E_FIN_IMPULSO};

	static const int W=8;
	static const int D=8;
	static const int H=16;
	static const int DISTANCIA_INTERACCION=4;
	static const int SNAP_CUBO=16;
	static const int MAX_SNAP_Z=4;
	static const int VECTOR_SALTO=140;
	static const int VECTOR_SUPER_SALTO=220;
	static const int VECTOR_GRAVEDAD=-400;
	static const int H_DESCENSO_ESCALERA=2;

	static const int W_REP=16;
	static const int H_REP_PARADO=27;
	static const int H_REP_SALTANDO=27;
	static const int H_REP_CAIDA_0=21;
	static const int H_REP_CAIDA_1=24;
	static const int H_REP_CAIDA_2=26;
	static const int H_REP_PERDER_VIDA=26;
	static const int H_REP_IMPULSO=26;
	static const int H_REP_ESCALERA=27;

	static const int X_REP_N=32;
	static const int X_REP_S=0;
	static const int X_REP_E=16;
	static const int X_REP_O=48;

	static const int Y_REP_PARADO=0;
	static const int Y_REP_SALTANDO_SUBIR=135;
	static const int Y_REP_SALTANDO_BAJAR=162;
	static const int Y_REP_CAIDA_0=189;
	static const int Y_REP_CAIDA_1=210;
	static const int Y_REP_CAIDA_2=234;
	static const int Y_REP_PERDER_VIDA=260;
	static const int Y_REP_IMPULSO=286;
	static const int Y_REP_ESCALERA=312;

	static const float T_DURACION_ANIMACION_CAMINAR;
	static const float T_DURACION_PASO_CAMINAR;

	static const float T_DURACION_CAIDA_0;
	static const float T_DURACION_CAIDA_1;

	static const float MIN_VELOCIDAD;
	static const float MAX_VELOCIDAD_CAMINAR;
	static const float MAX_VELOCIDAD_CORRER;
	static const float ACELERACION;
	static const float TIEMPO_PAUSA_CAIDA;
	static const float TIEMPO_PERDER_VIDA;
	static const float TIEMPO_SUBIR_BAJAR_ESCALERA;

	unsigned int estado;
	unsigned int direccion;
	unsigned int direccion_aire; //Cuando saltas se almacena la dirección y la velocidad máxima.
	float max_velocidad_aire;
//	bool en_aire;
	bool corriendo;
	float tiempo_animacion;
	float tiempo_turno;

	float obtener_vector_movimiento_contrario(unsigned int eje);
	
	void establecer_estado(unsigned int v);

	public:

	Jugador(int px, int py, int pz);
	~Jugador();

	bool puede_saltar() const {return estado==E_TIERRA || estado==E_ESCALERA;}
	bool puede_super_saltar() const {return puede_saltar() && estado!=E_ESCALERA;}
	bool es_en_aire() const {return estado==E_AIRE;}
	bool es_en_tierra() const {return estado==E_TIERRA;}
	bool es_en_pausa_caida() const {return estado==E_PAUSA_CAIDA;}
	bool es_perdiendo_vida() const {return estado==E_PERDIENDO_VIDA;}
	bool es_finalizada_animacion_perder_vida() const {return estado==E_FIN_PERDER_VIDA;}
	bool es_habilidad_impulso() const {return estado==E_IMPULSO;}
	bool es_fin_habilidad_impulso() const {return estado==E_FIN_IMPULSO;}

	//Implica si debe comprobar colisiones con el mundo.
	bool es_comprueba_colisiones() const {return 
		estado==E_TIERRA || estado==E_AIRE || 
		estado==E_ESCALERA || estado==E_IMPULSO ||
		estado==E_FIN_IMPULSO ||
		estado==E_PAUSA_CAIDA;}

	//Implica si puede recibir input del usuario o no.

	bool es_controlable() const {return estado==E_TIERRA || estado==E_AIRE || estado==E_ESCALERA;}
	bool es_en_escalera() const {return estado==E_ESCALERA;}
	bool es_estado_corre_tiempo() const {return estado!=E_PERDIENDO_VIDA;}

	//Realmente sólo lo usamos en un momento muy determinado, para ver si hay bajada de rampa o desaparece la tierra bajo los pies.
	bool es_afectado_por_gravedad() const {return 
		estado==E_TIERRA ||
		estado==E_AIRE ||
		estado==E_PAUSA_CAIDA ||
		estado==E_FIN_IMPULSO;}
	
	void establecer_subir_escalera() {establecer_estado(E_SUBIENDO_ESCALERA);}
	void establecer_bajar_escalera() {establecer_estado(E_BAJANDO_ESCALERA);}
	void establecer_en_aire() {establecer_estado(E_AIRE);}
	void establecer_en_tierra() {establecer_estado(E_TIERRA);}
	void establecer_pausa_caida() {establecer_estado(E_PAUSA_CAIDA);}
	void establecer_perdiendo_vida() {establecer_estado(E_PERDIENDO_VIDA);}
	void establecer_en_caida();

	void saltar();
	void super_saltar();
	void activar_habilidad_impulso(float);
	void establecer_corriendo(bool v) {corriendo=v;}
	unsigned int acc_direccion() const {return direccion;}
	void girar_180() {direccion=Definiciones::obtener_posicion_opuesta(direccion);}
	bool recibir_snap(int cantidad);
	Cubo obtener_cubo_descenso_escalera() const;
	Cubo obtener_cubo_snap() const;
	Cubo obtener_cubo_interaccion() const;

	///////////////////////////////
	// Espaciable

	public:

	virtual void actualizar_volumen(){establecer_volumen(W, D, H);}
	//No hace nada porque no va a colisionar consigo mismo.
	virtual bool es_recibe_sombra() const {return false;}

	//////////////////////////////
	// Representable

	public:

	virtual std::string string_me_about_yourself() const {return "Jugador";}
	virtual void transformar_representacion(Bloque_transformacion_representable& br);

	//Fix para la representación.
	unsigned int obtener_h_representable() const {return acc_h();}

	/////////////////////////////
	// Actor movil

	protected:

	//virtual float vector_limite(unsigned int flags);

	public:

	//virtual bool recibir_datos_colision(const Datos_colision& datos_colision);
	void procesar_turno(float delta);
	void recibir_input_movimiento(float v, float delta, unsigned int eje);
	void activar_movimiento(float p_delta, unsigned int flags=Definiciones::X | Definiciones::Y | Definiciones::Z);
	void decelerar_vector(float valor, unsigned int flags=0);

	/////////////////////////////
	// Actor...
};
#endif
