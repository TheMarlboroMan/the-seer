#include "controlador_mapa.h"

const std::string Controlador_mapa::RUTA_NOMBRE_ZONAS="data/localizacion/zonas";

//Para cuando esto se compile igual no hay Gestor_color o la pantalla no está inicializada. 
//Inicializamos los valores dentro.
Uint32 Controlador_mapa::COLOR_NORMAL=0; //DLibV::Gestor_color::color(200, 200, 200);
Uint32 Controlador_mapa::COLOR_ACTUAL=0; //DLibV::Gestor_color::color(32, 200, 32);
Uint32 Controlador_mapa::COLOR_SECRETO=0; //DLibV::Gestor_color::color(200, 32, 32);
Uint32 Controlador_mapa::COLOR_NORMAL_SIN_GEMAS=0;
Uint32 Controlador_mapa::COLOR_ACTUAL_SIN_GEMAS=0;
Uint32 Controlador_mapa::COLOR_SECRETO_SIN_GEMAS=0;
Uint32 Controlador_mapa::COLOR_FONDO_MAPA=0; //DLibV::Gestor_color::color(200, 32, 32);
Uint32 Controlador_mapa::COLOR_SEPARADOR=0; //DLibV::Gestor_color::color(200, 32, 32);

Controlador_mapa::Controlador_mapa(
	Datos_sistema& pds,
	const Control_salas& pcs):
	datos_sistema(pds), 
	control_salas(pcs), 
	loc(RUTA_NOMBRE_ZONAS),
	id_sala_actual(0), id_zona_actual(0), 
	max_x(0), min_x(0), max_y(0), min_y(0),
	offset_x(0), offset_y(0)
{
	cargador_mapa.inicializar();
	loc.inicializar(datos_sistema.acc_id_idioma());

/*
	COLOR_NORMAL=DLibV::Gestor_color::color(64, 64, 64);
	COLOR_ACTUAL=DLibV::Gestor_color::color(255, 195, 32);
	COLOR_SECRETO=DLibV::Gestor_color::color(47, 135, 45);
*/


	COLOR_NORMAL=DLibV::Gestor_color::color(74, 74, 74);
	COLOR_ACTUAL=DLibV::Gestor_color::color(255, 205, 42);
	COLOR_SECRETO=DLibV::Gestor_color::color(57, 145, 55);

	COLOR_NORMAL_SIN_GEMAS=DLibV::Gestor_color::color(32, 32, 32);
	COLOR_ACTUAL_SIN_GEMAS=DLibV::Gestor_color::color(223, 157, 0);
	COLOR_SECRETO_SIN_GEMAS=DLibV::Gestor_color::color(0, 104, 0);

	COLOR_FONDO_MAPA=DLibV::Gestor_color::color(0, 0, 0);
	COLOR_SEPARADOR=DLibV::Gestor_color::color(32, 32, 32);
}

void Controlador_mapa::configurar(unsigned int ids)
{
	id_sala_actual=ids;
	bmp.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(7));

	try
	{
		//Obtener sala actual de cargador mapa.
		const Mapa_sala& ms=cargador_mapa.obtener_sala_por_id(id_sala_actual);
		
		//Extraer id_zona actual.
		id_zona_actual=ms.acc_id_zona();
		preparar_mostrado();
	}
	catch(Excepcion_mapa& e)
	{

	}
}

void Controlador_mapa::loop(float delta)
{

}

void Controlador_mapa::preparar_mostrado()
{
	//Inicializar estos valores...
	max_x=-1;
	min_x=-1;
	max_y=-1;
	min_y=-1;

	//Sacar el mapa del cargador de mapas.
	//Iterar y guardar aquellos que estén
	//en la zona y que estén descubiertos
	//según el control de salas. Vaciar actual.

	mostrando.clear();

	const std::map<unsigned int, Mapa_sala> mapa=cargador_mapa.acc_mapa().acc_mapa();
	std::map<unsigned int, Mapa_sala>::const_iterator ini=mapa.begin();

	while(ini != mapa.end())
	{
		Mapa_sala s=(*ini).second;
	
		//Esto lo hacemos con todas las salas, visibles o no.
		controlar_zonas_visibles(s);

		if(s.acc_id_zona()==id_zona_actual &&
		control_salas.es_sala_descubierta(s.acc_id_sala()))
		{
			mostrando.push_back(s);
			calcular_limites(s);
		}
		++ini;
	}

	calcular_offsets();
}

