#include "visitante_eliminado_sala.h"

void Visitante_eliminado_sala::eliminar_con_turno(Actor * a)
{
	std::vector<Actor*>::iterator i=std::find(con_turno.begin(), con_turno.end(), a);
	if(i != con_turno.end()) con_turno.erase(i);
}

void Visitante_eliminado_sala::eliminar_entrada(Actor * a)
{
	std::vector<Entrada_sala*>::iterator i=std::find(entradas.begin(), entradas.end(), a);
	if(i != entradas.end()) entradas.erase(i);
}

void Visitante_eliminado_sala::eliminar_interactuable(Interactuable * a)
{
	std::vector<Interactuable*>::iterator i=std::find(interactuables.begin(), interactuables.end(), a);
	if(i != interactuables.end()) interactuables.erase(i);
}

void Visitante_eliminado_sala::eliminar_control_direccion(Actor_control_direccion * a)
{
	std::vector<Actor_control_direccion*>::iterator i=std::find(controles_direccion.begin(), controles_direccion.end(), a);
	if(i != controles_direccion.end()) controles_direccion.erase(i);
}

void Visitante_eliminado_sala::eliminar_recogible(Actor_generico * a)
{
	std::vector<Actor_generico*>::iterator i=std::find(recogibles.begin(), recogibles.end(), a);
	if(i != recogibles.end()) recogibles.erase(i);
}

void Visitante_eliminado_sala::eliminar_escalera(Escalera * a)
{
	std::vector<Escalera*>::iterator i=std::find(escaleras.begin(), escaleras.end(), a);
	if(i != escaleras.end()) escaleras.erase(i);
}

void Visitante_eliminado_sala::eliminar_overlay(Representable_overlay * a)
{
	std::vector<Representable_overlay *>::iterator i=std::find(overlays.begin(), overlays.end(), a);
	if(i != overlays.end()) overlays.erase(i);
}

void Visitante_eliminado_sala::eliminar_teletransportador(Teletransportador * a)
{
	std::vector<Teletransportador *>::iterator i=std::find(teletransportadores.begin(), teletransportadores.end(), a);
	if(i != teletransportadores.end()) teletransportadores.erase(i);
}


Visitante_eliminado_sala::Visitante_eliminado_sala(
		std::vector<Actor *>& t, 
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
	overlays(over), teletransportadores(teles)
{

}

Visitante_eliminado_sala::~Visitante_eliminado_sala()
{
}

void Visitante_eliminado_sala::visitar(Actor_generico& a)
{
	if(a.es_recogible()) eliminar_recogible(&a);
}

void Visitante_eliminado_sala::visitar(Proyectil_obstaculo& a) 
{
	eliminar_con_turno(&a);
}

void Visitante_eliminado_sala::visitar(Entrada_sala& a)
{
	eliminar_entrada(&a);
}

void Visitante_eliminado_sala::visitar(Interruptor_repetible& a)
{
	eliminar_interactuable(&a);
}

void Visitante_eliminado_sala::visitar(Interruptor_no_repetible& a)
{
	eliminar_interactuable(&a);
}

void Visitante_eliminado_sala::visitar(Interruptor_invisible_no_repetible& a)
{
	eliminar_interactuable(&a);
}

void Visitante_eliminado_sala::visitar(Interruptor_invisible_repetible& a)
{
	eliminar_interactuable(&a);
}

void Visitante_eliminado_sala::visitar(Cofre& a)
{
	eliminar_interactuable(&a);
	eliminar_overlay(&a);
}

void Visitante_eliminado_sala::visitar(Actor_control_direccion& a)
{
	eliminar_control_direccion(&a);
}

void Visitante_eliminado_sala::visitar(Disparador_proyectiles& a)
{
	eliminar_con_turno(&a);
}

void Visitante_eliminado_sala::visitar(Alternador_actor_generico& a)
{
	eliminar_con_turno(&a);
}

void Visitante_eliminado_sala::visitar(Actor_audio& a)
{
	eliminar_con_turno(&a);
}

void Visitante_eliminado_sala::visitar(Actor_diapositivas& a)
{
	eliminar_con_turno(&a);
}

void Visitante_eliminado_sala::visitar(Escalera& a)
{
	eliminar_escalera(&a);
}

void Visitante_eliminado_sala::visitar(Interruptor_contacto& a)
{
	eliminar_con_turno(&a);
}

void Visitante_eliminado_sala::visitar(Teletransportador& a)
{
	eliminar_teletransportador(&a);
}

void Visitante_eliminado_sala::visitar(Actor_info_juego& a)
{
	eliminar_con_turno(&a);
}

void Visitante_eliminado_sala::visitar(Actor_puzzle& a)
{
	eliminar_con_turno(&a);
}

void Visitante_eliminado_sala::visitar(Actor_punto_guardado& a)
{
	eliminar_interactuable(&a);
}

void Visitante_eliminado_sala::visitar(Actor_recuperar_tiempo& a)
{
	eliminar_interactuable(&a);
}

void Visitante_eliminado_sala::visitar(Interruptor_auto& a)
{
	eliminar_con_turno(&a);
}
