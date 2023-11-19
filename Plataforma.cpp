#include "Plataforma.hpp"
#include <iostream>

namespace Entidades
{
    namespace Obstaculos
    {
        Plataforma::Plataforma(const sf::Vector2f pos, const sf::Vector2f tam, bool grande) :
            Obstaculo(pos, tam)
        {

            corpo = sf::RectangleShape(tam);
            corpo.setPosition(pos);
            corpo.setFillColor(Color::Red);

            if (!grande) {
                if (!textura.loadFromFile("Assets/Cenario/plataforma.png")) {
                    exit(1);
                }
                sprite.setTexture(textura);
                sprite.setPosition(Vector2f(pos.x - 102.0f, pos.y - 75.0f));
                sprite.setScale(0.5, 0.31);
            }
            else
            {
                if (!textura.loadFromFile("Assets/Background/Battleground1/stones&grass.png")) {
                    exit(1);
                }
                sprite.setTexture(textura);
                sprite.setPosition(Vector2f(pos.x, pos.y - 525.0f));
                sprite.setScale(1.0f, 1.0f);
            }
        }
        Plataforma::~Plataforma()
        {
        }
        void Plataforma::executar()
        {
        }
        void Plataforma::atualizar()
        {

        }
    }
}