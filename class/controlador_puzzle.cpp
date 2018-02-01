#include "controlador_puzzle.h"

extern DLibH::Log_base LOG;
Uint32 Controlador_puzzle::COLOR_TABLERO_ACTUAL=0;
Uint32 Controlador_puzzle::COLOR_TABLERO_NORMAL=0;
Uint32 Controlador_puzzle::COLOR_TABLERO_SELECCIONADA=0;
const float Controlador_puzzle::T_PARPADEO=1.0; 
const float Controlador_puzzle::T_DESTELLO_FINAL=4.0; 
const std::string Controlador_puzzle::RUTA_LOCALIZACION="data/localizacion/puzzles";

/*********** Cargador de puzzles... *******************************************/

const std::string Cargador_puzzle::RUTA_DEFINICIONES="data/info/datos_puzzles.txt";

Pieza_colocada_puzzle Cargador_puzzle::leer_linea_como_info_solucion(const std::string& linea)
{
	const char separador='\t';
	std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);
	if(valores.size()!=3) 
	{
		LOG<<"ERROR: No hay 3 parametros para solucion de puzzle "<<valores.size()<<" en linea "<<linea<<std::endl;
		return Pieza_colocada_puzzle(0,0,0);
	}
	else 
	{
		return Pieza_colocada_puzzle(valores[0], valores[1], valores[2]);
	}
}

void Cargador_puzzle::inicializar()
{
	std::ifstream archivo(RUTA_DEFINICIONES.c_str(), std::ios::in);
	const char separador='\t';
	const std::string delimitador_linea="[F]";

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de datos de puzzles"<<std::endl;
	}
	else
	{
		bool linea_iniciada=false;
		Definicion_puzzle dp;

		while(!archivo.eof())
		{
			std::string linea=Utilidades_cargadores::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				

			if(linea.size())
			{
				//Si es fin de entrada podemos meter 
				if(linea.find(delimitador_linea)!=std::string::npos)
				{
					cdp.insertar(dp.acc_id(), dp);
					linea_iniciada=false;
					dp.reiniciar();
					continue;
				}

				//Si no es línea iniciada es una nueva: los datos de cabecera..
				if(!linea_iniciada)
				{
					std::vector<unsigned int> valores=Utilidades_cargadores::explotar_linea_a_enteros(linea, separador);
					if(valores.size()!=8) LOG<<"ERROR: No hay 8 parametros para cabecera de puzzle en su lugar "<<valores.size()<<" en linea "<<linea<<std::endl;
					else 
					{
						dp=Definicion_puzzle(valores[0], valores[1], 
						valores[2], valores[3], valores[4], 
						valores[5], valores[6], valores[7]);

						dp.preparar_solucion();
						linea_iniciada=true;
					}
				}
				//Si la línea es iniciada y no es fin, sabemos que es un frame.
				else
				{
					dp.asignar_solucion(leer_linea_como_info_solucion(linea));
				}
			}
		}

		archivo.close();
	}

	LOG<<"Cargados "<<cdp.size()<<" puzzles."<<std::endl;
}

Definicion_puzzle Cargador_puzzle::obtener_puzzle_por_id(unsigned int idp) const
{
	if(cdp.existe_entrada_por_id(idp))
	{
		return cdp.obtener_por_id(idp);
	}
	else
	{
		return Definicion_puzzle();
	}
}

/*********** Controlador de puzzles... *******************************************/

