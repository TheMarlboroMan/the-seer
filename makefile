CFLAGS=-c -g -O0 -Wno-deprecated -Wall -ansi -pedantic -std=c++11
CFLAGS_FINAL=-g -O0 -Wno-deprecated -Wall -ansi -pedantic -std=c++11
#CFLAGS=-c -O3 -Wno-deprecated -Wall -ansi -pedantic -std=c++11
#CFLAGS_FINAL=-O3 -Wno-deprecated -Wall -ansi -pedantic -std=c++11
CC=g++
#-DLOG_BASE_SILENCIOSO=1
DIR_CLASS=./class/
DIR_O=./objects/

OBJETOS=$(DIR_O)control_temporizador.o $(DIR_O)temporizador_animaciones.o \
$(DIR_O)datos_legibles.o $(DIR_O)geometria.o $(DIR_O)visitante.o $(DIR_O)representable.o \
$(DIR_O)datos_colision.o $(DIR_O)espaciable.o $(DIR_O)bloque.o $(DIR_O)actor.o \
$(DIR_O)sombra.o $(DIR_O)actor_movil.o $(DIR_O)jugador.o \
$(DIR_O)interactuable.o $(DIR_O)interruptor.o $(DIR_O)visitante_eliminado_sala.o \
$(DIR_O)visitante_insercion_sala.o $(DIR_O)sala.o $(DIR_O)calculo_colisiones.o \
$(DIR_O)factoria_actores.o $(DIR_O)cargador_datos.o $(DIR_O)representador.o \
$(DIR_O)controlador.o $(DIR_O)activable.o $(DIR_O)localizador.o  \
$(DIR_O)control_salas.o $(DIR_O)control_objetivos.o $(DIR_O)audio.o \
$(DIR_O)utilidades_cargadores.o $(DIR_O)cargador_recursos.o \
$(DIR_O)comprobador_rampas.o $(DIR_O)gestor_mapa_datos_legibles.o $(DIR_O)datos_diapositivas.o\
$(DIR_O)cargador_diapositivas.o $(DIR_O)sistema.o $(DIR_O)controlador_diapositivas.o\
$(DIR_O)control_puzzle.o $(DIR_O)cargador_mapa.o $(DIR_O)datos_mapa.o \
$(DIR_O)controlador_mapa.o $(DIR_O)tiempo_juego.o $(DIR_O)controlador_auxiliar.o\
$(DIR_O)controlador_status.o $(DIR_O)control_energia.o $(DIR_O)control_habilidades.o\
$(DIR_O)input.o $(DIR_O)controlador_info_juego.o $(DIR_O)diccionario_info_juego.o\
$(DIR_O)herramientas_graficas.o $(DIR_O)control_pieza_puzzle.o $(DIR_O)controlador_puzzle.o\
$(DIR_O)herramientas_calculos.o $(DIR_O)representador_tiempo_restante.o $(DIR_O)pantallazo_sistema.o\
$(DIR_O)controlador_confirmacion.o $(DIR_O)controlador_intro.o\
$(DIR_O)control_actores_persistentes.o $(DIR_O)definiciones_serializador_deserializador.o\
$(DIR_O)serializador.o $(DIR_O)visitante_serializacion.o $(DIR_O)deserializador.o\
$(DIR_O)menu_seleccionable.o $(DIR_O)control_sesiones_guardadas.o $(DIR_O)estructura_animacion.o\
$(DIR_O)configuracion.o $(DIR_O)item_configuracion.o $(DIR_O)cargador_idiomas.o

ACTORES=$(DIR_O)entrada_salida_sala.o\
$(DIR_O)interruptor_repetible.o\
$(DIR_O)interruptor_no_repetible.o\
$(DIR_O)interruptor_invisible_no_repetible.o\
$(DIR_O)interruptor_invisible_repetible.o\
$(DIR_O)cofre.o\
$(DIR_O)proyectil_obstaculo.o\
$(DIR_O)disparador_proyectiles.o\
$(DIR_O)actor_control_direccion.o\
$(DIR_O)actor_generico.o\
$(DIR_O)alternador_actor_generico.o\
$(DIR_O)actor_audio.o\
$(DIR_O)plataforma_flexible.o\
$(DIR_O)escalera.o\
$(DIR_O)actor_diapositivas.o\
$(DIR_O)interruptor_contacto.o\
$(DIR_O)teletransportador.o\
$(DIR_O)actor_info_juego.o\
$(DIR_O)actor_puzzle.o\
$(DIR_O)actor_punto_guardado.o\
$(DIR_O)actor_recuperar_tiempo.o\
$(DIR_O)interruptor_auto.o

DEPENDENCIAS_VG=$(DIR_O)cargador_recursos.o\
$(DIR_O)utilidades_cargadores.o\
$(DIR_O)datos_legibles.o\
$(DIR_O)gestor_mapa_datos_legibles.o

