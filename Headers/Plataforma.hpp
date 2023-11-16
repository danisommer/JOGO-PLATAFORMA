#pragma once
#include "Entidade.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Plataforma : public Entidade
		{
		private:
			sf::Texture textura;

		public:
			Plataforma(const Vector2f pos, const Vector2f tam, bool grande);
			~Plataforma();
			void atualizar();
		};
	}
}
