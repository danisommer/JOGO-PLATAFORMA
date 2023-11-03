#pragma once
#include "Inimigo.hpp"
#include "vector"

namespace Entidades
{
	namespace Personagens
	{
		class Mushroom : public Inimigo
		{
		private:
			void inicializaAnimacoes();

		public:
			Mushroom(Vector2f pos, Vector2f tam, Jogador* jogador);
			~Mushroom();
			void atacar();
			void setAnimacao(int anim);


		};

	}
}