DEPENDENCIAS_MAPA=$(DIR_O)cargador_recursos.o\
$(DIR_O)utilidades_cargadores.o\
$(DIR_O)datos_mapa.o\
$(DIR_O)gestor_mapa_datos_legibles.o\
$(DIR_O)cargador_mapa.o\

all: a.out

vg:\
$(OBJETOS)\
$(ACTORES)\
visor_graficos.cpp
	$(CC) $(CFLAGS_FINAL) visor_graficos.cpp $(DEPENDENCIAS_VG) -o vg.out -L. -lDanSDL -lSDL -lSDL_mixer -lSDL_image

mapa:\
$(OBJETOS)\
$(ACTORES)\
mapa.cpp
	$(CC) $(CFLAGS_FINAL) mapa.cpp $(DEPENDENCIAS_MAPA) -o mapa.out -L. -lDanSDL -lSDL -lSDL_mixer -lSDL_image

zonas_mapa:\
$(OBJETOS)\
$(ACTORES)\
zonas_mapa.cpp
	$(CC) $(CFLAGS_FINAL) zonas_mapa.cpp $(DEPENDENCIAS_MAPA) -o zonas_mapa.out -L. -lDanSDL -lSDL -lSDL_mixer -lSDL_image

a.out: $(OBJETOS) $(ACTORES) \
main.cpp \
libDanSDL.a
	$(CC) $(CFLAGS_FINAL) main.cpp $(OBJETOS) $(ACTORES) -o a.out -L. -lDanSDL -lSDL -lSDL_mixer -lSDL_image

$(DIR_O)sistema.o:\
$(DIR_O)pantallazo_sistema.o\
$(DIR_O)input.o\
$(DIR_O)configuracion.o\
$(DIR_O)controlador.o\
$(DIR_O)control_sesiones_guardadas.o\
$(DIR_O)cargador_recursos.o\
$(DIR_O)controlador_intro.o\
$(DIR_O)controlador_diapositivas.o\
$(DIR_O)controlador_auxiliar.o\
$(DIR_O)controlador_puzzle.o\
$(DIR_O)controlador_info_juego.o\
$(DIR_O)controlador_confirmacion.o\
$(DIR_CLASS)datos_sistema.h\
$(DIR_CLASS)tiempo_sistema.h\
$(DIR_CLASS)sistema.h\
$(DIR_CLASS)sistema.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)sistema.cpp -o $(DIR_O)sistema.o

$(DIR_O)controlador.o:\
$(DIR_O)input.o\
$(DIR_O)control_actores_persistentes.o\
$(DIR_O)control_puzzle.o\
$(DIR_O)control_salas.o\
$(DIR_O)control_objetivos.o\
$(DIR_O)comprobador_rampas.o\
$(DIR_O)cargador_datos.o\
$(DIR_O)localizador.o\
$(DIR_O)serializador.o\
$(DIR_O)deserializador.o\
$(DIR_O)sala.o\
$(DIR_O)jugador.o\
$(DIR_O)sombra.o\
$(DIR_O)audio.o\
$(DIR_O)representador.o\
$(DIR_O)datos_colision.o\
$(DIR_O)calculo_colisiones.o\
$(DIR_O)control_temporizador.o\
$(DIR_O)entrada_salida_sala.o\
$(DIR_O)tiempo_juego.o\
$(DIR_O)control_energia.o\
$(DIR_O)control_habilidades.o\
$(DIR_O)diccionario_info_juego.o\
$(DIR_O)representador_tiempo_restante.o\
$(DIR_CLASS)datos_sistema.h\
$(DIR_CLASS)info_juego.h\
$(DIR_CLASS)visitante_colision.h\
$(DIR_CLASS)visitante_interaccion.h\
$(DIR_CLASS)visitante_turno.h\
$(DIR_CLASS)controlador.h\
$(DIR_CLASS)controlador.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)controlador.cpp -o $(DIR_O)controlador.o

$(DIR_O)controlador_puzzle.o:\
$(DIR_O)input.o\
$(DIR_O)localizador.o\
$(DIR_O)tiempo_juego.o\
$(DIR_O)herramientas_graficas.o\
$(DIR_O)herramientas_calculos.o\
$(DIR_O)control_pieza_puzzle.o\
$(DIR_O)representador_tiempo_restante.o\
$(DIR_O)datos_legibles.o\
$(DIR_O)utilidades_cargadores.o\
$(DIR_CLASS)datos_sistema.h\
$(DIR_CLASS)controlador_puzzle.h\
$(DIR_CLASS)controlador_puzzle.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)controlador_puzzle.cpp -o $(DIR_O)controlador_puzzle.o

