#ifndef DATOS_PERSISTENTES_H
#define DATOS_PERSISTENTES_H

#include <iostream>
#include <vector>
#include <cmath>
#include "gestor_mapa_datos_legibles.h"

/*El dato persistente es una colección de información que hace referencia a un
aspecto concreto de un actor, bloque o cosa que esté en una sala en concreto, 
guardando el estado del mismo.

Por ejemplo, puede guardar que el bloque con id 33 de la sala 12 tiene un 
valor de representación de 3. 

Estos bloques se crean con los niveles del juego y se consultan cada vez que
se inicia una sala para actualizar los valores al valor que se guarda. Para
organizarlos existe la clase Controlador_datos_persistentes, que los guarda y
da utilidades para obtenerlos por id de mapa, id del propio dato y cosas así.
*/

/*Estos son los elementos que se pueden almacenar en gestores de mapas...*/

/*Este tiene una particularidad: al entrar en una sala es el dato el que 
indica si debe leerse o no (por ejemplo, un dato que controla un obstáculo que
deja de disparar al usar una palanca no debe dejar de disparar al entrar en 
la sala). Además, si debe leerse y es el del tipo bloque es requisito 
indispensable que se haya actualizado al valor al menos una vez 
(no se especifica un valor inicial, de hecho no hay valor inicial salvo para
los bloques, en el resto de casos se ignora).*/

struct Dato_persistente
{
	private:

	unsigned int id_dato;
	unsigned int valor_actual;	//El valor a pasar.
	bool aplicable;	//Se aplica sólo para los bloques... Sólo se hace aplicable al recibir un valor.

	public:

	enum tipos{
		TDP_NADA=0,
		TDP_BLOQUE_COMPORTAMIENTO=1,	//Cambiar comportamiento de un bloque.
		TDP_BLOQUE_REPRESENTACION=2,	//Cambiar representación de un bloque.
		TDP_INICIAR_TEMPORIZADOR=3,	//Iniciar temporizador.
		TDP_REACTIVAR_INTERACTUABLE=4,	//Reactivar interactuable.
		TDP_ACTIVAR_ACTIVABLE=5,
		TDP_DESACTIVAR_ACTIVABLE=6,
		TDP_INTERCAMBIAR_BLOQUE=7
			};

	unsigned int tipo_dato;		//El tipo de acción.
	unsigned int id_sala;		//La sala a la que se aplica. 0 es "global".
	unsigned int id_elemento;	//El id del elemento a llamar.
	bool aplicable_al_entrar_en_sala;

	Dato_persistente(unsigned int idd, unsigned int pt, unsigned int ps, 
		unsigned int pe, unsigned int papl):
		id_dato(idd), valor_actual(0), aplicable(false), tipo_dato(pt), 
		id_sala(ps), id_elemento(pe), aplicable_al_entrar_en_sala(papl)
	{}

	//El constructor de copia por defecto funcionará correctamente aquí.

	unsigned int acc_valor_actual() const {return valor_actual;}
	unsigned int acc_id_dato() const {return id_dato;}

	void establecer_valor_actual(unsigned int v)
	{
		valor_actual=v;
		aplicable=true;
	}

//	void hacer_aplicable() {aplicable=true;}

	bool es_aplicable() const {return aplicable;}	
	bool es_aplicable_al_iniciar_sala() const {return aplicable_al_entrar_en_sala;}
	bool es_global() const {return id_sala==0;}
	bool es_inicio_temporizador() const {return tipo_dato==TDP_INICIAR_TEMPORIZADOR;}
};

struct Cadena_acciones
{
	unsigned int id_dato_persistente;
	unsigned int valor/*_encendido*/;
//	unsigned int valor_apagado;
	unsigned int siguiente_eslabon;

	Cadena_acciones(unsigned int pid, unsigned int pve, /*unsigned int pva, */unsigned int pse):
		id_dato_persistente(pid), valor/*_encendido*/(pve), /*valor_apagado(pva),*/ siguiente_eslabon(pse)
	{}
};


//Esto es para el actor temporizador... El actor va buscando una de estas, que
//ejecuta la cadena de acciones con "id_cadena" cuando ha pasado el tiempo en
//segundos e inicia la siguiente_entrada. Al ejecutar la cadena se le pasa
//el valor que el interruptor le pasara al actor en si.

