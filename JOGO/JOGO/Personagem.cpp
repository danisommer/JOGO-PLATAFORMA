#include "Personagem.hpp"
#include <iostream>
#define GRAVIDADE 0.018f
#define VIDA 100.0f

namespace Entidades
{
	namespace Personagens
	{
		Personagem::Personagem() :
			isJumping(true),
			gravity(GRAVIDADE),
			velocity(0.0f, 0.0f),
			direcao(0.0f, 0.0f),
			Entidade(position, corpo.getSize()),
			voador(false),
			vida(VIDA),
			morto(false),
			animacao(0),
			moveu(true),
			posAnterior(),
			concluida(true),
			dano(),
			inimigo(false)
		{

			if (!healthBarTexture.loadFromFile("Assets/vida.png")) {
				exit(1);
			}

			if (!borderTexture.loadFromFile("Assets/moldura.png")) {
				exit(1);
			}
		}

		Personagem::~Personagem()
			= default;

		void Personagem::cair()
		{
			if (!voador)
			{
				// Gravidade
				if (isJumping)
				{
					direcao.y += gravity;
					direcao.y += velocity.y;
					if (gravity >= 0.18f)
						gravity = 0.18f;
					else
						gravity += 0.005f;
				}
				else
				{
					gravity = GRAVIDADE;
					direcao.y += gravity;

				}

				corpo.move(0.0, direcao.y);
			}

		}

		void Personagem::setIsJumping(bool IJ)
		{
			isJumping = IJ;
		}
		void Personagem::setY(float Y)
		{
			direcao.y = Y;
		}

		void Personagem::setPos(float X, float Y)
		{
			corpo.setPosition(X, Y);
		}

		Vector2f Personagem::getPos()
		{
			return corpo.getPosition();
		}

		void Personagem::tomarDano(float dano)
		{
			if (animacao != 2)
			{
				vida -= dano;
				animacao = 1;
			}

		}

		void Personagem::morrer()
		{
			morto = true;
		}

		bool Personagem::getMorte()
		{
			return morto;
		}

		bool Personagem::getMoveu()
		{
			Vector2f posAtual = corpo.getPosition();

			if (posAtual.x != posAnterior.x || posAtual.y != posAnterior.y)
				moveu = true;
			else
				moveu = false;

			posAnterior = posAtual;

			return moveu;
		}

		void Personagem::atualizarBarraVida()
		{
			float vidaMax = getVida();

			if (vida >= 0.0f)
			{
				healthBar.setTexture(healthBarTexture);
				border.setTexture(borderTexture);

				sf::Vector2f pos = corpo.getPosition();
				pos.x -= 25.0f;
				pos.y -= 25.0f;

				healthBar.setPosition(pos.x, pos.y);
				border.setPosition(pos.x, pos.y);

				if (inimigo)
				{
					if (concluida)
						healthBar.setScale((vida / vidaMax) * 0.2f, 0.2f);
				}
				else
				{
					healthBar.setScale((vida / vidaMax) * 0.2f, 0.2f);
				}

				border.setScale(0.2f, 0.2f);
			}
		}

		Sprite Personagem::getBorder()
		{
			if (animacao != 2)
				return border;
			else
				return Sprite();
		}

		Sprite Personagem::getHealthBar()
		{
			if (animacao != 2)
				return healthBar;
			else
				return Sprite();
		}

	}

}
