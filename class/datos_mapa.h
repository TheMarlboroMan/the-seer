#ifndef DATOS_MAPA_H
#define DATOS_MAPA_H

#include <string>
#include "gestor_mapa_datos_legibles.h"
#include <vector>

struct Mapa_celda
{
	public:

	enum muro_flags{
		M_PARED_N=1,	//1
		M_PARED_S=2,	//2
		M_PARED_E=4,	//3
		M_PARED_O=8,	//4
		M_PARED_NE=16,	//5
		M_PARED_SE=32,	//6
		M_PARED_NO=64,	//7
		M_PARED_SO=128	//8
	};
	
	enum puerta_flags{
		P_PUERTA_N=1,
		P_PUERTA_S=2,
		P_PUERTA_E=4,
		P_PUERTA_O=8,
		P_PUERTA_ZONA_N=16,
		P_PUERTA_ZONA_S=32,
		P_PUERTA_ZONA_E=64,	
		P_PUERTA_ZONA_O=128
	};

	enum iconos_flags{
		I_COFRE=1,
		I_FLECHA=2,
		I_PORTAL=4,
		I_PUNTO_GUARDADO=8,
		I_PUNTO_TIEMPO=16
	};

	static const unsigned int MAX_FLAG=256;

	//Offsets en cantidad de flags desde el inicio...
	static const unsigned int OFFSET_MUROS=0;
	static const unsigned int OFFSET_PUERTAS=8;
	static const unsigned int OFFSET_ICONOS=16;

	private:

	enum old_migracion_flags{
	T_NADA=0,
		T_PARED_N=1,	//1
		T_PARED_S=2,	//2
		T_PARED_E=4,	//3
		T_PARED_O=8,	//4
		T_PARED_NE=16,	//5
		T_PARED_SE=32,	//6
		T_PARED_NO=64,	//7
		T_PARED_SO=128,	//8
		T_PUERTA_N=256,	//9
		T_PUERTA_S=512,	//10
		T_PUERTA_E=1024,//11
		T_PUERTA_O=2048,//12
		T_COFRE=4096,	//13
		T_FLECHA=8192,	//14
		T_PORTAL=16384,	//15
		T_PUNTO_GUARDADO=32768, //16
	T_MAX=65536
	};


	unsigned short int x;
	unsigned short int y;
	unsigned short int flags_muro;
	unsigned short int flags_puerta;
	unsigned short int flags_iconos;

	public:

	Mapa_celda(unsigned short int px, unsigned short int py, 
		unsigned int pfa, unsigned int pfb, unsigned int pfc):
		x(px), y(py), 
		flags_muro(pfa), 
		flags_puerta(pfb), 
		flags_iconos(pfc)
	{}

	unsigned short int acc_x() const {return x;}
	unsigned short int acc_y() const {return y;}
	unsigned short int acc_flags_muro() const {return flags_muro;}
	unsigned short int acc_flags_puerta() const {return flags_puerta;}
	unsigned short int acc_flags_iconos() const {return flags_iconos;}

	void mut_x(unsigned short int v) {x=v;}
	void mut_y(unsigned short int v) {y=v;}
	void mut_flags_muro(unsigned short int v) {flags_muro=v;}
	void mut_flags_puerta(unsigned short int v) {flags_puerta=v;}
	void mut_flags_iconos(unsigned short int v) {flags_iconos=v;}

	static Mapa_celda migrar_desde_flags_unicas(unsigned int x, unsigned int y, unsigned int flags)
	{
		unsigned short int fl_muro=0, fl_puerta=0, fl_iconos=0;

		if(flags & T_PARED_N) fl_muro|=M_PARED_N;
		if(flags & T_PARED_S) fl_muro|=M_PARED_S;
		if(flags & T_PARED_E) fl_muro|=M_PARED_E;
		if(flags & T_PARED_O) fl_muro|=M_PARED_O;
		if(flags & T_PARED_NE) fl_muro|=M_PARED_NE;
		if(flags & T_PARED_SE) fl_muro|=M_PARED_SE;
		if(flags & T_PARED_NO) fl_muro|=M_PARED_NO;
		if(flags & T_PARED_SO) fl_muro|=M_PARED_SO;
		if(flags & T_PUERTA_N) fl_puerta|=P_PUERTA_N;
		if(flags & T_PUERTA_S) fl_puerta|=P_PUERTA_S;
		if(flags & T_PUERTA_E) fl_puerta|=P_PUERTA_E;
		if(flags & T_PUERTA_O) fl_puerta|=P_PUERTA_O;
		if(flags & T_COFRE) fl_iconos|=I_COFRE;
		if(flags & T_FLECHA) fl_iconos|=I_FLECHA;
		if(flags & T_PORTAL) fl_iconos|=I_PORTAL;
		if(flags & T_PUNTO_GUARDADO) fl_iconos|=I_PUNTO_GUARDADO;
	
		return Mapa_celda(x, y, fl_muro, fl_puerta, fl_iconos);
	}
};

class Mapa_sala
{
	private:

	std::vector<Mapa_celda> celdas;
	unsigned int id_sala;
	unsigned short int id_zona;
	unsigned short int x;
	unsigned short int y;
	unsigned short int z;

	public:

	void insertar_celda(const Mapa_celda& c) {celdas.push_back(c);}
	void mut_x(unsigned short int v) {x=v;}
	void mut_y(unsigned short int v) {y=v;}
	void mut_z(unsigned short int v) {z=v;}
	void mut_id_zona(unsigned short int v) {id_zona=v;}
	void mut_id_sala(unsigned int v) {id_sala=v;}

	unsigned short int acc_x() const {return x;}
	unsigned short int acc_y() const {return y;}
	unsigned short int acc_z() const {return z;}
	unsigned short int acc_id_zona() const {return id_zona;}
	unsigned int acc_id_sala() const {return id_sala;}
	unsigned int total_celdas() const {return celdas.size();}	
	const Mapa_celda& obtener_celda(unsigned int i) const {return celdas.at(i);}
	Mapa_celda& obtener_celda_para_modificar(unsigned int i) {return celdas.at(i);}
	std::vector<Mapa_celda>& obtener_celdas() {return celdas;}

	void eliminar_celda(unsigned int i) {celdas.erase(celdas.begin()+i);}
	void vaciar() {celdas.clear();}

	Mapa_sala(unsigned int ids, unsigned short int idz,
		unsigned short int px, unsigned short int py, unsigned short int pz):
			id_sala(ids), id_zona(idz), x(px), y(py), z(pz)
	{}
};

class Gestor_mapa_juego:public Gestor_mapa<unsigned int, Mapa_sala>
{
};

#endif
