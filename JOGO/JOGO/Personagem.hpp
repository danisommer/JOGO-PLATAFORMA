#pragma once
#include <SFML/Graphics.hpp>
#include "Entidade.hpp"

using namespace sf;

namespace Entidades
{
	namespace Personagens
	{
		class Personagem : public Entidade
		{
		protected:
			Vector2f vel;
			Vector2f velFinal;
			Vector2f direcao;
			Vector2f position;
			Vector2f velocity;
			Clock relogio;
			bool podeAndar;
			bool isJumping;
			bool direita;
			float dt;
			float ds;
			float ds2;
			float gravity;

		public:
			Personagem();
			~Personagem();
			virtual void atualizar() = 0;

			void andar();
			void parar();
			void cair();
			void setIsJumping(bool IJ);
			void setY(float Y);
			void setPos(float X, float Y);
			Vector2f getPos();
		};
	}
	
}


