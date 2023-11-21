#include "../Headers/Slime.hpp"

#include <iostream>

namespace Entidades
{
	namespace Obstaculos
	{
		Slime::Slime(const sf::Vector2f pos, const sf::Vector2f tam) :
			Obstaculo(pos, tam),
			pegajosidadePulo(-0.13f),
			pegajosidadeMovimento(0.3f)
		{
			danoso = true;
			corpo = sf::RectangleShape(tam);
			corpo.setPosition(pos);
			corpo.setFillColor(Color::Red);


			if (!textura.loadFromFile("Assets/Cenario/slime.png")) {
				exit(1);
			}
			sprite.setTexture(textura);
			sprite.setPosition(Vector2f(pos.x - 102.0f, pos.y - 75.0f));
			sprite.setScale(0.5f, 0.31f);


		}
		Slime::~Slime()
		{
		}

		void Slime::atualizar()
		{

		}

		void Slime::danar(int jogador)
		{
			if (jogador == 1)
			{
				jogador1->setLento(true, 300, pegajosidadeMovimento, pegajosidadePulo);
			}
			else if (jogador == 2)
			{
				jogador2->setLento(true, 300, pegajosidadeMovimento, pegajosidadePulo);

			}

		}
	}
}