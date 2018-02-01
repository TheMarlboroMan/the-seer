#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "input.h"
#include "cargador_datos.h"
#include "sala.h"
#include "serializador.h"
#include "deserializador.h"
#include "jugador.h"
#include "sombra.h"
#include "control_temporizador.h"
#include "entrada_salida_sala.h"
#include "datos_colision.h"
#include "calculo_colisiones.h"
#include "representador.h"
#include "representador_tiempo_restante.h"
#include "audio.h"
#include "visitante_interaccion.h"
#include "visitante_turno.h"
#include "visitante_colision.h"
#include "localizador.h"
#include "control_salas.h"
#include "control_objetivos.h"
#include "control_puzzle.h"
#include "control_energia.h"
#include "control_habilidades.h"
#include "control_actores_persistentes.h"
#include "tiempo_juego.h"
#include "datos_sistema.h"
#include "info_juego.h"
#include "diccionario_info_juego.h"

#include <vector>

#include "utils/comprobador_rampas.h"

class Controlador
{
	private:

	struct Debug_bloques
	{
		unsigned int x, y, z;
		bool mostrar_plano;
		Debug_bloques(): x(0), y(0), z(0), mostrar_plano(false) {};
	};

	/*En lugar de tener varias variables sueltas con los valores que se 
	usan con el input del usuario, lo metemos todo aquí.*/

	struct Info_controles
	{
		static const float T_HABILIDAD_PULSADA_CAMBIO;
		int x, y;
		float t_habilidad_pulsada;
		void reset_ejes() {x=0; y=0;}
		Info_controles():x(0), y(0), t_habilidad_pulsada(0.0) {}
		bool es_modo_cambio() const {return t_habilidad_pulsada >= T_HABILIDAD_PULSADA_CAMBIO;}
	};

	/*En esta estructura se meten los sonidos antes de ser empujados a la
	cola de sonidos. La tenemos ahí porque es posible, llegado el momento,
	retirar un sonido de la cola (principalmente cuando caemos sobre algo
	letal, que no haga el sonido de tocar tierra).

	Esto identifica el sonido en si, no la fuente del mismo. Un modo 
	alternativo sería organizarlos por fuentes de modo que se pueda silenciar
	una fuente completa.*/

	class Sonidos_activos
	{
		public:

		struct Datos
		{
			unsigned int id_sonido;
			unsigned int volumen;
			bool activo;
			Datos(unsigned int i):
				id_sonido(i), volumen(100), activo(false){}
		};

		private:

		std::vector<Datos> activos;

		void inicializar() 
		{
			unsigned int i=Audio::S_MIN;
			while(i < Audio::S_MAX)
			{
				activos.push_back(Datos(i));
				++i;
			};
		}

		public:

		void reiniciar() {for(Datos& d : activos) d.activo=false;}

		void cambiar(unsigned int r, unsigned int v, bool a) 
		{
			try 
			{
				Datos& d=activos.at(r);
				d.volumen=v;
				d.activo=a;
			}
			catch(std::out_of_range &e) {}
		}

		const std::vector<Sonidos_activos::Datos>& obtener_datos() const {return activos;}

		Sonidos_activos() {inicializar();}
	};

	static const int X_CAJA_MARGEN_SCROLL=200;
	static const int Y_CAJA_MARGEN_SCROLL=160;
	static const int W_CAJA_MARGEN_SCROLL=200;
	static const int H_CAJA_MARGEN_SCROLL=100;
	static const float T_HABILIDAD_PULSADA_CAMBIO;	//Indica la cantidad de tiempo (en segundos) que hay que mantener la tecla de habilidad pulsada para entrar en modo de cambio de habilidad.

	//Cosas del motor. Estas son referencias prestadas.
	Datos_sistema& datos_sistema;
	DLibV::Pantalla &pantalla;
	/*const*/ Input& input; //TODO...
	const int W_PANTALLA;
	const int H_PANTALLA;

	DLibV::Camara camara;	//La cámara se usará para no dibujar más entidades que las que estén en pantalla.

	SDL_Rect caja_margen_camara;
	Localizador loc_nombres_salas;
	DLibH::t_cadena txt_nombre_sala;	

	//Controles de juego que guardan info fija.
	Cargador_datos cargador_datos;
	Controlador_cadenas_acciones controlador_cadenas_acciones;
	Controlador_datos_temporizador controlador_datos_temporizador;
	
	//Controles de juego que guardan info variable.
	Tiempo_juego tiempo_juego;
	Controlador_datos_persistentes controlador_datos_persistentes;
	Control_actores_persistentes control_actores_persistentes;
	Control_salas control_salas;
	Control_objetivos control_objetivos;
	Control_puzzle control_puzzle;
	Control_energia control_energia;
	Control_habilidades control_habilidades;

	Info_controles info_controles;
	Jugador jugador;
	Sombra sombra;
	//std::vector<Sala *> salas;
	std::vector<Control_temporizador> controles_temporizador;
	Sala * sala_actual;
	Entrada_sala entrada_actual;
	Escalera escalera_actual;

	/*Al contrario que en otros casos, vamos a guardar el actor que controla
	el puzzle actual: el puzzle cambia de estado pero al finalizarse debe
	volver al controlador de juego y desactivar este actor de forma que el
	puzzle deje de activarse. En lugar de hacer visitantes para esto nos es
	más sencillo incluir este requisito particular en el controlador.*/