/*Para cada sala, comprobamos si la zona está en el vector
de zonas visibles.*/

void Controlador_mapa::controlar_zonas_visibles(const Mapa_sala& s)
{
	unsigned int id_zona=s.acc_id_zona();
	
	//En primer lugar, rellenamos si no existe.
	if(!zonas_visibles.count(id_zona)) zonas_visibles[id_zona]=false;
	
	//Y ahora asignamos la visibilidad.
	if(control_salas.es_sala_descubierta(s.acc_id_sala()))
	{
		zonas_visibles[id_zona]=true;
	}
}

/*Cuando se saben los máximos y mínimos en las dos dimensiones
del mapa para un mapa en concreto, podemos centrarlo en el área
que tenemos reservada para ello.*/

void Controlador_mapa::calcular_offsets()
{
	int w_mapa=REJILLA_X * (max_x - min_x);
	int h_mapa=REJILLA_Y * (max_y - min_y);

	offset_x=(W_MAPA - w_mapa) / 2;
	offset_y=(H_MAPA - h_mapa) / 2;
}

/*Se hace en base 1 en lugar de 16, como la rejilla. Luego lo 
adaptaremos...*/

void Controlador_mapa::calcular_limites(const Mapa_sala& s)
{
	int min_x_sala=/*REJILLA_X * */s.acc_x();
	int min_y_sala=/*REJILLA_Y * */s.acc_y();

	int max_x_sala=-1;
	int max_y_sala=-1;

	unsigned int celdas=s.total_celdas();
	unsigned int i=0;

	while(i < celdas)
	{
		const Mapa_celda& mc=s.obtener_celda(i);
		const int xc=mc.acc_x()+min_x_sala;
		const int yc=mc.acc_y()+min_y_sala;

		if(xc > max_x_sala) max_x_sala=xc;
		if(yc > max_y_sala) max_y_sala=yc;
		++i;
	}

	auto comprobar_valores_mayor=[](int &maestro, int comparado)
	{
		if(maestro==-1) maestro=comparado;
		else if(comparado > maestro) maestro=comparado;
	};

	auto comprobar_valores_menor=[](int &maestro, int comparado)
	{
		if(maestro==-1) maestro=comparado;
		else if(comparado < maestro) maestro=comparado;
	};

	comprobar_valores_mayor(max_x, max_x_sala);
	comprobar_valores_menor(min_x, min_x_sala);

	comprobar_valores_mayor(max_y, max_y_sala);
	comprobar_valores_menor(min_y, min_y_sala);
}

void Controlador_mapa::dibujar_nombre_zona(DLibV::Pantalla& pantalla)
{
	std::string nombre=loc.obtener(id_zona_actual);

	DLibV::Representacion_texto_auto_estatica txt(DLibV::Gestor_recursos_graficos::obtener(3), nombre);
	txt.establecer_posicion(X_MAPA+W_SEPARADOR, H_MAPA);
	txt.volcar(pantalla);
}

void Controlador_mapa::dibujar(DLibV::Pantalla& pantalla, float delta)
{
	pantalla.rellenar(COLOR_FONDO_MAPA,
		DLibH::Herramientas_SDL::nuevo_sdl_rect(
			X_MAPA, Y_MAPA, W_MAPA, H_PANTALLA)); //Ojo, no es H_MAPA...

	pantalla.rellenar(COLOR_SEPARADOR,
		DLibH::Herramientas_SDL::nuevo_sdl_rect(
			X_MAPA-W_SEPARADOR, Y_MAPA, W_SEPARADOR, H_PANTALLA));

	unsigned int i=0;
	unsigned int visibles=mostrando.size();

	while(i < visibles)
	{
		dibujar_sala(pantalla, delta, mostrando[i]);
		++i;
	}

	dibujar_nombre_zona(pantalla);

	//pantalla.flipar();
}

