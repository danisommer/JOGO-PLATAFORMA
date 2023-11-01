#include "Inimigo.hpp"

namespace Entidades
{
	namespace Personagens
	{
		Inimigo::Inimigo(Vector2f pos, Vector2f	tam, Jogador* jogador) :
			jogador(jogador), iteracoes(0)
		{
			corpo = RectangleShape(tam);
			corpo.setPosition(pos);

			std::random_device rd;
			std::mt19937 gen(rd());

			int min_value = 0;
			int max_value = 2;

			std::uniform_int_distribution<int> distribution(min_value, max_value);

			random_number = distribution(gen);


			inicializa();


		}

		Inimigo::~Inimigo() = default;

		void Inimigo::inicializa()
		{
			vel = Vector2f(0.25f, 0.25f);
			corpo.setFillColor(sf::Color::Red);

		}

		void Inimigo::perseguirJogador(Vector2f posJogador, Vector2f posInimigo)
		{
			if (posJogador.x - posInimigo.x > 0.0f)
			{
				corpo.move(vel.x, 0.0f);
			}
			else
			{
				corpo.move(-vel.x, 0.0f);
			}

		}

		void Inimigo::moveAleatorio()
		{
			if (iteracoes >= 600)
			{
				std::random_device rd;
				std::mt19937 gen(rd());

				int min_value = 0;
				int max_value = 2;

				std::uniform_int_distribution<int> distribution(min_value, max_value);

				random_number = distribution(gen);

				iteracoes = 0;
			}


			sf::Vector2f direcao(0.0f, 0.0f);

			if (random_number == 0)
				direcao.x = -vel.x;
			else if (random_number == 1)
				direcao.x = vel.x;

			iteracoes++;

			corpo.move(direcao);
		}



		void Inimigo::atualizar()
		{
			Vector2f posJogador = jogador->getCorpo().getPosition();
			Vector2f posInimigo = corpo.getPosition();

			if (fabs(posJogador.x - posInimigo.x) <= ALCANCE_X && fabs(posJogador.y - posInimigo.y) <= ALCANCE_Y)
			{
				perseguirJogador(posJogador, posInimigo);
			}
			else
			{
				moveAleatorio();
			}

		}
	}


}