Controlador_puzzle::Controlador_puzzle(Datos_sistema& ds, Tiempo_juego& tj):
	datos_sistema(ds), 
	tiempo_juego(tj),
	img_piezas(NULL),
	rep_txt_titulo(
		DLibV::Gestor_recursos_graficos::obtener(3), 
		""),
	rep_txt_ayuda(
		DLibV::Gestor_recursos_graficos::obtener(3), 
		""),
	//bmp_fondo;
	caja_borde(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA, Y_CAJA, W_CAJA, H_CAJA),
		DLibV::Gestor_color::color(255,255,255)),
	caja_interior(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA+BORDE, Y_CAJA+BORDE, W_CAJA-(2*BORDE), H_CAJA-(2*BORDE)),
		DLibV::Gestor_color::color(0,0,0)),
	caja_separador(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_SEPARADOR, Y_CAJA, W_SEPARADOR, H_CAJA),
		DLibV::Gestor_color::color(255,255,255)),
	loc_textos(RUTA_LOCALIZACION),
	montado(false),
	comprobar_finalizacion(false),
	estado(E_MOVER),
	indice_tablero(0),
	puzzle_actual()
{
	COLOR_TABLERO_SELECCIONADA=DLibV::Gestor_color::color(0, 128, 255);
	COLOR_TABLERO_NORMAL=DLibV::Gestor_color::color(255, 255, 255);
	COLOR_TABLERO_ACTUAL=DLibV::Gestor_color::color(255, 64, 64);

	rep_txt_titulo.establecer_posicion(X_TXT_TITULO, Y_TXT_TITULO);
	rep_txt_titulo.mut_interletra(1);

	rep_txt_ayuda.establecer_posicion(X_TEXTO_AYUDA, Y_TEXTO_AYUDA);
	rep_txt_ayuda.mut_interletra(0);

	loc_textos.inicializar(datos_sistema.acc_id_idioma());
}

Controlador_puzzle::~Controlador_puzzle()
{
	descartar_imagen_piezas();
}

bool Controlador_puzzle::procesar_controles(const Input& i)
{
	bool resultado=true;

	if(i.es_senal_salida() || i.es_input_down(Input::I_ESCAPE))
	{
		datos_sistema.establecer_en_menu(true);
	}
	else
	{
		if(i.es_input_down(Input::I_MAPA))
		{
			resultado=false; 
		}
		else if(i.es_input_down(Input::I_SALTAR))
		{
			if(estado==E_MOVER) estado=E_CAMBIAR;
			else if(estado==E_CAMBIAR) estado=E_MOVER;
		}
		//TODO TODO TODO TODO...
		else if(i.es_input_down(Input::I_HABILIDAD))
		{
			const std::vector<Pieza_colocada_puzzle>& solucion=puzzle_actual.acc_solucion();
			const unsigned int max=solucion.size();

			std::cout<<"========="<<puzzle_actual.acc_id()<<"=========="<<std::endl;
	
			unsigned int i=0;
			while(i < max)
			{
				const Pieza_colocada_puzzle& p=solucion[i];
				const Pieza_colocada_puzzle& s=estado_puzzle[i];

				bool posicion=p.id_pieza == s.id_pieza;
				bool rotacion=p.rotacion & s.rotacion;

				std::cout<<i<<" ==> "
<<" "<<p.id_pieza<<" "<<p.rotacion<<"\t"<<
s.id_pieza<<" "<<s.rotacion<<" ==> "<<posicion<<" "<<rotacion<<std::endl;
				++i;
			}
		}
		//TODO TODO TODO TODO...
		else switch(estado)
		{
			case E_MOVER:
				if(i.es_input_down(Input::I_ARRIBA)) seleccionar_pieza_tablero(M_ARR);
				else if(i.es_input_down(Input::I_DERECHA)) seleccionar_pieza_tablero(M_DER);
				else if(i.es_input_down(Input::I_ABAJO)) seleccionar_pieza_tablero(M_ABA);
				else if(i.es_input_down(Input::I_IZQUIERDA)) seleccionar_pieza_tablero(M_IZQ);
			break;

			case E_CAMBIAR:
				if(i.es_input_down(Input::I_ARRIBA)) cambiar_pieza(-1);
				else if(i.es_input_down(Input::I_ABAJO)) cambiar_pieza(1);
				else if(i.es_input_down(Input::I_DERECHA)) rotar_pieza(1);
				else if(i.es_input_down(Input::I_IZQUIERDA)) rotar_pieza(-1);
			break;
		}
	}

	return resultado;
}

/*Cambia el indice_tablero para seleccionar una nueva pieza en función del input
recibido y las comprobaciones de límites de turno.*/

