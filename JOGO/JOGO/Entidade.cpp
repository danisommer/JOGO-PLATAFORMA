#include "Entidade.hpp"

namespace Entidades
{

	Entidade::Entidade(const Vector2f pos, const Vector2f tam)
	{
	}

	Entidade::~Entidade()
	{
	}

	const RectangleShape Entidade::getCorpo() const
	{
		return corpo;
	}

}
