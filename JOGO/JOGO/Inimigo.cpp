#include "Inimigo.hpp"

namespace Entidades
{
	namespace Personagens
	{

		Inimigo::Inimigo(Vector2f pos, Vector2f	tam, Jogador* jogador) :
			jogador(jogador), 
			iteracoes(0),
			Personagem(),
			direita(true),
			distanciaAlvo(50.0f),
			animacaoAtual(nullptr),
			n_frames(0),
			count(0),
			lado(0),
			animacao(0),
			anterior(0)
		{
			corpo.setSize(tam);
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
			corpo.setFillColor(sf::Color::Transparent);

		}

		void Inimigo::perseguirJogador(Vector2f posJogador, Vector2f posInimigo)
		{
			distanciaX = posJogador.x - posInimigo.x;
			distanciaY = posJogador.y - posInimigo.y;

			if (fabs(distanciaX) > distanciaAlvo)
			{
				if (distanciaX > 0.0f)
				{
					corpo.move(vel.x, 0.0f);
					direita = true;
				}
				else
				{
					corpo.move(-vel.x, 0.0f);
					direita = false;
				}

				if (voador)
				{
					if (distanciaY > 30.0f)
					{
						corpo.move(0.0f, vel.y);
					}
					else if (distanciaY < -30.0f)
					{
						corpo.move(0.0f, -vel.y);
					}
				}

				animacao = 0;
			}
			else
			{
				if (fabs(distanciaY) <= 30.0f)
				{
					animacao = 3;
					atacar();
				}
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
			{
				direcao.x = -vel.x;
				direita = false;
			}
			else if (random_number == 1) 
			{
				direcao.x = vel.x;
				direita = true;
			}
				
			iteracoes++;

			corpo.move(direcao);
		}

		void Inimigo::atualizar()
		{
			static sf::Vector2f posAnteriorInimigo = corpo.getPosition();

			Vector2f posJogador = jogador->getCorpo().getPosition();
			Vector2f posInimigo = corpo.getPosition();

			float distanciaPercorrida = sqrt(pow(posInimigo.x - posAnteriorInimigo.x, 2) + pow(posInimigo.y - posAnteriorInimigo.y, 2));

			posAnteriorInimigo = posInimigo;

			if (fabs(posJogador.x - posInimigo.x) <= ALCANCE_X && fabs(posJogador.y - posInimigo.y) <= ALCANCE_Y)
			{
				perseguirJogador(posJogador, posInimigo);

				if (distanciaPercorrida < DISTANCIA_MINIMA_ANIMACAO)
				{
					animacao = 1;
				}
			}
			else
			{
				moveAleatorio();
			}
			atualizarAnimacao();
		}



		void Inimigo::setAnimacao(int anim)
		{
		}
	}


}
