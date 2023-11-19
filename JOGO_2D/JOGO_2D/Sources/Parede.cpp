#include "Parede.hpp"
#include <iostream>

namespace Entidades
{
	namespace Obstaculos
	{
		Parede::Parede(const sf::Vector2f pos, const sf::Vector2f tam) :
			Obstaculo(pos, tam)
		{

			corpo = sf::RectangleShape(tam);
			corpo.setPosition(pos);
			corpo.setFillColor(Color::Transparent);

		}

		Parede::~Parede()
		{

		}

		void Parede::executar()
		{
		}

		void Parede::atualizar()
		{

		}
	}
}