#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

namespace Entidades
{
	class Entidade
	{
	protected:
		Sprite sprite;
		RectangleShape corpo;

	public:
		Entidade(const Vector2f pos, const Vector2f tam);
		virtual ~Entidade();
		const RectangleShape getCorpo() const;
		const Sprite getSprite() const;
		Vector2f getPos();
	};
}



