#include "Personagem.hpp"
#include <iostream>
#define GRAVIDADE 0.018f

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
			vida(100.0f),
			morto(false),
			animacao(0),
			moveu(true),
			posAnterior()
		{

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
					if (gravity >= 0.205f)
						gravity = 0.205f;
					else
						gravity += 0.00195f;
				}
				else
				{
					direcao.y += gravity;
					gravity = GRAVIDADE;
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
			vida -= dano;
			animacao = 1;
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

	}

}
