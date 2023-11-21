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
		Entidade();
		virtual ~Entidade();
		RectangleShape* getCorpo();
		const Sprite getSprite() const;
		virtual void atualizar() = 0;
		Vector2f getPos();
	};
}



