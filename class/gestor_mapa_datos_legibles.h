#ifndef GESTOR_MAPA_DATOS_LEGIBLES_H
#define GESTOR_MAPA_DATOS_LEGIBLES_H

#include <map>
#include <vector>

template<class C, class V>
class Gestor_mapa
{
	protected:
	std::map<C, V> mapa;

	public:	

	Gestor_mapa(){}
	virtual ~Gestor_mapa() {}
	bool existe_entrada_por_id(C pid) const {return mapa.count(pid);}
	const V& obtener_por_id(C pid) const {return mapa.at(pid);}
	void insertar(C id, V datos){mapa.insert(std::pair<C, V>(id, datos));}
	unsigned int size() const {return mapa.size();}
	const std::map<C, V>& acc_mapa() const {return mapa;}
	void vaciar() {mapa.clear();}
	std::vector<C> obtener_vector_claves() const 
	{
		std::vector<C> resultado;
		auto it=mapa.begin();
		while(it != mapa.end())
		{
			resultado.push_back(it->first);
			++it;
		}
		return resultado;
	}
};
#endif
