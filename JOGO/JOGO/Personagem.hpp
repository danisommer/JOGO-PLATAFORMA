#pragma once
#include <SFML/Graphics.hpp>
#include "Entidade.hpp"
#include "Animacao.hpp"

using namespace sf;

namespace Entidades
{
	namespace Personagens
	{
		class Personagem : public Entidade
		{
		protected:
			Vector2f vel;
			Vector2f direcao;
			Vector2f position;
			Vector2f velocity;
			Clock relogio;
			bool isJumping;
			bool voador;
			float dt;
			float ds;
			float gravity;

		public:
			Personagem();
			~Personagem();
			virtual void atualizar() = 0;

			void cair();
			void setIsJumping(bool IJ);
			void setY(float Y);
			void setPos(float X, float Y);
			Vector2f getPos();
			virtual void setAnimacao(int anim) = 0;
		};
	}

}


