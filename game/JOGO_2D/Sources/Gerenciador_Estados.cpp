#include "Gerenciador_Estados.hpp"

namespace Gerenciadores
{
	void Gerenciador_Estados::empilhar(std::unique_ptr<Estados::Estado> estado)
	{
		pendentes.push_back({ TipoTransicao::Empilhar, std::move(estado) });
	}

	void Gerenciador_Estados::desempilhar()
	{
		pendentes.push_back({ TipoTransicao::Desempilhar, nullptr });
	}

	void Gerenciador_Estados::trocar(std::unique_ptr<Estados::Estado> estado)
	{
		pendentes.push_back({ TipoTransicao::Trocar, std::move(estado) });
	}

	bool Gerenciador_Estados::vazio() const
	{
		return pilha.empty();
	}

	void Gerenciador_Estados::aplicarTransicoes()
	{
		for (auto& t : pendentes)
		{
			switch (t.tipo)
			{
			case TipoTransicao::Empilhar:
				pilha.push_back(std::move(t.estado));
				break;
			case TipoTransicao::Desempilhar:
				if (!pilha.empty())
					pilha.pop_back();
				break;
			case TipoTransicao::Trocar:
				if (!pilha.empty())
					pilha.pop_back();
				pilha.push_back(std::move(t.estado));
				break;
			}
		}
		pendentes.clear();
	}

	void Gerenciador_Estados::executarQuadro()
	{
		if (!pilha.empty())
		{
			Estados::Estado* topo = pilha.back().get();
			topo->processarEventos();
			topo->atualizar();
			topo->desenhar();
		}

		aplicarTransicoes();
	}
}
