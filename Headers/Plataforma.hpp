#pragma once
#include "Obstaculo.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Plataforma : public Obstaculo
		{
		private:
			sf::Texture textura;

		public:
			Plataforma(const Vector2f pos, const Vector2f tam, bool grande);
			~Plataforma();
			void atualizar();
			void danar(int jogador);

		};
	}
}