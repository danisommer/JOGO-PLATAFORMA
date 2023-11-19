#pragma once 

#include "Lista.hpp"
#include "Entidade.hpp"

namespace Gerenciadores
{
	class Gerenciador_Grafico;
}

namespace Lista
{
	class ListaEntidade
	{
	private:
		Lista<Entidades::Entidade> LEs;

	public:
		ListaEntidade();
		~ListaEntidade();
		void addEntidade(Entidades::Entidade* entidade);
		void removerEntidade(Entidades::Entidade* entidade);
		void removerEntidade(int pos);
		const int getTam() const;
		Entidades::Entidade* operator[](int pos);
		void executar(sf::RenderWindow* window);
		void limpar();
		void desenharEntidades(Gerenciadores::Gerenciador_Grafico* pGG);
	};
}