$(DIR_O)controlador_confirmacion.o:\
$(DIR_O)controlador_info_juego.o\
$(DIR_CLASS)controlador_confirmacion.h\
$(DIR_CLASS)controlador_confirmacion.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)controlador_confirmacion.cpp -o $(DIR_O)controlador_confirmacion.o

$(DIR_O)controlador_intro.o:\
$(DIR_O)localizador.o\
$(DIR_O)cargador_idiomas.o\
$(DIR_O)estructura_animacion.o\
$(DIR_O)configuracion.o\
$(DIR_O)item_configuracion.o\
$(DIR_O)input.o\
$(DIR_O)control_sesiones_guardadas.o\
$(DIR_O)menu_seleccionable.o\
$(DIR_O)herramientas_calculos.o\
$(DIR_CLASS)controlador_intro.h\
$(DIR_CLASS)controlador_intro.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)controlador_intro.cpp -o $(DIR_O)controlador_intro.o

$(DIR_O)controlador_info_juego.o:\
$(DIR_O)localizador.o\
$(DIR_O)input.o\
$(DIR_O)diccionario_info_juego.o\
$(DIR_CLASS)datos_sistema.h\
$(DIR_CLASS)controlador_info_juego.h\
$(DIR_CLASS)controlador_info_juego.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)controlador_info_juego.cpp -o $(DIR_O)controlador_info_juego.o

$(DIR_O)controlador_diapositivas.o:\
$(DIR_O)cargador_diapositivas.o\
$(DIR_O)herramientas_graficas.o\
$(DIR_O)herramientas_calculos.o\
$(DIR_O)input.o\
$(DIR_CLASS)datos_sistema.h\
$(DIR_CLASS)controlador_diapositivas.h\
$(DIR_CLASS)controlador_diapositivas.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)controlador_diapositivas.cpp -o $(DIR_O)controlador_diapositivas.o

$(DIR_O)controlador_auxiliar.o:\
$(DIR_O)input.o\
$(DIR_O)controlador_mapa.o\
$(DIR_O)controlador_status.o\
$(DIR_O)control_habilidades.o\
$(DIR_CLASS)datos_sistema.h\
$(DIR_CLASS)controlador_auxiliar.h\
$(DIR_CLASS)controlador_auxiliar.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)controlador_auxiliar.cpp -o $(DIR_O)controlador_auxiliar.o

$(DIR_O)controlador_status.o:\
$(DIR_O)control_objetivos.o\
$(DIR_O)control_salas.o\
$(DIR_O)control_pieza_puzzle.o\
$(DIR_O)localizador.o\
$(DIR_CLASS)info_juego.h\
$(DIR_CLASS)datos_sistema.h\
$(DIR_CLASS)controlador_status.h\
$(DIR_CLASS)controlador_status.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)controlador_status.cpp -o $(DIR_O)controlador_status.o

$(DIR_O)controlador_mapa.o:\
$(DIR_O)datos_mapa.o\
$(DIR_O)cargador_mapa.o\
$(DIR_O)control_salas.o\
$(DIR_O)localizador.o\
$(DIR_CLASS)datos_sistema.h\
$(DIR_CLASS)controlador_mapa.h\
$(DIR_CLASS)controlador_mapa.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)controlador_mapa.cpp -o $(DIR_O)controlador_mapa.o

$(DIR_O)audio.o:\
$(DIR_CLASS)audio.h\
$(DIR_CLASS)audio.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)audio.cpp -o $(DIR_O)audio.o

$(DIR_O)input.o:\
$(DIR_CLASS)input.h\
$(DIR_CLASS)input.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)input.cpp -o $(DIR_O)input.o

$(DIR_O)representador.o:\
$(DIR_O)representable.o\
$(DIR_CLASS)representador.h\
$(DIR_CLASS)representador.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)representador.cpp -o $(DIR_O)representador.o

$(DIR_O)control_sesiones_guardadas.o:\
$(DIR_O)deserializador.o\
$(DIR_CLASS)control_sesiones_guardadas.h\
$(DIR_CLASS)control_sesiones_guardadas.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)control_sesiones_guardadas.cpp -o $(DIR_O)control_sesiones_guardadas.o

$(DIR_O)cargador_recursos.o:\
$(DIR_O)utilidades_cargadores.o\
$(DIR_CLASS)cargador_recursos.h\
$(DIR_CLASS)cargador_recursos.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)cargador_recursos.cpp -o $(DIR_O)cargador_recursos.o

$(DIR_O)cargador_datos.o:\
$(DIR_O)control_actores_persistentes.o\
$(DIR_O)utilidades_cargadores.o\
$(DIR_O)visitante.o\
$(DIR_O)control_salas.o\
$(DIR_O)sala.o\
$(DIR_O)datos_legibles.o\
$(DIR_O)factoria_actores.o\
$(DIR_CLASS)cargador_datos.h\
$(DIR_CLASS)cargador_datos.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)cargador_datos.cpp -o $(DIR_O)cargador_datos.o

