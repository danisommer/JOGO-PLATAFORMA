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

		void Parede::atualizar()
		{
		}

		void Parede::danar(int jogador)
		{

		}
		void Parede::limparArquivo(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_parede.txt", std::ofstream::out);

			if (arquivo.is_open())
			{
				arquivo.close();
			}
		}
		void Parede::salvar(int save)
		{
			std::ofstream arquivo("Saves/save" + to_string(save) + "_parede.txt", std::ios::app);

			if (arquivo.is_open())
			{
				arquivo << "Posicao: " << corpo.getPosition().x << " " << corpo.getPosition().y << "\n";

				arquivo.close();
			}
		}
	}
}