#pragma once
#include "Entidade.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Parede : public Entidade
		{
		private:
			sf::Texture textura;

		public:
			Parede(const Vector2f pos, const Vector2f tam);
			~Parede();
			void atualizar();
		};
	}
}