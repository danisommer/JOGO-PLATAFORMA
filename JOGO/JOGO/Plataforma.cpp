#include "Plataforma.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
        Plataforma::Plataforma(const sf::Vector2f pos, const sf::Vector2f tam) :
            Entidade(pos, tam)
        {

            if (!textura.loadFromFile("Assets/plataforma.png")) {
                exit(1);
            }

            corpo = sf::RectangleShape(tam);
            corpo.setPosition(pos);
            corpo.setFillColor(Color::Transparent);
            //corpo.setTextureRect(sf::IntRect(0, 0, tam.x, tam.y));

            sprite.setTexture(textura);
            sprite.setPosition(Vector2f(pos.x - 102.0f, pos.y - 75.0f));
            sprite.setScale(0.5, 0.31);
        }
	}
}