void Controlador_puzzle::seleccionar_pieza_tablero(unsigned int dir)
{
	Punto pt=obtener_coordenadas_de_indice(indice_tablero);

	switch(dir)
	{
		case M_ARR: if(pt.y > 0) indice_tablero-=puzzle_actual.acc_w(); break;
		case M_ABA: if(pt.y < puzzle_actual.acc_h()-1) indice_tablero+=puzzle_actual.acc_w(); break;
		case M_IZQ: if(pt.x > 0) --indice_tablero; break;
		case M_DER: if(pt.x < puzzle_actual.acc_w()-1) ++indice_tablero; break;
	}
}

/*Si dir es mayor que cero rota la pieza actual 90 grados hacia la derecha. En
el caso contrario la rota hacia la izquierda la misma cantidad. Las rotaciones
están en el enum rotacion:

enum rotacion {R_0=1, R_90=2, R_180=4, R_270=8};

Y esos son los únicos valores que puede tener.
*/

void Controlador_puzzle::rotar_pieza(int dir)
{
	Pieza_colocada_puzzle& p=estado_puzzle[indice_tablero];

	if(dir==1)
	{
		p.rotacion*=2; 
		if(p.rotacion > R_270) p.rotacion=R_0;
	}
	else
	{
		if(p.rotacion == R_0) p.rotacion=R_270;
		else p.rotacion/=2;
	}

	//Levantamos la bandera para comprobar si el puzzle se ha finalizado.
	comprobar_finalizacion=true;
}

/*Si dir es mayor que cero se selecciona la siguiente pieza. Si es menor que
cero se selecciona la pieza anterior. 

Más allá de la última pieza y antes de la primera colocamos PIEZA_NULA, que 
tiene un valor de -1.

Ojo, no se comprueba si la pieza existe o no... Mejor hacemos bien los recursos
y punto!.

Si la pieza ya ha sido colocada 
*/

void Controlador_puzzle::cambiar_pieza(int dir)
{
	Pieza_colocada_puzzle& p=estado_puzzle[indice_tablero];
	
	const int MAX_PIEZA=Control_pieza_puzzle::obtener_id_maximo();
	const int MIN_PIEZA=-1;

	//Al seleccionar la pieza nula se anula la rotación.
	if(p.id_pieza==MIN_PIEZA) p.rotacion=R_0;
	p.id_pieza+=dir;

	if(p.id_pieza > MAX_PIEZA) p.id_pieza=MIN_PIEZA;
	else if(p.id_pieza < MIN_PIEZA) p.id_pieza=MAX_PIEZA;

	if(p.id_pieza != MIN_PIEZA)
	{	//Si la pieza está colocada o no la tenemos... 
		if(
		(es_pieza_colocada(p.id_pieza, indice_tablero))
		||
		(
			std::find(piezas_obtenidas.begin(), piezas_obtenidas.end(), p.id_pieza) 
			==
			piezas_obtenidas.end()
		)
		)
		{
			cambiar_pieza(dir);
		}
	}

	//Levantamos la bandera para comprobar si el puzzle se ha finalizado.
	comprobar_finalizacion=true;
}

/*La finalidad de este método es generar un recurso que tenga todas las piezas
posibles rotadas 0, 90, 180 y 270 grados.

El proceso para generar las piezas es más pesado que otra cosa.
Empezamos por generar una superficie que sea tan largo como el 
id de la última pieza multiplicado por el ancho del 
marco de una pieza * factor de ampliación. Será cuatro veces tan
alto como marco de una pieza * factor de ampliación.
.*/