struct Datos_temporizador
{
	unsigned int id_cadena;
	float tiempo;
	unsigned int siguiente_entrada;

	Datos_temporizador(unsigned int pidc, unsigned int pt_ms, unsigned int p_s):
		id_cadena(pidc), tiempo(0.0), siguiente_entrada(p_s)
	{
		tiempo=(float) pt_ms / 1000.0;
	}
};

/*Representa a un frame de animación de bloque o actor.*/

struct Frame_representacion
{
	float duracion;
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
	unsigned int correccion_x;
	unsigned int correccion_y;

	Frame_representacion():
		duracion(0.0), x(0), y(0), w(0), h(0), correccion_x(0), correccion_y(0)
	{}

	Frame_representacion(unsigned int pd, unsigned int px, unsigned int py,
		unsigned int pw, unsigned int ph, unsigned int pcx, 
		unsigned int pcy):
		duracion(0.0), x(px), y(py), w(pw), h(ph), correccion_x(pcx), correccion_y(pcy)
	{
		duracion=(float) pd / 1000;
	}
};

/*Información sobre cómo representar un bloque para no tener que compilar una
y otra vez...*/

class Datos_representacion_animada
{
	private:

	std::vector<Frame_representacion> frames;
	unsigned int indice_recurso; //Todos los frames están en el mismo mapa.
	unsigned int total_frames;
	float duracion;	//En segundos... Viene expresada en ms desde fuera.

	public:
	
	Datos_representacion_animada():
		total_frames(0.0), duracion(0.0)
	{}

	void reiniciar()
	{
		total_frames=0;
		duracion=0.0;
		frames.clear();
	}

	void asignar_total_frames(unsigned int v) {total_frames=v;}
	void asignar_duracion(unsigned int v) {duracion=(float) v / 1000.00;}
	void asignar_indice_recurso(unsigned int v) {indice_recurso=v;}
	void insertar_frame(Frame_representacion fr) {frames.push_back(fr);}

	Frame_representacion& obtener_frame_unico() {return frames.at(0);}
	Frame_representacion& obtener_para_tiempo_animacion(float t) 
	{
		float transformado=fmod(t, duracion);
		for(Frame_representacion& fr : frames)
			if(transformado <= fr.duracion) return fr;
		return frames.at(0);
	}

	unsigned int acc_indice_recurso() const {return indice_recurso;}
	bool es_estatico() const {return total_frames==1;}
};

/*Información de cómo representar e interpretar un actor genérico...
Tiene todos los datos de representación animada y los del actor. Es un mal
ejemplo de herencia pública, pero tampoco importa demasiado en este caso.*/

class Datos_actor_generico:public Datos_representacion_animada
{
	private:

	unsigned int w;
	unsigned int h;
	unsigned int d;
	unsigned int flags;
	unsigned int efecto_colision;

	public:
	
	Datos_actor_generico():Datos_representacion_animada(), 
		w(0), h(0), d(0), flags(0), efecto_colision(0)
	{}

	void reiniciar()
	{
		Datos_representacion_animada::reiniciar();
		w=0;
		h=0;
		d=0;
		flags=0;
		efecto_colision=0;
	}

	unsigned int acc_w() const {return w;}
	unsigned int acc_h() const {return d;}
	unsigned int acc_d() const {return h;}
	unsigned int acc_efecto_colision() const {return efecto_colision;}
	unsigned int acc_flags() const {return flags;}

	void asignar_w(unsigned int v) {w=v;}
	void asignar_h(unsigned int v) {h=v;}
	void asignar_d(unsigned int v) {d=v;}
	void asignar_efecto_colision(unsigned int v) {efecto_colision=v;}
	void asignar_flags(unsigned int v) {flags=v;}
};

/*Información acerca de los diferentes patrones de disparo (pausas y 
disparos) que pueden seguir los actores que disparan cosas. Se refiere
a patrones en el tiempo y no en el espacio. 

El info_patron disparo es cada unidad con estado y tiempo. El 
datos_patron_disparo es el que los agrupa con un id. Luego estará el 
controlador.*/

struct Info_patron_disparo
{
	unsigned int estado;
	unsigned int pausa_ms;

	Info_patron_disparo(unsigned int pe, unsigned int pp):
		estado(pe), pausa_ms(pp)
	{} 
};

