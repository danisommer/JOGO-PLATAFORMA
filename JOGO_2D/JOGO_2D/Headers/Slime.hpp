#pragma once
#include "Obstaculo.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Slime : public Obstaculo
		{
		private:
			sf::Texture textura;
			float pegajosidadePulo;
			float pegajosidadeMovimento;

		public:
			Slime(const Vector2f pos, const Vector2f tam);
			~Slime();
			void atualizar();
			void danar(int jogador);

		};
	}
}