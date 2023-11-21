#include "../Headers/Plataforma.hpp"
#include <iostream>

namespace Entidades
{
    namespace Obstaculos
    {
        Plataforma::Plataforma(const sf::Vector2f p, const sf::Vector2f tam, bool g) :
            Obstaculo(pos, tam),
            fase(),
            pos(p)
        {
            grande = g;
            corpo = sf::RectangleShape(tam);
            corpo.setPosition(pos);
            corpo.setFillColor(Color::Red);
        }

        Plataforma::~Plataforma()
        {
        }

        void Plataforma::atualizar()
        {

        }

        void Plataforma::danar(int jogador)
        {
        }

        void Plataforma::setFase(int f)
        {
            fase = f;

            if (!grande) {
                if (!textura.loadFromFile("Assets/Cenario/plataformaPequena.png")) {
                    exit(1);
                }
                sprite.setTexture(textura);
                sprite.setPosition(Vector2f(pos.x - 102.0f, pos.y - 75.0f));
                sprite.setScale(0.5f, 0.31f);
            }
            else
            {
                if (!textura.loadFromFile("Assets/Cenario/plataformaGrande_" + std::to_string(fase) + ".png")) {
                    exit(1);
                }
                sprite.setTexture(textura);
                sprite.setPosition(Vector2f(pos.x, pos.y - 550.0f));
                sprite.setScale(1.0f, 1.0f);
            }
        }
    }
}