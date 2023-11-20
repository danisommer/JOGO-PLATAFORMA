#pragma once
#include "Obstaculo.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Parede : public Obstaculo
		{
		private:
			sf::Texture textura;

		public:
			Parede(const Vector2f pos, const Vector2f tam);
			~Parede();
			void atualizar();
			void danar(int jogador);

		};
	}
}