void Controlador_puzzle::generar_imagen_piezas()
{
	descartar_imagen_piezas();

	const int factor_ampliacion=2;
	const int id_max=Control_pieza_puzzle::obtener_id_maximo()+1; //Ojo, el primer id es cero, necesitamos uno más!.
	const unsigned int w=factor_ampliacion * id_max * W_PIEZA_SIN_AMPLIAR;
	const unsigned int h=factor_ampliacion * 4 * H_PIEZA_SIN_AMPLIAR;

	//En lugar de una superficie podemos usar una representación	
	//compuesta... Luego podemos copiar su superficie y no necesitamos
	//andar con la memoria de un lado para otro.
	DLibV::Representacion_compuesta_estatica composicion(w, h);

	//Vamos a generar el marco...
	SDL_Rect caja_marco=DLibH::Herramientas_SDL::nuevo_sdl_rect(X_TABLETA, Y_TABLETA, W_TABLETA, H_TABLETA);	
	Uint32 trans_original=DLibV::Gestor_recursos_graficos::obtener(8)->acc_color_transparencia();

	//Esta Imagen liberará la superficie automáticamente cuando salga de ámbito.
	SDL_Surface * marco_ampliado=Herramientas_graficas::ampliar_superficie(DLibV::Gestor_recursos_graficos::obtener(8)->acc_superficie() , factor_ampliacion, caja_marco);
	DLibV::Imagen img_marco(marco_ampliado);
	img_marco.establecer_transparencia(trans_original);

	//El siguiente paso es ir pieza por pieza, ponemos el marco y encima la pieza.
	std::vector<unsigned int> ids=Control_pieza_puzzle::obtener_vector_claves();
	for(unsigned int id : ids)
	{
		Definicion_pieza_puzzle d=Control_pieza_puzzle::obtener_definicion_pieza(id);
		SDL_Rect caja_pieza=DLibH::Herramientas_SDL::nuevo_sdl_rect(d.recorte_x, d.recorte_y, Control_pieza_puzzle::W_PIEZA, Control_pieza_puzzle::H_PIEZA);
	
		//La superficie se libera al salir de ámbito.			
		SDL_Surface * pieza_ampliada=Herramientas_graficas::ampliar_superficie(DLibV::Gestor_recursos_graficos::obtener(d.id_recurso)->acc_superficie(), factor_ampliacion, caja_pieza);
		DLibV::Imagen img_glifo(pieza_ampliada);
		img_glifo.establecer_transparencia(trans_original);

		//Empezamos a montar, reusamos caja pieza.
		DLibV::Representacion_bitmap_estatica bmp;

		caja_pieza.x=id * W_PIEZA_SIN_AMPLIAR * factor_ampliacion;
		caja_pieza.y=0;

		bmp.establecer_recurso(&img_marco);
		composicion.volcar_representacion(bmp, caja_pieza);

		bmp.establecer_recurso(&img_glifo);
		caja_pieza.x+=4;
		caja_pieza.y+=4;
		composicion.volcar_representacion(bmp, caja_pieza);
	}

	//En este momento tenemos ya una fila completa. Ahora vamos a recorrer 
	//cada fila generando rotaciones: recortamos cada pieza y las vamos rotando
	//y pegando.
	
	for(int y=0; y<3; ++y)
	{
		int y_origen=y * (H_PIEZA_SIN_AMPLIAR * factor_ampliacion);
		int y_destino=y_origen + (H_PIEZA_SIN_AMPLIAR * factor_ampliacion);

		for(unsigned int id : ids)
		{
			int x=id * (W_PIEZA_SIN_AMPLIAR * factor_ampliacion);

			SDL_Rect caja_pieza=DLibH::Herramientas_SDL::nuevo_sdl_rect(x, y_origen, W_PIEZA_SIN_AMPLIAR * factor_ampliacion, H_PIEZA_SIN_AMPLIAR * factor_ampliacion);

			SDL_Surface * pieza_completa=composicion.ref_superficie();
			SDL_Surface * rotacion_90=Herramientas_graficas::rotar_superficie_90_derecha(pieza_completa, caja_pieza);

			//Una vez más, la rotación se liberará al finalizar este ciclo.
			DLibV::Imagen img_rotada(rotacion_90);
			img_rotada.establecer_transparencia(trans_original);

			caja_pieza.y=y_destino;
			DLibV::Representacion_bitmap_estatica bmp;
			bmp.establecer_recurso(&img_rotada);
			composicion.volcar_representacion(bmp, caja_pieza);
		}
	}

	//Finalmente generamos una copia de la superficie, que pasa a ser gestionada
	//por la clase. El resto de cosas deben eliminarse sólas.

	SDL_Surface * copia=DLibV::Utilidades_graficas_SDL::copiar_superficie(composicion.ref_superficie());
	img_piezas=new DLibV::Imagen(copia);
}

