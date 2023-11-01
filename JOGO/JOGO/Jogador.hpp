#pragma once
#include "Personagem.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

namespace Entidades
{
	namespace Personagens
	{
		class Jogador : public Personagem
		{
		private:
			float aceleracao;
			float desaceleracao;
			float velocidadeMaxima;
			float jumpStrength;


		public:
			Jogador(Vector2f pos, Vector2f tam);
			~Jogador();
			void atualizar();

		};
	}

}



