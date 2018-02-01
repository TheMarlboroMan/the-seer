#include "plataforma_flexible.h"

Plataforma_flexible::Plataforma_flexible(int px, int py, int pz, int pw, int ph, int pd, bool pl, bool sl):
	Actor(px, py, pz), w(pw), h(ph), d(pd), letal(pl), solido(sl)
{
	actualizar_volumen();
}

Plataforma_flexible::~Plataforma_flexible()
{

}

void Plataforma_flexible::actualizar_volumen()
{
	establecer_volumen(w, d, h);
}