class Datos_patron_disparo
{
	unsigned int id;
	std::vector<Info_patron_disparo> pasos;

	public:

	Datos_patron_disparo(unsigned int pid):id(pid) {}
	void insertar_paso(Info_patron_disparo i){pasos.push_back(i);}
	unsigned int obtener_total_pasos() const {return pasos.size();}
	const Info_patron_disparo& obtener_paso(unsigned int i) const {return pasos.at(i);}
};

class Datos_proyectil_generico:public Datos_representacion_animada
{
	private:

	unsigned int w;
	unsigned int h;
	unsigned int d;
	unsigned int tipo_letal; //Define el tipo de daño que el proyectil hace, según estará en la clase Obstaculo proyectil.
	bool desaparece_colision;

	public:
	
	Datos_proyectil_generico():Datos_representacion_animada(), 
		w(0), h(0), d(0), tipo_letal(1), desaparece_colision(false)
	{}

	void reiniciar()
	{
		Datos_representacion_animada::reiniciar();
		w=0;
		h=0;
		d=0;
		tipo_letal=1;
		desaparece_colision=false;
	}

	unsigned int acc_w() const {return w;}
	unsigned int acc_h() const {return d;}
	unsigned int acc_d() const {return h;}
	unsigned int acc_tipo_letal() const {return tipo_letal;}
	bool es_desaparece_colision() const {return desaparece_colision;}

	void asignar_w(unsigned int v) {w=v;}
	void asignar_h(unsigned int v) {h=v;}
	void asignar_d(unsigned int v) {d=v;}
	void asignar_tipo_letal(unsigned int v) {tipo_letal=v;}
	void asignar_desaparece_colision(bool v) {desaparece_colision=v;}
};

//Identificador y posición para el recorte de las piezas del puzzle,
//posibilitando extender el sistema sin depender directamente de los
//recursos gráficos. Esto es algo sólo gráfico.

struct Definicion_pieza_puzzle
{
	unsigned int id_pieza;
	unsigned int id_recurso;
	unsigned int recorte_x;
	unsigned int recorte_y;

	Definicion_pieza_puzzle(unsigned int id, unsigned int idr,
	unsigned int x, unsigned int y):
		id_pieza(id), id_recurso(idr), 
		recorte_x(x), recorte_y(y)
	{} 
};

/*Define una pieza colocada en un puzzle, se usa tanto para control
de estado como para guardar las soluciones.*/

struct Pieza_colocada_puzzle
{
	static const int PIEZA_NULA=-1;
	static const int ROTACION_INICIAL=1;

	unsigned int indice;
	int id_pieza;
	unsigned int rotacion;

	Pieza_colocada_puzzle(unsigned int i, unsigned int idp,	unsigned int r):
		indice(i), id_pieza(idp), rotacion(r)
	{}

	Pieza_colocada_puzzle(unsigned int i):
		indice(i), id_pieza(PIEZA_NULA), rotacion(ROTACION_INICIAL)
	{}
};

/*Define un puzzle.*/

class Definicion_puzzle
{
	private:

	unsigned int id;
	unsigned int w;
	unsigned int h;
	unsigned int recurso_img;
	unsigned int x_img;
	unsigned int y_img;
	unsigned int w_img;
	unsigned int h_img;

	std::vector<Pieza_colocada_puzzle> solucion;

	public:

	Definicion_puzzle(unsigned int pid, unsigned int pw, unsigned int ph,
		unsigned int rec, unsigned int pxi, unsigned int pyi, 
		unsigned int pwi, unsigned int phi):
		id(pid), w(pw), h(ph), recurso_img(rec), 
		x_img(pxi), y_img(pyi), w_img(pwi), h_img(phi)
	{}

	Definicion_puzzle():
		id(0), w(0), h(0), recurso_img(0), x_img(0), y_img(0), w_img(0), h_img(0)
	{}

	unsigned int acc_id() const {return id;}
	unsigned int acc_w() const {return w;}
	unsigned int acc_h() const {return h;}
	unsigned int acc_recurso_img() const {return recurso_img;}
	unsigned int acc_x_img() const {return x_img;}
	unsigned int acc_y_img() const {return y_img;}
	unsigned int acc_w_img() const {return w_img;}
	unsigned int acc_h_img() const {return h_img;}