void Controlador_puzzle::descartar_imagen_piezas()
{
	if(img_piezas)
	{
		delete img_piezas;
		img_piezas=NULL;
	}
}

void Controlador_puzzle::desmontar()
{
	descartar_imagen_piezas();
	montado=false;
}

/*Se generan las superficies necesarias cada vez que preparamos esto.
Las piezas no sería necesario, pero el controlador se crea antes de que
se carguen las piezas disposibles, de modo que es mejor así.*/

void Controlador_puzzle::preparar(
	unsigned int id_puzzle, 
	const std::vector<unsigned int>& piezas)
{
	//Generamos la imagen de las piezas.
	generar_imagen_piezas();

	//Cargamos el puzzle actual y generamos el vector de soluciones.
	Cargador_puzzle c;
	c.inicializar();
	puzzle_actual=c.obtener_puzzle_por_id(id_puzzle);
	generar_estado_puzzle();

	//Copiar vector de piezas disponibles en ese momento.
	piezas_obtenidas=piezas;
	comprobar_finalizacion=false;
	estado=E_MOVER;

	std::string cad_puzzle=DLibH::Herramientas::entero_a_cadena(id_puzzle)+
		": "+
		loc_textos.obtener(id_puzzle);

	std::string cad_ayuda=loc_textos.obtener(id_puzzle+OFFSET_TEXTO_AYUDA);

	rep_txt_titulo.asignar(cad_puzzle);
	rep_txt_ayuda.asignar(cad_ayuda);

	montado=true;
}

/*
Dibuja realmente el tablero.

Se infieren las coordenadas teniendo en cuenta que las piezas son
un vector pero se ordenan según w y h, de izquierda a derecha y 
arriba a abajo (0,0 1,0 2,0, 0,1 1,1...)*/

void Controlador_puzzle::dibujar_tablero(DLibV::Pantalla& pantalla)
{
	DLibV::Representacion_agrupada_estatica grupo(true);

	for(const Pieza_colocada_puzzle& p : estado_puzzle) 
	{
		//Empezamos por inferir las coordenadas a partir del índice.
		Punto pt=obtener_coordenadas_de_indice(p.indice);

		unsigned int x=(pt.x * W_HUECO_PIEZA) + (pt.x * MARGEN_PIEZA);
		unsigned int y=(pt.y * H_HUECO_PIEZA) + (pt.y * MARGEN_PIEZA);

		//Selección del color de fondo.
		Uint32 color=COLOR_TABLERO_NORMAL;
		if(indice_tablero==p.indice)
		{
			color=estado==E_MOVER ? COLOR_TABLERO_ACTUAL : COLOR_TABLERO_SELECCIONADA;
		}

		//Se dibujan dos cajas, una tiene todo el color y la otra es un
		//agujero dentro de la primera.

		DLibV::Primitiva_grafica_caja_estatica * fondo=new DLibV::Primitiva_grafica_caja_estatica(
			DLibH::Herramientas_SDL::nuevo_sdl_rect(x, y, W_HUECO_PIEZA, H_HUECO_PIEZA),
			color);

		DLibV::Primitiva_grafica_caja_estatica * frente=new DLibV::Primitiva_grafica_caja_estatica(
			DLibH::Herramientas_SDL::nuevo_sdl_rect(
				x+BORDE_HUECO_PIEZA, y+BORDE_HUECO_PIEZA, 
				W_HUECO_PIEZA-(2*BORDE_HUECO_PIEZA), 
				H_HUECO_PIEZA-(2*BORDE_HUECO_PIEZA)),
			DLibV::Gestor_color::color(0, 0, 0));

		grupo.insertar_representacion(fondo);
		grupo.insertar_representacion(frente);
	
		//Ahora podemos ver si tiene pieza y la rotación de la misma,
		//para escoger.

		if(p.id_pieza!=Pieza_colocada_puzzle::PIEZA_NULA)
		{
			unsigned int rx=(W_PIEZA_SIN_AMPLIAR * 2) * p.id_pieza;
			unsigned int ry=0;

			switch(p.rotacion)
			{
				case R_0: ry=0; break;
				case R_90: ry=(H_PIEZA_SIN_AMPLIAR * 2) * 1; break;
				case R_180: ry=(H_PIEZA_SIN_AMPLIAR * 2) * 2; break;
				case R_270: ry=(H_PIEZA_SIN_AMPLIAR * 2) * 3; break;
			}

			DLibV::Representacion_bitmap_estatica * tableta=new DLibV::Representacion_bitmap_estatica();
			tableta->establecer_recurso(img_piezas);
			tableta->establecer_posicion(x+BORDE_HUECO_PIEZA, y+BORDE_HUECO_PIEZA);			
			tableta->establecer_recorte(rx, ry, W_PIEZA_SIN_AMPLIAR * 2, H_PIEZA_SIN_AMPLIAR * 2);			
			grupo.insertar_representacion(tableta);
		}
	}

	if(estado==E_CAMBIAR) dibujar_ayuda_input(grupo);


	//Nos faltaría centrar el grupo...

	unsigned int max_x=(puzzle_actual.acc_w() * W_HUECO_PIEZA) + ( (puzzle_actual.acc_w()-1) * MARGEN_PIEZA);
	unsigned int max_y=(puzzle_actual.acc_h() * H_HUECO_PIEZA) + ( (puzzle_actual.acc_h()-1) * MARGEN_PIEZA);

	unsigned int xg=X_TABLERO + Herramientas_calculos::centrar(max_x, W_EFECTIVO_PIEZAS);
	unsigned int yg=Y_TABLERO + Herramientas_calculos::centrar(max_y, H_EFECTIVO_PIEZAS);

	grupo.establecer_posicion(xg, yg);
	grupo.volcar(pantalla);
}

