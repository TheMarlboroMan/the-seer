#ifndef TIEMPO_SISTEMA_H
#define TIEMPO_SISTEMA_H

/* Un contador de tiempo para el sistema en general. LLeva la cuenta
del tiempo pasado en cada estado. No es que sirva para mucho, pero
quiero tenerlo.*/

class Tiempo_sistema
{
	private:

	float tiempo_sistema;
	float tiempo_juego;
	float tiempo_mapa;
	float tiempo_diapositivas;
	float tiempo_puzzle;
	float tiempo_otros;
	float tiempo_intro;

	public:

	enum tipos
		{E_SISTEMA, E_JUEGO, E_MAPA, E_DIAPOSITIVAS,
		E_PUZZLE, E_OTROS, E_INTRO};
		

	void t(float t, unsigned int pt)
	{
		switch(pt)
		{
			case E_SISTEMA: tiempo_sistema+=t; break;
			case E_JUEGO: tiempo_juego+=t; break;
			case E_MAPA: tiempo_mapa+=t; break;
			case E_DIAPOSITIVAS: tiempo_diapositivas+=t; break;
			case E_PUZZLE: tiempo_puzzle+=t; break;
			case E_OTROS: tiempo_otros+=t; break;
			case E_INTRO: tiempo_intro+=t; break;
		}
	}

	float acc_t(unsigned int pt)
	{
		switch(pt)
		{
			case E_SISTEMA: return tiempo_sistema; break;
			case E_JUEGO: return tiempo_juego; break;
			case E_MAPA: return tiempo_mapa; break;
			case E_DIAPOSITIVAS: return tiempo_diapositivas; break;
			case E_PUZZLE: return tiempo_puzzle; break;
			case E_OTROS: return tiempo_otros; break;
		}

		return 0.0;
	}

	Tiempo_sistema():
		tiempo_sistema(0.0),
		tiempo_juego(0.0),
		tiempo_mapa(0.0),
		tiempo_diapositivas(0.0),
		tiempo_puzzle(0.0),
		tiempo_otros(0.0),
		tiempo_intro(0.0)
	{}
};
#endif
