#include "visitante_insercion_sala.h"

Visitante_insercion_sala::Visitante_insercion_sala(std::vector<Actor *>& t, 
		std::vector<Entrada_sala *>& e, 
		std::vector<Interactuable *>& i, 
		std::vector<Actor_control_direccion *>& acd,
		std::vector<Actor_generico *>& r,
		std::vector<Escalera *>& esc,
		std::vector<Representable_overlay *>& over,
		std::vector<Teletransportador *>& teles

)
	:Visitante(), con_turno(t), entradas(e), interactuables(i),
	controles_direccion(acd), recogibles(r), escaleras(esc),
	overlays(over), teletransportadores(teles), sumar_gema(false)
{

}

Visitante_insercion_sala::~Visitante_insercion_sala()
{

}

void Visitante_insercion_sala::visitar(Actor_generico& a)
{
	if(a.es_recogible()) recogibles.push_back(&a);
	if(a.es_bonus()) sumar_gema=true;
}

void Visitante_insercion_sala::visitar(Interruptor_repetible& a)
{
	interactuables.push_back(&a);
}

void Visitante_insercion_sala::visitar(Interruptor_no_repetible& a) 
{
	interactuables.push_back(&a);
}

void Visitante_insercion_sala::visitar(Interruptor_invisible_no_repetible& a)
{
	interactuables.push_back(&a);
}

void Visitante_insercion_sala::visitar(Interruptor_invisible_repetible& a)
{
	interactuables.push_back(&a);
}

void Visitante_insercion_sala::visitar(Cofre& a)
{
	interactuables.push_back(&a);
	overlays.push_back(&a);
}

void Visitante_insercion_sala::visitar(Proyectil_obstaculo& a) 
{
	con_turno.push_back(&a);
}

void Visitante_insercion_sala::visitar(Entrada_sala& a) 
{
	entradas.push_back(&a);
}

void Visitante_insercion_sala::visitar(Actor_control_direccion& a)
{	
	controles_direccion.push_back(&a);
}

void Visitante_insercion_sala::visitar(Disparador_proyectiles& a)
{
	con_turno.push_back(&a);
}

void Visitante_insercion_sala::visitar(Alternador_actor_generico& a)
{
	con_turno.push_back(&a);
}

void Visitante_insercion_sala::visitar(Actor_diapositivas& a)
{
	con_turno.push_back(&a);
}

void Visitante_insercion_sala::visitar(Actor_info_juego& a)
{
	con_turno.push_back(&a);
}

void Visitante_insercion_sala::visitar(Actor_audio& a)
{
	con_turno.push_back(&a);
}

void Visitante_insercion_sala::visitar(Escalera& a)
{
	escaleras.push_back(&a);
}

void Visitante_insercion_sala::visitar(Interruptor_contacto& a)
{
	con_turno.push_back(&a);
}

void Visitante_insercion_sala::visitar(Teletransportador& a)
{
	teletransportadores.push_back(&a);
}

void Visitante_insercion_sala::visitar(Actor_puzzle& a)
{
	con_turno.push_back(&a);
}

void Visitante_insercion_sala::visitar(Actor_punto_guardado& a) 
{
	interactuables.push_back(&a);
}

void Visitante_insercion_sala::visitar(Actor_recuperar_tiempo& a) 
{
	interactuables.push_back(&a);
}

void Visitante_insercion_sala::visitar(Interruptor_auto& a)
{
	con_turno.push_back(&a);
	a.marcar_como_no_usado();
}