/*Cuando estamos en modo de cambiar ponemos una pequeña ayuda en pantalla...*/

void Controlador_puzzle::dibujar_ayuda_input(DLibV::Representacion_agrupada_estatica& grupo)
{
	//Preparamos la posición
	Punto pt=obtener_coordenadas_de_indice(indice_tablero);
	unsigned int x=(pt.x * W_HUECO_PIEZA) + (pt.x * MARGEN_PIEZA);
	unsigned int y=(pt.y * H_HUECO_PIEZA) + (pt.y * MARGEN_PIEZA);

	//Preparamos las cajas.
	std::vector<SDL_Rect> posiciones;
	posiciones.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_INPUT_ARR_ABA+x, Y_INPUT_ARR+y, W_REC_ARR_ABA, H_REC_ARR_ABA));
	posiciones.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_INPUT_ARR_ABA+x, Y_INPUT_ABA+y, W_REC_ARR_ABA, H_REC_ARR_ABA));
	posiciones.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_INPUT_IZQ+x, Y_INPUT_IZQ_DER+y, W_REC_IZQ_DER, W_REC_IZQ_DER));
	posiciones.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_INPUT_DER+x, Y_INPUT_IZQ_DER+y, W_REC_IZQ_DER, W_REC_IZQ_DER));

	std::vector<SDL_Rect> recortes;
	recortes.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_REC_ARR, Y_REC_ARR, W_REC_ARR_ABA, H_REC_ARR_ABA));
	recortes.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_REC_ABA, Y_REC_ABA, W_REC_ARR_ABA, H_REC_ARR_ABA));
	recortes.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_REC_IZQ, Y_REC_IZQ, W_REC_IZQ_DER, W_REC_IZQ_DER));
	recortes.push_back(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_REC_DER, Y_REC_DER, W_REC_IZQ_DER, W_REC_IZQ_DER));

	unsigned int i=0;
	while(i < posiciones.size())
	{
		//Preparamos el recurso. Serán poseidos por el grupo, que los
		//liberará.
		DLibV::Representacion_bitmap_estatica * bmp=new DLibV::Representacion_bitmap_estatica();
		bmp->establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(8));

		//Cambiar alpha según propiedad tiempo. 
		int alpha=Herramientas_calculos::calcular_valor_onda_triangulo(tiempo, T_PARPADEO) * 255;
		bmp->establecer_alpha(alpha);
		bmp->establecer_posicion(posiciones[i]);
		bmp->establecer_recorte(recortes[i]);
		grupo.insertar_representacion(bmp);
		++i;
	}
}