void Controlador_mapa::dibujar_sala(DLibV::Pantalla& pantalla, float delta, const Mapa_sala& s)
{
	unsigned int celdas=s.total_celdas();
	unsigned int i=0;

	int desp_x=X_MAPA+(REJILLA_X * s.acc_x())+offset_x-(REJILLA_X*min_x);
	int desp_y=Y_MAPA+(REJILLA_Y * s.acc_y())+offset_y-(REJILLA_Y*min_y);

	unsigned int id_sala=s.acc_id_sala();

	while(i < celdas)
	{
		const Mapa_celda& mc=s.obtener_celda(i);
		dibujar_celda(pantalla, delta, mc, id_sala, desp_x, desp_y);
		++i;
	}
}

void Controlador_mapa::dibujar_celda(DLibV::Pantalla& pantalla, float delta, const Mapa_celda& mc, 
	unsigned int id_sala, unsigned int desp_x, unsigned int desp_y)
{
	bmp.establecer_posicion(
		(mc.acc_x() * REJILLA_X) + desp_x, 
		(mc.acc_y() * REJILLA_Y) + desp_y);

	Uint32 color=0;

	if(control_salas.es_con_gemas(id_sala))
	{	
		color=COLOR_NORMAL;
		if(id_sala==id_sala_actual) color=COLOR_ACTUAL;
		else if(control_salas.es_sala_secreta(id_sala)) color=COLOR_SECRETO;
	}
	else
	{
		color=COLOR_NORMAL_SIN_GEMAS;
		if(id_sala==id_sala_actual) color=COLOR_ACTUAL_SIN_GEMAS;
		else if(control_salas.es_sala_secreta(id_sala)) color=COLOR_SECRETO_SIN_GEMAS;
	}


	DLibV::Primitiva_grafica_caja_estatica c(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(
			(mc.acc_x() * REJILLA_X) + desp_x, 
			(mc.acc_y() * REJILLA_Y) + desp_y,
			REJILLA_X, REJILLA_Y),
		color);

	c.volcar(pantalla);

	dibujar_flags(pantalla, bmp, mc.acc_flags_muro(), Mapa_celda::OFFSET_MUROS);
	dibujar_flags(pantalla, bmp, mc.acc_flags_puerta(), Mapa_celda::OFFSET_PUERTAS);
	dibujar_flags(pantalla, bmp, mc.acc_flags_iconos(), Mapa_celda::OFFSET_ICONOS);
}

void Controlador_mapa::dibujar_flags(DLibV::Pantalla& pantalla, DLibV::Representacion_bitmap_estatica& bmp, unsigned int flags, unsigned int offset)
{
	unsigned short flag=1;
	int i=0;

	while(flag < Mapa_celda::MAX_FLAG)
	{
		if(flags & flag)
		{
			bmp.establecer_recorte((offset+i) * REJILLA_X, 0, REJILLA_X, REJILLA_Y); 
			bmp.volcar(pantalla);
		}

		++i;
		flag=pow(2, i);
	}
}

/*Esto funciona asumiendo que no hay saltos en las zonas y que son siempre
correlativas, empezando desde 0 y subiendo.*/

void Controlador_mapa::escoger_zona(int cambio)
{
	int id_zona=id_zona_actual;
	int nindice=id_zona_actual;
	int max=zonas_visibles.size(); //La primera zona es 0.

	//Iteramos sin parar...
	while(true)
	{
		nindice+=cambio;

		if(nindice==id_zona) break;
		else if(nindice < 0) nindice=max;
		else if(nindice >= max) nindice=0;

		if(zonas_visibles[nindice])
		{
			id_zona_actual=nindice;
			break;
		}
	}

	preparar_mostrado();
}
