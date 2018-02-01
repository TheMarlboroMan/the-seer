#ifndef CONTROL_HABILIDADES_H
#define CONTROL_HABILIDADES_H

#include <map>

/*Controla las habilidades que se tienen y la que está activada actualmente.
También controla el tiempo de activación de habilidades globales, como parar
el tiempo o el inspector de secretos.*/

class Control_habilidades
{
	public:

	enum habilidades {
		H_SUPER_SALTO=0,
		H_RESISTENCIA_LAVA,
		H_RESISTENCIA_AGUA,
		H_RESISTENCIA_DARDO,
		H_TELETRANSPORTE,
		H_IMPULSO,
		H_PARAR_TIEMPO,
		H_INSPECTOR_SECRETOS,
			H_MAX
	};

	static const unsigned int COSTE_TANQUES_TELETRANSPORTE=1;
	static const unsigned int COSTE_TANQUES_IMPULSO=1;
	static const unsigned int COSTE_TANQUES_SUPER_SALTO=1;
	static const unsigned int COSTE_TANQUES_PARAR_TIEMPO=3;
	static const unsigned int COSTE_TANQUES_INSPECTOR_SECRETOS=1;
 
	static const float CONSUMO_SEGUNDO_RESISTENCIA_LAVA;
	static const float CONSUMO_SEGUNDO_RESISTENCIA_AGUA;
	static const float CONSUMO_SEGUNDO_RESISTENCIA_DARDO;
	static const float SEGUNDOS_UTIL_PARAR_TIEMPO; //Tiempo que dura la habilidad
	static const float SEGUNDOS_UTIL_INSPECTOR_SECRETOS; //Idem.

	private:

	std::map<unsigned int, bool> habilidades;
	bool habilidades_activadas; //En un principio no se puede usar nada.
	unsigned int indice_habilidad;
	float tiempo_parando_tiempo;
	float tiempo_inspeccionando_secretos;
	bool reanudar_sonido; //Indica que acaba de terminar el paro del tiempo.

	void inicializar();
	void cambiar_habilidad(int);

	public:

	unsigned int acc_habilidad_activa() const {return indice_habilidad;}
	bool es_habilidad_usable(unsigned int) const;
	bool es_habilidad_concedida(unsigned int) const;
	bool es_parando_tiempo() const {return tiempo_parando_tiempo > 0.0;}
	bool es_inspeccionando_secretos() const {return tiempo_inspeccionando_secretos > 0.0;}
	bool es_reanudar_sonido() const {return reanudar_sonido;}
	bool es_super_salto() const {return indice_habilidad==H_SUPER_SALTO;}
	bool es_resistencia_lava() const {return indice_habilidad==H_RESISTENCIA_LAVA;}
	bool es_resistencia_agua() const {return indice_habilidad==H_RESISTENCIA_AGUA;}
	bool es_resistencia_dardo() const {return indice_habilidad==H_RESISTENCIA_DARDO;}

	bool es_habilidades_activadas() const {return habilidades_activadas;}
	void activar_habilidades() {habilidades_activadas=true;}
	void desactivar_habilidades() {habilidades_activadas=false;}

	void conceder_habilidad(unsigned int);
	void retirar_habilidad(unsigned int);
	
	void habilidad_siguiente() {cambiar_habilidad(1);}
	void habilidad_anterior() {cambiar_habilidad(-1);}

	void activar_paro_tiempo() {tiempo_parando_tiempo=0.01;}
	void activar_inspector_secretos() {tiempo_inspeccionando_secretos=0.01;}

	void desactivar_todo()
	{
		tiempo_parando_tiempo=0.0;
		tiempo_inspeccionando_secretos=0.0;
		reanudar_sonido=false;
	}

	void turno(float);

	void reiniciar();

	Control_habilidades();
};

#endif
