#include "Entidade.hpp"

namespace Entidades
{
	Entidade::Entidade(const Vector2f pos, const Vector2f tam):
		Ente()
	{
		id = 1;
	}

	Entidade::~Entidade()
	{
	}

	RectangleShape* Entidade::getCorpo()
	{
		return &corpo;
	}	

	Vector2f Entidade::getPos()
	{
		return corpo.getPosition();
	}
}