$(DIR_O)control_actores_persistentes.o:\
$(DIR_O)actor.o\
$(DIR_O)control_objetivos.o\
$(DIR_CLASS)visitante_recuento_items.h\
$(DIR_CLASS)control_actores_persistentes.h\
$(DIR_CLASS)control_actores_persistentes.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)control_actores_persistentes.cpp -o $(DIR_O)control_actores_persistentes.o

$(DIR_O)cargador_mapa.o:\
$(DIR_O)utilidades_cargadores.o\
$(DIR_O)datos_mapa.o\
$(DIR_CLASS)cargador_mapa.h\
$(DIR_CLASS)cargador_mapa.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)cargador_mapa.cpp -o $(DIR_O)cargador_mapa.o

$(DIR_O)cargador_diapositivas.o:\
$(DIR_O)utilidades_cargadores.o\
$(DIR_O)datos_diapositivas.o\
$(DIR_O)localizador.o\
$(DIR_CLASS)cargador_diapositivas.h\
$(DIR_CLASS)cargador_diapositivas.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)cargador_diapositivas.cpp -o $(DIR_O)cargador_diapositivas.o

$(DIR_O)factoria_actores.o :\
$(ACTORES)\
$(DIR_CLASS)factoria_actores.h\
$(DIR_CLASS)factoria_actores.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)factoria_actores.cpp -o $(DIR_O)factoria_actores.o

$(DIR_O)calculo_colisiones.o :\
$(DIR_O)geometria.o\
$(DIR_O)datos_colision.o\
$(DIR_O)actor.o\
$(DIR_O)bloque.o\
$(DIR_CLASS)calculo_colisiones.h\
$(DIR_CLASS)calculo_colisiones.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)calculo_colisiones.cpp -o $(DIR_O)calculo_colisiones.o

$(DIR_O)sala.o :\
$(DIR_O)bloque.o\
$(DIR_CLASS)visitante_insercion_sala.h\
$(DIR_CLASS)visitante_eliminado_sala.h\
$(DIR_CLASS)visitante_salida_sala.h\
$(DIR_CLASS)visitante_activacion.h\
$(DIR_CLASS)sala.h\
$(DIR_CLASS)sala.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)sala.cpp -o $(DIR_O)sala.o

$(DIR_O)visitante_insercion_sala.o :\
$(DIR_O)visitante.o\
$(ACTORES)\
$(DIR_CLASS)visitante_insercion_sala.h\
$(DIR_CLASS)visitante_insercion_sala.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)visitante_insercion_sala.cpp -o $(DIR_O)visitante_insercion_sala.o

$(DIR_O)visitante_eliminado_sala.o :\
$(DIR_O)visitante.o\
$(ACTORES)\
$(DIR_CLASS)visitante_eliminado_sala.h\
$(DIR_CLASS)visitante_eliminado_sala.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)visitante_eliminado_sala.cpp -o $(DIR_O)visitante_eliminado_sala.o

$(DIR_O)interruptor_auto.o :\
$(DIR_O)actor.o\
$(DIR_O)activable.o\
$(DIR_CLASS)interruptor_auto.h\
$(DIR_CLASS)interruptor_auto.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)interruptor_auto.cpp -o $(DIR_O)interruptor_auto.o

$(DIR_O)interruptor_contacto.o :\
$(DIR_O)actor.o\
$(DIR_O)activable.o\
$(DIR_CLASS)interruptor_contacto.h\
$(DIR_CLASS)interruptor_contacto.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)interruptor_contacto.cpp -o $(DIR_O)interruptor_contacto.o

$(DIR_O)interruptor_invisible_repetible.o :\
$(DIR_O)interruptor_repetible.o\
$(DIR_CLASS)interruptor_invisible_repetible.h\
$(DIR_CLASS)interruptor_invisible_repetible.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)interruptor_invisible_repetible.cpp -o $(DIR_O)interruptor_invisible_repetible.o


$(DIR_O)interruptor_repetible.o :\
$(DIR_O)interruptor.o\
$(DIR_CLASS)interruptor_repetible.h\
$(DIR_CLASS)interruptor_repetible.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)interruptor_repetible.cpp -o $(DIR_O)interruptor_repetible.o

$(DIR_O)interruptor_no_repetible.o :\
$(DIR_O)interruptor.o\
$(DIR_CLASS)interruptor_no_repetible.h\
$(DIR_CLASS)interruptor_no_repetible.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)interruptor_no_repetible.cpp -o $(DIR_O)interruptor_no_repetible.o