	const std::vector<Pieza_colocada_puzzle>& acc_solucion() const {return solucion;}
	void preparar_solucion() 
	{
		const unsigned int c=w*h;
		unsigned int i=0;
		while(i < c)
		{
			solucion.push_back(Pieza_colocada_puzzle(i));
			++i;
		}
	}

	void asignar_solucion(const Pieza_colocada_puzzle& p) 
	{
		if(p.indice < solucion.size())
		{
			Pieza_colocada_puzzle& pieza=solucion[p.indice];
			pieza.id_pieza=p.id_pieza;
			pieza.rotacion=p.rotacion;
		}
	}

	void reiniciar() 
	{
		id=0;
		w=0;
		h=0;
		recurso_img=0;
		x_img=0;
		y_img=0;
		w_img=0;
		h_img=0;
		solucion.clear();
	}
};

/***/

/*Y estos son los gestores en si...*/

	//Este en particular sería estático para los bloques.

class Controlador_datos_representacion_bloque:public Gestor_mapa<unsigned int, Datos_representacion_animada>
{
	public:

	Controlador_datos_representacion_bloque(){}
	~Controlador_datos_representacion_bloque(){}
};

	//Este para actores genéricos.

class Controlador_datos_actores_genericos:public Gestor_mapa<unsigned int, Datos_actor_generico>
{
	public:

	Controlador_datos_actores_genericos(){}
	~Controlador_datos_actores_genericos(){}
};

class Controlador_datos_persistentes:public Gestor_mapa<unsigned int, Dato_persistente>
{
	public:

	Controlador_datos_persistentes(){}
	~Controlador_datos_persistentes(){}

	void actualizar_valor(unsigned int id, unsigned int valor) {mapa.at(id).establecer_valor_actual(valor);}

	std::vector<unsigned int> obtener_ids_para_sala(unsigned int id_sala) const
	{
		std::vector<unsigned int> resultado;

		for(std::pair<unsigned int, Dato_persistente> par : mapa)
		{
			if(par.second.id_sala==id_sala) resultado.push_back(par.first);
		}

		return resultado;
	}

	std::vector<Dato_persistente> obtener_leer_al_entrar() const
	{
		std::vector<Dato_persistente> resultado;
		
		for(std::pair<unsigned int, Dato_persistente> par : mapa)
		{
			if(par.second.es_aplicable_al_iniciar_sala()) resultado.push_back(par.second);
		}

		return resultado;
	}
};

class Controlador_cadenas_acciones:public Gestor_mapa<unsigned int, Cadena_acciones>
{
	public:

	Controlador_cadenas_acciones() {}
	~Controlador_cadenas_acciones() {}
};

class Controlador_datos_temporizador:public Gestor_mapa<unsigned int, Datos_temporizador>
{
	public:
	Controlador_datos_temporizador() {}
	~Controlador_datos_temporizador() {}
};

class Disparador_proyectiles;
class Alternador_actor_generico;

class Controlador_patrones_disparo:public Gestor_mapa<unsigned int, Datos_patron_disparo>
{
	private:

	//Instancia compartida... Es la que rellenará el controlador.
	//Fuck patrones de diseño.
	static Controlador_patrones_disparo controlador_patrones_disparo;

	public:
	Controlador_patrones_disparo() {}
	~Controlador_patrones_disparo() {}
	void insertar_paso(unsigned int id, Info_patron_disparo i)
	{
		Datos_patron_disparo& d=mapa.at(id);
		d.insertar_paso(i);		
	}

	void static asignar_datos_patrones_disparo(Controlador_patrones_disparo v) {controlador_patrones_disparo=v;}

	friend class Disparador_proyectiles;
	friend class Alternador_actor_generico;


};

class Controlador_datos_proyectiles_genericos:public Gestor_mapa<unsigned int, Datos_proyectil_generico>
{
	public:

	Controlador_datos_proyectiles_genericos(){}
	~Controlador_datos_proyectiles_genericos(){}
};

class Controlador_piezas_puzzle:public Gestor_mapa<unsigned int, Definicion_pieza_puzzle>
{
	public:

	Controlador_piezas_puzzle(){}
	~Controlador_piezas_puzzle(){}
};

class Controlador_definicion_puzzle:public Gestor_mapa<unsigned int, Definicion_puzzle>
{
	public:

	Controlador_definicion_puzzle(){}
	~Controlador_definicion_puzzle(){}
};
#endif