Controlador_puzzle::Punto Controlador_puzzle::obtener_coordenadas_de_indice(unsigned int i)
{
	unsigned int py=i / puzzle_actual.acc_w();
	unsigned int px=i % puzzle_actual.acc_w();

	return Punto(px, py);
}

void Controlador_puzzle::generar_estado_puzzle()
{
	indice_tablero=0;

	estado_puzzle.clear();
	unsigned int t=puzzle_actual.acc_w() * puzzle_actual.acc_h();
	unsigned int i=0;

	while(i < t)
	{
		estado_puzzle.push_back(Pieza_colocada_puzzle(i));
		i++;
	}
}

/*Esto está completamente fusilado del controlador de status. Es básicamente
el mismo código. Hemos añadido un salto en el eje y para poder rediseñar un poco
la estructura de la pantalla del puzzle.
Un detalle: las piezas se vuelcan en una representación compuesta, que luego 
vamos a volcar posicionándola en la pantalla.
*/

void Controlador_puzzle::dibujar_piezas_disponibles(DLibV::Pantalla& p)
{
	if(piezas_obtenidas.size())
	{
		DLibV::Representacion_compuesta_estatica rep_iconos_puzzle(W_REP_PIEZAS, H_REP_PIEZAS);

		int x=0, y=0;
		unsigned int total_dibujadas=0;
		DLibV::Representacion_bitmap_estatica rbe;
		
		for(unsigned int i : piezas_obtenidas)
		{			
			//Si la pieza está ya colocada, debe salir transparente.
			unsigned int alpha=es_pieza_colocada(i) ? 128 : 255;

			Definicion_pieza_puzzle d=Control_pieza_puzzle::obtener_definicion_pieza(i);
			rbe.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(d.id_recurso));
			rbe.establecer_alpha(alpha);

			//Marco...
			rbe.establecer_posicion(x, y);
			rbe.establecer_recorte(X_TABLETA, Y_TABLETA, W_TABLETA, H_TABLETA);
			rep_iconos_puzzle.volcar_representacion(rbe, 
				DLibH::Herramientas_SDL::nuevo_sdl_rect(
					x, y, W_TABLETA, H_TABLETA));

			//Símbolo.
			rbe.establecer_posicion(x+2, y+2);
			rbe.establecer_recorte(d.recorte_x, d.recorte_y, Control_pieza_puzzle::W_PIEZA, Control_pieza_puzzle::H_PIEZA);
			rep_iconos_puzzle.volcar_representacion(rbe, 
				DLibH::Herramientas_SDL::nuevo_sdl_rect(
					x+2, y+2, Control_pieza_puzzle::W_PIEZA, Control_pieza_puzzle::H_PIEZA
					));

			++total_dibujadas;
		
			if(total_dibujadas==PIEZAS_POR_LINEA)
			{
				x=0;
				y+=SALTO_Y_PIEZAS;
			}
			else
			{
				x+=SALTO_X_PIEZAS;
			}
			
		}
		
		rep_iconos_puzzle.preparar();
		rep_iconos_puzzle.establecer_posicion(X_REP_PIEZAS, Y_REP_PIEZAS);
		rep_iconos_puzzle.volcar(p);
	}
}

/*Comrpueba si una pieza está ya puesta en el tablero. Si se pasa el segundo
parámetro con un valor distinto de -1 se ignorará la casilla con el índice 
seleccionado.*/

bool Controlador_puzzle::es_pieza_colocada(int i, int ignorar) const
{
	for(const Pieza_colocada_puzzle& p : estado_puzzle) 
	{
		if(ignorar!=-1 && (int) p.indice==ignorar) continue;
		if(p.id_pieza==i) return true;
	}

	return false;
}