$(DIR_O)interruptor_invisible_no_repetible.o :\
$(DIR_O)interruptor.o\
$(DIR_CLASS)interruptor_invisible_no_repetible.h\
$(DIR_CLASS)interruptor_invisible_no_repetible.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)interruptor_invisible_no_repetible.cpp -o $(DIR_O)interruptor_invisible_no_repetible.o

$(DIR_O)interruptor.o :\
$(DIR_O)interactuable.o\
$(DIR_CLASS)interruptor.h\
$(DIR_CLASS)interruptor.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)interruptor.cpp -o $(DIR_O)interruptor.o

$(DIR_O)cofre.o :\
$(DIR_O)interactuable.o\
$(DIR_O)control_pieza_puzzle.o\
$(DIR_CLASS)cofre.h\
$(DIR_CLASS)cofre.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)cofre.cpp -o $(DIR_O)cofre.o

$(DIR_O)actor_recuperar_tiempo.o :\
$(DIR_O)interactuable.o\
$(DIR_CLASS)actor_recuperar_tiempo.h\
$(DIR_CLASS)actor_recuperar_tiempo.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor_recuperar_tiempo.cpp -o $(DIR_O)actor_recuperar_tiempo.o

$(DIR_O)actor_punto_guardado.o :\
$(DIR_O)interactuable.o\
$(DIR_O)temporizador_animaciones.o\
$(DIR_CLASS)actor_punto_guardado.h\
$(DIR_CLASS)actor_punto_guardado.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor_punto_guardado.cpp -o $(DIR_O)actor_punto_guardado.o

$(DIR_O)interactuable.o :\
$(DIR_O)actor.o\
$(DIR_CLASS)interactuable.h\
$(DIR_CLASS)interactuable.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)interactuable.cpp -o $(DIR_O)interactuable.o

$(DIR_O)plataforma_flexible.o :\
$(DIR_O)actor.o \
$(DIR_CLASS)plataforma_flexible.h\
$(DIR_CLASS)plataforma_flexible.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)plataforma_flexible.cpp -o $(DIR_O)plataforma_flexible.o

$(DIR_O)escalera.o :\
$(DIR_O)activable.o \
$(DIR_O)actor.o\
$(DIR_CLASS)escalera.h\
$(DIR_CLASS)escalera.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)escalera.cpp -o $(DIR_O)escalera.o

$(DIR_O)actor_diapositivas.o :\
$(DIR_O)activable.o \
$(DIR_O)actor.o\
$(DIR_CLASS)actor_diapositivas.h\
$(DIR_CLASS)actor_diapositivas.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor_diapositivas.cpp -o $(DIR_O)actor_diapositivas.o

$(DIR_O)actor_info_juego.o :\
$(DIR_O)activable.o \
$(DIR_O)actor.o\
$(DIR_CLASS)actor_info_juego.h\
$(DIR_CLASS)actor_info_juego.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor_info_juego.cpp -o $(DIR_O)actor_info_juego.o

$(DIR_O)actor_puzzle.o :\
$(DIR_O)activable.o \
$(DIR_O)actor.o\
$(DIR_CLASS)actor_puzzle.h\
$(DIR_CLASS)actor_puzzle.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor_puzzle.cpp -o $(DIR_O)actor_puzzle.o

$(DIR_O)actor_audio.o :\
$(DIR_O)activable.o \
$(DIR_O)audio.o\
$(DIR_O)actor.o\
$(DIR_CLASS)actor_audio.h\
$(DIR_CLASS)actor_audio.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor_audio.cpp -o $(DIR_O)actor_audio.o

$(DIR_O)alternador_actor_generico.o :\
$(DIR_O)actor_generico.o\
$(DIR_O)datos_legibles.o\
$(DIR_O)activable.o \
$(DIR_CLASS)alternador_actor_generico.h\
$(DIR_CLASS)alternador_actor_generico.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)alternador_actor_generico.cpp -o $(DIR_O)alternador_actor_generico.o

$(DIR_O)disparador_proyectiles.o :\
$(DIR_O)actor.o\
$(DIR_O)proyectil_obstaculo.o\
$(DIR_O)activable.o \
$(DIR_CLASS)disparador_proyectiles.h\
$(DIR_CLASS)disparador_proyectiles.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)disparador_proyectiles.cpp -o $(DIR_O)disparador_proyectiles.o

$(DIR_O)proyectil_obstaculo.o :\
$(DIR_O)actor_movil.o\
$(DIR_CLASS)proyectil_obstaculo.h\
$(DIR_CLASS)proyectil_obstaculo.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)proyectil_obstaculo.cpp -o $(DIR_O)proyectil_obstaculo.o

$(DIR_O)jugador.o :\
$(DIR_O)actor_movil.o\
$(DIR_CLASS)jugador.h\
$(DIR_CLASS)jugador.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)jugador.cpp -o $(DIR_O)jugador.o

