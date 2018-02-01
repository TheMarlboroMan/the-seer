#ifndef DATOS_SISTEMA_H
#define DATOS_SISTEMA_H

/*Esta estructura guarda información sobre el estado del sistema
y puede ser compartida por los distintos controladores para indicar
al sistema que el control de juego cede el paso a otro, por ejemplo.*/

class Sistema;

class Datos_sistema
{
	private:

	enum estados{
		E_NADA,
		E_INICIALIZAR,
		E_PREPARANDO_MENU,
		E_PREPARANDO_MENU_INTRO,
		E_MENU,
		E_INICIAR_JUEGO,
		E_JUEGO,
		E_PREPARAR_DIAPOSITIVA,
		E_DIAPOSITIVA,
		E_MAPA,
		E_INFO_JUEGO,
		E_PUZZLE,
		E_CONFIRMACION_SALIDA,
		E_ABORTANDO_EJECUCION,
		E_ABANDONANDO_SISTEMA
			};

	unsigned int estado;
	unsigned int estado_pre_menu;
	unsigned int reconfigurar;

	unsigned int id_idioma;
	unsigned int id_sesion_guardada;

	unsigned int id_diapositiva;
	unsigned int id_info_juego;
	unsigned int id_puzzle;

	bool menu_en_intro;

	void establecer_en_juego() {estado=E_JUEGO;}
	void diapositiva_preparada() {estado=E_DIAPOSITIVA;}
	void establecer_id_sesion_guardado(unsigned int i)
	{
		id_sesion_guardada=i;
	}

	public:

	enum config{
		C_NADA=0,
		C_VIDEO=1,
		C_AUDIO=2,
		C_IDIOMA=4
	};

	Datos_sistema():
		estado(E_INICIALIZAR), estado_pre_menu(E_NADA),
		reconfigurar(C_NADA),
		id_idioma(0), id_sesion_guardada(0),
		id_diapositiva(0), id_info_juego(0),
		id_puzzle(0)
	{};

	void establecer_en_diapositiva(unsigned int pid)
	{
		estado=E_PREPARAR_DIAPOSITIVA;
		id_diapositiva=pid;
	}

	void establecer_en_info_juego(unsigned int pid)
	{
		estado=E_INFO_JUEGO;
		id_info_juego=pid;
	}

	void establecer_en_puzzle(unsigned int pid)
	{
		estado=E_PUZZLE;
		id_puzzle=pid;
	}

	void abortar_ejecucion() {estado=E_ABORTANDO_EJECUCION;}
	void abandonar_sistema() {estado=E_ABANDONANDO_SISTEMA;}
	void confirmar_salida() {estado=E_CONFIRMACION_SALIDA;}
	void establecer_en_mapa() {estado=E_MAPA;}
	void establecer_en_intro() {estado=E_PREPARANDO_MENU_INTRO;}

	void establecer_en_menu(bool v_guardar_estado_anterior)
	{
		if(v_guardar_estado_anterior) estado_pre_menu=estado;
		estado=E_PREPARANDO_MENU;
	}

	void restaurar_juego() {estado=estado_pre_menu;}
	void establecer_en_menu_real() {estado=E_MENU;}
	void iniciar_juego() {estado=E_INICIAR_JUEGO;}
	void establecer_reconfiguracion(unsigned int v) {reconfigurar=v;}

	unsigned int acc_id_idioma() const {return id_idioma;}
	bool es_modo_juego() const {return estado==E_JUEGO;}
	unsigned int acc_id_sesion_guardado() const {return id_sesion_guardada;}

	friend class Sistema;
};

#endif