	Actor_puzzle * actor_puzzle_actual;
	Info_juego info_juego;
	Debug_bloques dbl;
	bool sala_actual_con_secreto;
	Sonidos_activos sonidos_activos;

	//Información del estado.
	enum estado {E_JUEGO=1, E_DEBUG_BLOQUES=2};

	unsigned int estado;
	bool pausa;
	bool modo_paso;
	bool modo_dios;

	//Información del representador.
	enum dibujar_planos {DP_FONDO=1, DP_MEDIO=2, DP_FRENTE=4};

	unsigned int dibujar_planos;
	int offset_x;
	int offset_y;
	std::vector<Representable *> vector_plano_fondo;
	std::vector<Representable *> vector_plano_frente;

	std::vector<Bloque*> bloques_colisiones_con_efecto_en_turno;
	std::vector<Actor*> actores_colisiones_con_efecto_en_turno;
	
	//void destruir_salas();
	void calcular_offsets_por_actor(const Actor& a);
	void calcular_offsets_para_sala(const Sala& sala);
	void dibujar_modo_juego();
	bool cubo_en_posicion_libre(const Cubo& c, bool verbose=false);
	void activar_interacciones_sala();
	void preparar_vectores_representacion_estaticos(Sala &s);
	void procesar_sonidos_activos();

	void intentar_descenso_escalera();
	void bajar_jugador_escalera();
	void soltar_jugador_escalera();
	void vincular_escalera(const Escalera& e);
	void trepar_jugador_escalera();

	void procesar_input_usuario(float delta);
	void procesar_controles_temporizador(float delta);
	void procesar_turno_jugador(int x, int y, float delta);
	void procesar_turno_jugador_tierra_aire(int x, int y, float delta);
	void procesar_turno_jugador_escalera(int x, int y, float delta);
	void obtener_posicion_sombra();
	void procesar_colisiones_turno(float, const Vector_3d&);
	void ejecutar_cadena_acciones(unsigned int pid_eslabon/*, bool encendido*/);
	void iniciar_temporizador(const Dato_persistente& dp/*, bool encendido*/);
	bool iniciar_salida_sala(unsigned int id_sala, unsigned int id_entrada);
	Sala * obtener_sala_por_id(unsigned int pid);
	void recuento_secretos_sala();

	void cargar_informacion_general();
	void cargar_informacion_juego();
	void recargar_sala_actual();
	void recargar_mundo();
	bool guardar_sesion_juego(unsigned int);
	void iniciar_proceso_guardado(unsigned int);
	
	void mostrar_mensaje_info_juego(unsigned int);
	void insertar_fundido_pantalla(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, float);
	void iniciar_puzzle(unsigned int);

	void conceder_amuleto();
	void conceder_tanque_energia(unsigned int);
	void conceder_habilidad(unsigned int);
	void abrir_cofre(unsigned int);
	void recoger_tesoro(unsigned int pc);
	void recoger_llaves(unsigned int pc);
	void recoger_bonus_tiempo(unsigned int pc);
	void redimir_bonus_tiempo();

	void perder_vida();
	void finalizar_perder_vida();
	void iniciar_sala(Sala& s);
	void actualizar_estado_gemas_sala_actual();
	
	void forzar_pausa_debug();
	void loop_juego(float);
	void loop_debug_bloques(Debug_bloques&);
	bool calcular_snap();
	void pausar_despausar();

	void controlar_accion_gravedad_en_jugador();
	void evaluar_colision_jugador_con_suelo(float vz_orig);
	void calcular_estado_fin_impulso_jugador();

	void activar_habilidad_actual();
	void activar_habilidad_teletransportador();
	void finalizar_habilidad_parar_tiempo(bool);
	bool evaluar_resistencia(float, unsigned int);

	public:

	Controlador(Datos_sistema&, DLibV::Pantalla&, 
	//const Input&,
//TODO: HACER CONST ANTES DE FINALIZAR!!!!
	Input&,
	int, int);
	~Controlador();

	//Estos son para que el sistema pueda obtener información...

	const Control_salas& acc_control_salas() {return control_salas;}
	const Info_juego& acc_info_juego() {return info_juego;}
	const Control_objetivos& acc_control_objetivos() {return control_objetivos;}
	Tiempo_juego& acc_tiempo_juego() {return tiempo_juego;}
	Control_habilidades& acc_control_habilidades() {return control_habilidades;}
	unsigned int acc_segundos_restantes() const {return tiempo_juego.acc_segundos_restantes();}
	unsigned int obtener_id_sala_actual() const {return sala_actual->acc_id();}
	std::vector<unsigned int> obtener_piezas_recogidas() const {return control_puzzle.obtener_piezas_recogidas();}

	//Estos son acciones que el sistema puede hacer...

	void iniciar_juego(unsigned int, unsigned int);
	bool recuperar_sesion_juego();


	bool inicializar_informacion();
	void loop(float);
	void solucionar_puzzle(bool);

	void iniciar_comprobador_rampas();
	void iniciar_comprobador_sintaxis();

	
	private:
	
	Controlador& operator=(const Controlador&);	
	Controlador(const Controlador&);
};

#endif
