#ifndef DEFINICIONES_H
#define DEFINICIONES_H

/*
Las coordenadas son así:

       [z] 0.0.n
        |
        |
        |
	|
	|
        . 0.0.0
       / \
      /   \
     /     \
    /       \
   /         \
  [y] 0.n.0  [x] n.0.0

Cuando se definen los puntos cardinales lo hacemos así:

*-------------------------------*
|                 [N]           |
|        [O]      /             |
|          -     /              |
|            -  /               |
|              *                |
|             / -               |
|            /   -              |
|           /     [E]           |
|          [S]                  |
*-------------------------------*

Mientras que arriba y abajo es evidente.

La posición que se codifica con "punto" sería el punto más alejado del punto de 
vista, de forma que las dimensiones "crezcan".

Esto lo he pillado por ahí...

screen.x = map.x * TILE_WIDTH_HALF - map.y * TILE_WIDTH_HALF;
screen.y = map.x * TILE_HEIGHT_HALF + map.y * TILE_HEIGHT_HALF;

And with some simplification we get the basic formula for isometric projection:

screen.x = (map.x - map.y) * TILE_WIDTH_HALF;
screen.y = (map.x + map.y) * TILE_HEIGHT_HALF;
*/

struct Definiciones
{
	public:

	enum eje{X=1, Y=2, Z=4};
	enum posicion{POS_ERROR=0, POS_N=1, POS_S=2, POS_E=4, POS_O=8, POS_ARR=16, POS_ABA=32, POS_DENTRO=64}; 	//Esto significa "YO colisiono con mi cara TAL".
	
	static const float MAX_DELTA;

	static float obtener_multiplicador_posicion(unsigned int p)
	{
		switch(p)
		{
			case POS_N: return -1.0; break;
			case POS_S: return 1.0; break;
			case POS_E: return 1.0; break;
			case POS_O: return -1.0; break;
			case POS_ARR: return 1.0; break;
			case POS_ABA: return -1.0; break;
			default: return 0.0; break;
		}
	}

	static unsigned int obtener_posicion_opuesta(unsigned int p)
	{
		switch(p)
		{
			case POS_N: return POS_S; break;
			case POS_S: return POS_N; break;
			case POS_E: return POS_O; break;
			case POS_O: return POS_E; break;
			case POS_ARR: return POS_ABA; break;
			case POS_ABA: return POS_ARR; break;
			default: return 0; break;
		}
	}
	
	static std::string traducir_eje(unsigned int pv)
	{
		switch(pv)
		{
			case X: return "X"; break;
			case Y: return "Y"; break;
			case Z: return "Z"; break;
			default: return "EJE DESCONOCIDO"; break;
		}
	}

	static std::string traducir_posicion(unsigned int pv)
	{
		switch(pv)
		{
			case POS_ERROR: return "ERROR"; break;
			case POS_N: return "NORTE"; break;
			case POS_S: return "SUR"; break;
			case POS_E: return "ESTE"; break;
			case POS_O: return "OESTE"; break;
			case POS_ARR: return "ARRIBA"; break;
			case POS_ABA: return "ABAJO"; break;
			case POS_DENTRO: return "DENTRO"; break;
			default: return "DIRECCION DESCONOCIDA"; break;
		}
	}

	static bool posicion_corresponde_con_eje(unsigned int dir, unsigned int eje)
	{
		switch(eje)
		{
			case X: return dir==POS_E || dir==POS_O; break;
			case Y: return dir==POS_N || dir==POS_S; break;
			case Z: return dir==POS_ARR || dir==POS_ABA; break;
			default: return false; break;
		}
	}

	static unsigned int obtener_eje_para_posicion(unsigned int pos)
	{
		switch(pos)
		{
			case POS_N:
			case POS_S:
				return Y;
			break;

			case POS_E:
			case POS_O:
				return X;
			break;

			case POS_ARR:
			case POS_ABA: 
				return Z; 
			break;

			default: return 0; break;
		}
	}

	private:
	Definiciones() {}

};

//Busca esto en el main.
//const float Definiciones::MAX_DELTA=0.030;

#endif