$(DIR_O)actor_movil.o :\
$(DIR_O)actor.o\
$(DIR_CLASS)actor_movil.h\
$(DIR_CLASS)actor_movil.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor_movil.cpp -o $(DIR_O)actor_movil.o

$(DIR_O)actor_generico.o :\
$(DIR_O)actor.o\
$(DIR_O)datos_legibles.o\
$(DIR_O)temporizador_animaciones.o\
$(DIR_CLASS)actor_generico.h\
$(DIR_CLASS)actor_generico.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor_generico.cpp -o $(DIR_O)actor_generico.o

$(DIR_O)sombra.o :\
$(DIR_O)actor.o\
$(DIR_O)herramientas_graficas.o\
$(DIR_CLASS)sombra.h\
$(DIR_CLASS)sombra.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)sombra.cpp -o $(DIR_O)sombra.o

$(DIR_O)teletransportador.o :\
$(DIR_O)actor.o\
$(DIR_CLASS)teletransportador.h\
$(DIR_CLASS)teletransportador.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)teletransportador.cpp -o $(DIR_O)teletransportador.o


$(DIR_O)entrada_salida_sala.o :\
$(DIR_O)actor.o\
$(DIR_CLASS)entrada_salida_sala.h\
$(DIR_CLASS)entrada_salida_sala.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)entrada_salida_sala.cpp -o $(DIR_O)entrada_salida_sala.o

$(DIR_O)actor_control_direccion.o :\
$(DIR_O)actor.o\
$(DIR_CLASS)actor_control_direccion.h\
$(DIR_CLASS)actor_control_direccion.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor_control_direccion.cpp -o $(DIR_O)actor_control_direccion.o

$(DIR_O)actor.o :\
$(DIR_O)representable.o\
$(DIR_O)geometria.o\
$(DIR_O)visitante.o\
$(DIR_O)datos_colision.o\
$(DIR_O)espaciable.o\
$(DIR_O)visitante.o\
$(DIR_CLASS)actor.h\
$(DIR_CLASS)actor.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)actor.cpp -o $(DIR_O)actor.o

$(DIR_O)bloque.o :\
$(DIR_O)representable.o\
$(DIR_O)geometria.o\
$(DIR_O)visitante.o\
$(DIR_O)datos_colision.o\
$(DIR_O)temporizador_animaciones.o\
$(DIR_O)datos_legibles.o\
$(DIR_O)espaciable.o\
$(DIR_CLASS)bloque.h\
$(DIR_CLASS)bloque.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)bloque.cpp -o $(DIR_O)bloque.o

$(DIR_O)espaciable.o :\
$(DIR_O)geometria.o\
$(DIR_CLASS)espaciable.h\
$(DIR_CLASS)espaciable.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)espaciable.cpp -o $(DIR_O)espaciable.o

$(DIR_O)datos_colision.o :\
$(DIR_O)geometria.o\
$(DIR_CLASS)datos_colision.h\
$(DIR_CLASS)datos_colision.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)datos_colision.cpp -o $(DIR_O)datos_colision.o

$(DIR_O)representable.o :\
$(DIR_O)geometria.o\
$(DIR_CLASS)representable.h\
$(DIR_CLASS)representable.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)representable.cpp -o $(DIR_O)representable.o

$(DIR_O)control_temporizador.o :\
$(DIR_O)datos_legibles.o\
$(DIR_CLASS)control_temporizador.h\
$(DIR_CLASS)control_temporizador.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)control_temporizador.cpp -o $(DIR_O)control_temporizador.o

$(DIR_O)temporizador_animaciones.o :\
$(DIR_CLASS)temporizador_animaciones.h \
$(DIR_CLASS)temporizador_animaciones.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)temporizador_animaciones.cpp -o $(DIR_O)temporizador_animaciones.o

$(DIR_O)visitante.o :\
$(DIR_CLASS)visitante.cpp\
$(DIR_CLASS)visitante.h
	$(CC) $(CFLAGS) $(DIR_CLASS)visitante.cpp -o $(DIR_O)visitante.o

$(DIR_O)datos_mapa.o :\
$(DIR_O)gestor_mapa_datos_legibles.o\
$(DIR_CLASS)datos_mapa.h \
$(DIR_CLASS)datos_mapa.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)datos_mapa.cpp -o $(DIR_O)datos_mapa.o


$(DIR_O)datos_diapositivas.o :\
$(DIR_O)gestor_mapa_datos_legibles.o\
$(DIR_CLASS)datos_diapositivas.h \
$(DIR_CLASS)datos_diapositivas.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)datos_diapositivas.cpp -o $(DIR_O)datos_diapositivas.o

