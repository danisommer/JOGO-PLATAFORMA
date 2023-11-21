#include "../Headers/Inimigo.hpp"
#include <iostream>

namespace Entidades
{
	namespace Personagens
	{
		Jogador* Inimigo::jogador1 = nullptr;
		Jogador* Inimigo::jogador2 = nullptr;

		Inimigo::Inimigo(Vector2f pos, Vector2f	tam) :
			iteracoes(0),
			Personagem(),
			direita(true),
			distanciaAlvo(40.0f),
			animacaoAtual(nullptr),
			n_frames(0),
			count(0),
			lado(0),
			anterior(0),
			atualizacao(0),
			posAnteriorInimigo(0.0f),
			ALCANCE_X(500.0f),
			ALCANCE_Y(300.0f),
			parado(false),
			distanciaAtaqueX(60.0f),
			distanciaAtaqueY(30.0f),
			teleportando(false)
		{
			inimigo = true;
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
				animacao = 0;
			}

			if (voador)
			{
				if (fabs(distanciaY) > distanciaAlvo)
				{


					if (distanciaY > 10.0f)
					{
						corpo.move(0.0f, vel.y);
					}
					else if (distanciaY < -10.0f)
					{
						corpo.move(0.0f, -vel.y);
					}

				}
			}
		}

		void Inimigo::atualizarAnimacao()
		{
			if (vida != vidaAnterior)
			{
				animacao = 1;
				parado = true;
			}
			else
			{
				parado = false;
			}

			vidaAnterior = vida;

			if (animacao != anterior)
			{
				count = 0;
				iteracoes = 0;
			}

			anterior = animacao;

			animacaoAtual = &animacoes[animacao];

			n_frames = animacaoAtual->getNumFrames();

			static sf::Clock clock;
			sf::Time elapsed = clock.getElapsedTime();

			if (iteracoes > animacaoAtual->getAnimationSpeed())
			{
				if (count < n_frames - 1)
				{
					concluida = false;
					count++;
				}
				else
				{
					concluida = true;
					count = 0;

					if (animacao == 2)
						morrer();

					if (animacao == 5)
						teleportando = false;
				}

				iteracoes = 0;
			}
			iteracoes++;


			int lado;

			if (direita)
				lado = 1;
			else
				lado = -1;

			sprite.setTexture(animacaoAtual->getFrame(count));
			sprite.setScale(getSize() * lado, getSize());
			sprite.setPosition(corpo.getPosition().x + 20.0f, corpo.getPosition().y - 10.0f);
		}

		void Inimigo::moveAleatorio()
		{
			if (atualizacao >= 600)
			{
				std::random_device rd;
				std::mt19937 gen(rd());

				int min_value = 0;
				int max_value = 3;

				std::uniform_int_distribution<int> distribution(min_value, max_value);

				random_number = distribution(gen);

				atualizacao = 0;
			}

			sf::Vector2f direcao(0.0f, 0.0f);

			if (random_number == 0)
			{
				direcao.x = -vel.x;
				direita = false;
				animacao = 0;
			}
			else if (random_number == 1)
			{
				direcao.x = vel.x;
				direita = true;
				animacao = 0;
			}
			else
			{
				animacao = 4;
			}

			atualizacao++;

			corpo.move(direcao);
		}

		void Inimigo::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}
		void Inimigo::setJogador(Jogador* j)
		{
			jogador1 = j;
		}
		void Inimigo::setJogador2(Jogador* j)
		{
			jogador2 = j;

		}
		const bool Inimigo::getVoador() const
		{
			return voador;
		}
		const bool Inimigo::getParado() const
		{
			return parado;
		}
		const float Inimigo::getDistanciaAtaqueX() const
		{
			return distanciaAtaqueX;
		}
		const float Inimigo::getDistanciaAtaqueY() const
		{
			return distanciaAtaqueY;
		}
	}


}
