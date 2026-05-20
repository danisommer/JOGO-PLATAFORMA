#include "Inimigo.hpp"
#include <iostream>

namespace Entidades
{
	namespace Personagens
	{
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
			ALCANCE_X(800.0f),
			ALCANCE_Y(500.0f),
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
			// O inimigo so fica "parado" (sem agir) durante o hit-stun
			// breve que segue um golpe - nao mais enquanto a vida muda.
			// Assim, sob ataque continuo, ele alterna dano -> contra-ataque
			// -> dano, em vez de morrer preso no loop de tomar dano.
			parado = estaAtordoado();

			if (parado)
				animacao = 1;

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

			animacaoAtual->aplicar(sprite, count);
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

		bool Inimigo::podeAtacarAlvo(const sf::RectangleShape& alvo) const
		{
			const sf::FloatRect rA = alvo.getGlobalBounds();
			const sf::FloatRect rE = corpo.getGlobalBounds();

			// Alcance horizontal: a distancia entre o centro do inimigo
			// e o centro do alvo precisa caber dentro de
			// distanciaAtaqueX + metade da largura do alvo. Assim a
			// hitbox do golpe sai a partir do corpo do inimigo, e nao
			// do "centro do mundo" como antes.
			const float centroAlvoX = rA.left + rA.width / 2.0f;
			const float centroInimigoX = rE.left + rE.width / 2.0f;
			const float deltaX = std::fabs(centroAlvoX - centroInimigoX);
			const float alcanceX = distanciaAtaqueX + rA.width / 2.0f;

			if (deltaX > alcanceX)
				return false;

			// Alinhamento vertical: para inimigos terrestres o ataque
			// so faz sentido quando os "pes" (base do corpo) estao
			// aproximadamente no mesmo nivel - assim eles nao atacam o
			// jogador estando em cima ou embaixo de uma plataforma. Para
			// voadores, usamos a sobreposicao entre as caixas verticais
			// (mais permissiva, mas exige aproximacao real).
			if (voador)
			{
				const float centroAlvoY = rA.top + rA.height / 2.0f;
				const float centroInimigoY = rE.top + rE.height / 2.0f;
				return std::fabs(centroAlvoY - centroInimigoY)
					<= (distanciaAtaqueY + rA.height / 2.0f);
			}

			const float baseAlvo = rA.top + rA.height;
			const float baseInimigo = rE.top + rE.height;
			// Tolerancia pequena (15 px) compensa diferencas finas
			// entre alturas de personagens e atrito de pulo curto, sem
			// deixar o inimigo atacar por cima/baixo de plataformas.
			return std::fabs(baseAlvo - baseInimigo) <= 15.0f;
		}
	}


}
