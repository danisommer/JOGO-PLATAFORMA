#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include "Entidade.hpp"

namespace Lista
{
	// Container que POSSUI as entidades. A propriedade e exclusiva:
	// remover uma entidade ou limpar a lista destroi a entidade.
	// Demais sistemas (colisoes, combate) guardam apenas ponteiros
	// observadores e nunca devem deletar.
	class ListaEntidade
	{
	private:
		std::vector<std::unique_ptr<Entidades::Entidade>> entidades;

	public:
		ListaEntidade() = default;
		~ListaEntidade() = default;

		ListaEntidade(const ListaEntidade&) = delete;
		ListaEntidade& operator=(const ListaEntidade&) = delete;
		ListaEntidade(ListaEntidade&&) = default;
		ListaEntidade& operator=(ListaEntidade&&) = default;

		// Assume a posse do ponteiro recebido.
		void addEntidade(Entidades::Entidade* entidade);
		void removerEntidade(Entidades::Entidade* entidade);
		void removerEntidade(int pos);
		int getTam() const;
		Entidades::Entidade* operator[](int pos);
		void limpar();
	};
}