void Controlador_puzzle::dibujar(DLibV::Pantalla& p)
{
	caja_borde.volcar(p);
	caja_interior.volcar(p);
	caja_separador.volcar(p);

	dibujar_dibujo_puzzle(p);
	dibujar_piezas_disponibles(p);
	dibujar_tablero(p);

	rep_txt_titulo.volcar(p);
	rep_txt_ayuda.volcar(p);

	//Tiempo restante...
	const std::string tiempo_restante=Tiempo_juego::como_cadena(tiempo_juego.acc_segundos_restantes());
	Representador_tiempo_restante::generar_contador_tiempo(p, tiempo_restante);
}

void Controlador_puzzle::dibujar_dibujo_puzzle(DLibV::Pantalla& p)
{
	const int x=puzzle_actual.acc_x_img();
	const int y=puzzle_actual.acc_y_img();
	const int w=puzzle_actual.acc_w_img();
	const int h=puzzle_actual.acc_h_img();

	//Crear la representacion
	DLibV::Representacion_bitmap_estatica bmp;
	bmp.establecer_recurso(DLibV::Gestor_recursos_graficos::obtener(puzzle_actual.acc_recurso_img()));
	bmp.establecer_recorte(x, y, w, h);
	
	//Ahora centramos.
	int px=Herramientas_calculos::centrar(w, W_MAX_DIBUJO);
	int py=Herramientas_calculos::centrar(h, H_MAX_DIBUJO);

	bmp.establecer_posicion(px+X_DIBUJO, py+Y_DIBUJO, w, h);
	bmp.volcar(p);
}

/*Cuando se finaliza un puzzle la pantalla hace un destello y cuando el
destello se termina volvemos al modo de juego.*/

void Controlador_puzzle::iniciar_finalizacion(DLibV::Pantalla& p)
{
	estado=E_FINALIZANDO;

	tiempo_juego.detener(); //El tiempo se para durante el rato que dura el destello.

	p.adjuntar_proceso(
		DLibV::Proceso_superficie_fundido::generar(
			p.acc_superficie(),
			255, 255, 255,
			0, 255,
			T_DESTELLO_FINAL));
}


bool Controlador_puzzle::loop(float delta, DLibV::Pantalla& p, const Input& i)
{
	bool resultado=true;

	tiempo+=delta;
	tiempo_juego.turno(delta);

	switch(estado)
	{
		case E_MOVER:
		case E_CAMBIAR:
			//Establecemos la bandera. Si hay algún cambio se pondrá a true.
			//Los cambios sólo tienen lugar cuando hay input que cambie
			//una pieza.

			comprobar_finalizacion=false;
			resultado=procesar_controles(i);

			//Habilitar comprobación del puzzle. Si se comprueba se da por finalizado.
			if(comprobar_finalizacion && comprobar_solucion()) iniciar_finalizacion(p);
			if(tiempo_juego.es_finalizado()) resultado=false;
		break;

		//Hasta que no termine el proceso de pantalla no salimos.
		//No se procesa input.
		case E_FINALIZANDO:
			if(!p.hay_procesos_en_marcha())
			{
				tiempo_juego.arrancar(); //El tiempo se reinicia llegado este momento.
				estado=E_FINALIZADO;
				resultado=false;
			}
		break;
	}

	p.rellenar(0, 0, 0);
	//En estado finalizado no dibujamos nada para que no se nos cuele un frame.
	if(estado!=E_FINALIZADO) dibujar(p);

	p.procesar_procesos(delta);

	return resultado;
}

/*Para comprobar la solución recorremos el estado del puzzle y comparamos cada
pieza y rotación con el puzzle original. Algunas rotaciones pueden ser 
equivalentes en ciertos puzzles, de modo que lo establecemos de esa forma.*/

bool Controlador_puzzle::comprobar_solucion()
{
	const std::vector<Pieza_colocada_puzzle>& solucion=puzzle_actual.acc_solucion();
	const unsigned int max=solucion.size();
	
	if(max!=estado_puzzle.size()) return false;
	else
	{
		unsigned int i=0;
		while(i < max)
		{
			const Pieza_colocada_puzzle& p=solucion[i];
			const Pieza_colocada_puzzle& s=estado_puzzle[i];

			if(p.id_pieza != s.id_pieza) return false;
			else if(! (p.rotacion & s.rotacion)) return false;

			++i;
		}
	}

	return true;
}
