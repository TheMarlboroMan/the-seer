#include "herramientas_calculos.h"
#include <iostream>

/*Devuelve un valor entre 0.0 y 1.0. Lo podemos luego multiplicar
para sacar rangos como 0-255.*/

float Herramientas_calculos::calcular_valor_onda_triangulo(float pt, float pl)
{
	const float pmax=1.0;
	const float max=pmax * 2;
	const float t=fmod(pt, pl);

	float alpha=(t / pl) * max;
	if(alpha > pmax) alpha=pmax-(alpha-pmax);

//	const float TOLERANCIA=0.003;

	//if(alpha < TOLERANCIA) alpha=TOLERANCIA;
	//else if(alpha > max) alpha=max;

	return alpha;
}

float Herramientas_calculos::centrar(float tam, float contenedor)
{
	return (contenedor - tam) / 2.0;
}
