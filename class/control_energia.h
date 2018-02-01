#ifndef CONTROL_ENERGIA_H
#define CONTROL_ENERGIA_H

#include <vector>

/*Cada tanque de energía tiene una cantidad dentro entre 0 y MAX_CANTIDAD. Una 
habilidad puede consumir un tanque entero o una cantidad por segundo. Cuando un 
tanque se vacía más allá de MAX_CANTIDAD y deja de usarse pasa al estado de 
recuperación, en el que no se puede usar hasta que ha recuperado toda la 
cantidad.

Para habilidades automáticas que gastan cantidades de energía en concreto habría
un tiempo intermedio entre que se gasta y se puede usar de nuevo y ya empieza
a recargarse y no puede usarse.
*/

class Control_energia
{
	public:

	struct Info_tanque
	{
		float cantidad;
		bool usable;

		Info_tanque(float c, bool u): cantidad(c), usable(u){}
	};

	private:
	
	class Tanque
	{
		enum estados{E_DISPONIBLE, //El tanque está disponible para su uso.
//			E_INTERMEDIO, 		//El tanque ha sido consumido pero no al 100, puede usarse de nuevo. Si no se usa en N tiempo, pasa a recarga.
			E_ENFRIAMIENTO,		//El tanque se ha vaciado y hay que esperar para que se recargue.
			E_CONSUMO,		//El tanque se está consumiendo.
			E_RECARGA		//El tanque está recargandose y no es usable.
		};

		static const float CANTIDAD_RECUPERADA_POR_SEGUNDO; //A cada segundo se recupera esta cantidad.
		static const float SEGUNDOS_INTERMEDIO;	//Este es el tiempo entre consumo y que empiece la recarga.
		static const float MAX_CANTIDAD; //Cantidad máxima que puede contener un tanque.

		float cantidad;
		float consumo;
		unsigned int estado;
		float tiempo;

		public:

//		float acc_cantidad() const {return cantidad;}
		bool es_consumiendo() const {return estado==E_CONSUMO;}
		bool es_usable() const {return estado==E_DISPONIBLE;}
		bool es_lleno() const {return cantidad==MAX_CANTIDAD;}
//		bool hay_reserva(float c) const {return cantidad >= c;}

		void iniciar_consumo(float c) 
		{
			consumo=c;
			estado=E_CONSUMO;
		}
		void turno(float delta, bool=false);
		void vaciar();
//		void consumir(float);
		void rellenar() 
		{
			cantidad=MAX_CANTIDAD;
			estado=E_DISPONIBLE;
		}

		Info_tanque como_info_tanque() const {return Info_tanque(cantidad, es_usable());}

		Tanque():cantidad(MAX_CANTIDAD), estado(E_DISPONIBLE), tiempo(0.0)
		{}
	};

	std::vector<Tanque> tanques;

	public:

//	bool hay_reserva(float cantidad) const; //Indica si al menos hay la cantidad especificado.
	bool hay_tanques_completos(unsigned int) const; //Indica si hay al menos un tanque completo.
	void gastar_completo(unsigned int); //Gasta N tanques completo disponible.
//	bool gastar(float); //Gasta del primer tanque disponible.
	void iniciar_gasto(float);
	void nuevo_tanque(); //Inserta un nuevo tanque.
	void turno(float, bool=false); //Pasa el turno para cada tanque.
	void rellenar();
	void reiniciar();
	bool es_consumiendo() const;
	unsigned int obtener_cantidad_tanques() const {return tanques.size();}
	std::vector<Info_tanque> obtener_info_tanques() const;
	
	//static float calcular_ratio_gasto(float, float);

	Control_energia();
};

#endif