$(DIR_O)datos_legibles.o :\
$(DIR_O)gestor_mapa_datos_legibles.o\
$(DIR_CLASS)datos_legibles.h \
$(DIR_CLASS)datos_legibles.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)datos_legibles.cpp -o $(DIR_O)datos_legibles.o

$(DIR_O)gestor_mapa_datos_legibles.o: \
$(DIR_CLASS)gestor_mapa_datos_legibles.h \
$(DIR_CLASS)gestor_mapa_datos_legibles.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)gestor_mapa_datos_legibles.cpp -o $(DIR_O)gestor_mapa_datos_legibles.o

$(DIR_O)activable.o :\
$(DIR_CLASS)activable.h \
$(DIR_CLASS)activable.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)activable.cpp -o $(DIR_O)activable.o

$(DIR_O)geometria.o :\
$(DIR_CLASS)geometria.h \
$(DIR_CLASS)geometria.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)geometria.cpp -o $(DIR_O)geometria.o

$(DIR_O)localizador.o: \
$(DIR_CLASS)localizador.cpp \
$(DIR_CLASS)localizador.h
	$(CC) $(CFLAGS) $(DIR_CLASS)localizador.cpp -o $(DIR_O)localizador.o

$(DIR_O)control_salas.o: \
$(DIR_CLASS)control_salas.cpp \
$(DIR_CLASS)control_salas.h
	$(CC) $(CFLAGS) $(DIR_CLASS)control_salas.cpp -o $(DIR_O)control_salas.o

$(DIR_O)control_objetivos.o: \
$(DIR_CLASS)control_objetivos.cpp \
$(DIR_CLASS)control_objetivos.h
	$(CC) $(CFLAGS) $(DIR_CLASS)control_objetivos.cpp -o $(DIR_O)control_objetivos.o

$(DIR_O)control_puzzle.o: \
$(DIR_O)control_pieza_puzzle.o\
$(DIR_CLASS)control_puzzle.cpp \
$(DIR_CLASS)control_puzzle.h
	$(CC) $(CFLAGS) $(DIR_CLASS)control_puzzle.cpp -o $(DIR_O)control_puzzle.o

$(DIR_O)control_pieza_puzzle.o: \
$(DIR_O)utilidades_cargadores.o \
$(DIR_CLASS)control_pieza_puzzle.cpp \
$(DIR_CLASS)control_pieza_puzzle.h
	$(CC) $(CFLAGS) $(DIR_CLASS)control_pieza_puzzle.cpp -o $(DIR_O)control_pieza_puzzle.o


$(DIR_O)utilidades_cargadores.o: \
$(DIR_CLASS)utilidades_cargadores.cpp \
$(DIR_CLASS)utilidades_cargadores.h
	$(CC) $(CFLAGS) $(DIR_CLASS)utilidades_cargadores.cpp -o $(DIR_O)utilidades_cargadores.o

$(DIR_O)tiempo_juego.o: \
$(DIR_CLASS)tiempo_juego.cpp \
$(DIR_CLASS)tiempo_juego.h
	$(CC) $(CFLAGS) $(DIR_CLASS)tiempo_juego.cpp -o $(DIR_O)tiempo_juego.o

$(DIR_O)control_energia.o: \
$(DIR_CLASS)control_energia.cpp \
$(DIR_CLASS)control_energia.h
	$(CC) $(CFLAGS) $(DIR_CLASS)control_energia.cpp -o $(DIR_O)control_energia.o

$(DIR_O)control_habilidades.o: \
$(DIR_CLASS)control_habilidades.cpp \
$(DIR_CLASS)control_habilidades.h
	$(CC) $(CFLAGS) $(DIR_CLASS)control_habilidades.cpp -o $(DIR_O)control_habilidades.o

$(DIR_O)herramientas_graficas.o: \
$(DIR_CLASS)herramientas_graficas.cpp \
$(DIR_CLASS)herramientas_graficas.h
	$(CC) $(CFLAGS) $(DIR_CLASS)herramientas_graficas.cpp -o $(DIR_O)herramientas_graficas.o

$(DIR_O)herramientas_calculos.o: \
$(DIR_CLASS)herramientas_calculos.cpp \
$(DIR_CLASS)herramientas_calculos.h
	$(CC) $(CFLAGS) $(DIR_CLASS)herramientas_calculos.cpp -o $(DIR_O)herramientas_calculos.o

$(DIR_O)representador_tiempo_restante.o: \
$(DIR_CLASS)representador_tiempo_restante.cpp \
$(DIR_CLASS)representador_tiempo_restante.h
	$(CC) $(CFLAGS) $(DIR_CLASS)representador_tiempo_restante.cpp -o $(DIR_O)representador_tiempo_restante.o

