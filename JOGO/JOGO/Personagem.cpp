#include "Personagem.hpp"
#include <iostream>
#define GRAVIDADE 0.02f

namespace Entidades
{
	namespace Personagens
	{
		Personagem::Personagem() :
			podeAndar(true),
			direita(true),
			ds(0.0f),
			dt(0.0f),
			ds2(0.0f),
			isJumping(true),
			gravity(GRAVIDADE),
			position(100.0f, 100.0f),
			velocity(0.0f, 0.0f),
			direcao(0.0f, 0.0f),
			velFinal(100.0f, 100.0f),
			Entidade(position, corpo.getSize())
		{

		}

		Personagem::~Personagem()
			= default;

		void Personagem::andar()
		{
			podeAndar = true;
		}

		void Personagem::parar()
		{
			podeAndar = false;
		}

		void Personagem::cair()
		{
			// Gravidade
			if (isJumping) {
				direcao.y += gravity;
				direcao.y += velocity.y;
				if (gravity >= 0.23f)
					gravity = 0.23f;
				else
					gravity += 0.0032f;
			}
			else {
				direcao.y += gravity;
				gravity = GRAVIDADE;
			}

			corpo.move(0.0, direcao.y);
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