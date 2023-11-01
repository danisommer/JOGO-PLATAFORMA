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
			float jumpForce;
			bool andando;
			bool direita;
			bool pular;
			int cont;


		public:
			Jogador(Vector2f pos, Vector2f tam);
			~Jogador();
			RectangleShape getCorpo();
			void atualizar();

		};
	}
	
}