$(DIR_O)pantallazo_sistema.o: \
$(DIR_O)herramientas_graficas.o\
$(DIR_CLASS)pantallazo_sistema.cpp \
$(DIR_CLASS)pantallazo_sistema.h
	$(CC) $(CFLAGS) $(DIR_CLASS)pantallazo_sistema.cpp -o $(DIR_O)pantallazo_sistema.o

$(DIR_O)diccionario_info_juego.o: \
$(DIR_CLASS)diccionario_info_juego.cpp \
$(DIR_CLASS)diccionario_info_juego.h
	$(CC) $(CFLAGS) $(DIR_CLASS)diccionario_info_juego.cpp -o $(DIR_O)diccionario_info_juego.o

$(DIR_O)definiciones_serializador_deserializador.o: \
$(DIR_CLASS)definiciones_serializador_deserializador.cpp \
$(DIR_CLASS)definiciones_serializador_deserializador.h
	$(CC) $(CFLAGS) $(DIR_CLASS)definiciones_serializador_deserializador.cpp -o $(DIR_O)definiciones_serializador_deserializador.o

$(DIR_O)serializador.o: \
$(DIR_O)definiciones_serializador_deserializador.o \
$(DIR_O)tiempo_juego.o \
$(DIR_O)control_salas.o \
$(DIR_O)control_objetivos.o \
$(DIR_O)control_puzzle.o \
$(DIR_O)control_energia.o \
$(DIR_O)control_habilidades.o \
$(DIR_O)datos_legibles.o \
$(DIR_O)visitante_serializacion.o \
$(DIR_O)control_actores_persistentes.o \
$(DIR_CLASS)info_juego.h\
$(DIR_CLASS)factoria_actores.h\
$(DIR_CLASS)serializador.cpp \
$(DIR_CLASS)serializador.h
	$(CC) $(CFLAGS) $(DIR_CLASS)serializador.cpp -o $(DIR_O)serializador.o

$(DIR_O)deserializador.o: \
$(DIR_O)definiciones_serializador_deserializador.o \
$(DIR_O)utilidades_cargadores.o \
$(DIR_O)factoria_actores.o \
$(DIR_O)tiempo_juego.o \
$(DIR_O)control_salas.o \
$(DIR_O)control_objetivos.o \
$(DIR_O)control_puzzle.o \
$(DIR_O)control_energia.o \
$(DIR_O)control_habilidades.o \
$(DIR_O)datos_legibles.o \
$(DIR_O)visitante_serializacion.o \
$(DIR_O)control_actores_persistentes.o \
$(DIR_CLASS)info_juego.h\
$(DIR_CLASS)deserializador.cpp \
$(DIR_CLASS)deserializador.h
	$(CC) $(CFLAGS) $(DIR_CLASS)deserializador.cpp -o $(DIR_O)deserializador.o

$(DIR_O)visitante_serializacion.o:\
$(DIR_O)factoria_actores.o\
$(DIR_O)visitante.o\
$(DIR_CLASS)visitante_serializacion.h\
$(DIR_CLASS)visitante_serializacion.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)visitante_serializacion.cpp -o $(DIR_O)visitante_serializacion.o

$(DIR_O)menu_seleccionable.o:\
$(DIR_CLASS)menu_seleccionable.h\
$(DIR_CLASS)menu_seleccionable.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)menu_seleccionable.cpp -o $(DIR_O)menu_seleccionable.o

$(DIR_O)estructura_animacion.o:\
$(DIR_CLASS)estructura_animacion.h\
$(DIR_CLASS)estructura_animacion.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)estructura_animacion.cpp -o $(DIR_O)estructura_animacion.o

$(DIR_O)configuracion.o:\
$(DIR_O)utilidades_cargadores.o\
$(DIR_CLASS)configuracion.h\
$(DIR_CLASS)configuracion.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)configuracion.cpp -o $(DIR_O)configuracion.o

$(DIR_O)cargador_idiomas.o:\
$(DIR_O)utilidades_cargadores.o\
$(DIR_CLASS)cargador_idiomas.h\
$(DIR_CLASS)cargador_idiomas.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)cargador_idiomas.cpp -o $(DIR_O)cargador_idiomas.o

$(DIR_O)item_configuracion.o:\
$(DIR_CLASS)item_configuracion.h\
$(DIR_CLASS)item_configuracion.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)item_configuracion.cpp -o $(DIR_O)item_configuracion.o

#Utilidades...
$(DIR_O)comprobador_rampas.o: \
$(DIR_O)sala.o \
$(DIR_CLASS)utils/comprobador_rampas.h \
$(DIR_CLASS)utils/comprobador_rampas.cpp
	$(CC) $(CFLAGS) $(DIR_CLASS)utils/comprobador_rampas.cpp -o $(DIR_O)comprobador_rampas.o

