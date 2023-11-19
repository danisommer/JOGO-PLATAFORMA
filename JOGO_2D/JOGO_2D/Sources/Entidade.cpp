#include "Entidade.hpp"

namespace Entidades
{
	Entidade::Entidade(const Vector2f pos, const Vector2f tam)
	{
	}

	Entidade::Entidade()
	{
	}
	
	Entidade::~Entidade()
	{
	}

	const RectangleShape Entidade::getCorpo() const
	{
		return corpo;
	}	
	const Sprite Entidade::getSprite() const
	{
		return sprite;
	}
	Vector2f Entidade::getPos()
	{
		return corpo.getPosition();
	}
}