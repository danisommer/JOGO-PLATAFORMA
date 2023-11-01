#include "Plataforma.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		Plataforma::Plataforma(const Vector2f pos, const Vector2f tam) :
			Entidade(pos, tam)
		{

			corpo = RectangleShape(tam);
			corpo.setPosition(pos);
			corpo.setFillColor(sf::Color::Blue);

		}
	}
}