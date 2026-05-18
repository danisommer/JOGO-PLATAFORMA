#include "ListaEntidade.hpp"

namespace Lista
{
	void ListaEntidade::addEntidade(Entidades::Entidade* entidade)
	{
		if (entidade)
			entidades.emplace_back(entidade);
	}

	void ListaEntidade::removerEntidade(Entidades::Entidade* entidade)
	{
		if (!entidade)
			return;

		entidades.erase(
			std::remove_if(entidades.begin(), entidades.end(),
				[entidade](const std::unique_ptr<Entidades::Entidade>& e)
				{
					return e.get() == entidade;
				}),
			entidades.end());
	}

	void ListaEntidade::removerEntidade(int pos)
	{
		if (pos >= 0 && pos < static_cast<int>(entidades.size()))
			entidades.erase(entidades.begin() + pos);
	}

	int ListaEntidade::getTam() const
	{
		return static_cast<int>(entidades.size());
	}

	Entidades::Entidade* ListaEntidade::operator[](int pos)
	{
		if (pos < 0 || pos >= static_cast<int>(entidades.size()))
			return nullptr;

		return entidades[pos].get();
	}

	void ListaEntidade::limpar()
	{
		entidades.clear();
	}
}
