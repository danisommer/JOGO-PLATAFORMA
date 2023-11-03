#include "Personagem.hpp"
#include <iostream>
#define GRAVIDADE 0.018f

namespace Entidades
{
	namespace Personagens
	{
		Personagem::Personagem() :
			ds(0.0f),
			dt(0.0f),
			isJumping(true),
			gravity(GRAVIDADE),
			velocity(0.0f, 0.0f),
			direcao(0.0f, 0.0f),
			Entidade(position, corpo.getSize()),
			voador(false)
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
					if (gravity >= 0.22f)
						gravity = 0.22f;
					else
						gravity += 0.0027f;
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

	}

}
