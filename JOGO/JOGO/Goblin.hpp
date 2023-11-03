#pragma once
#include "Inimigo.hpp"
#include "vector"

namespace Entidades
{
	namespace Personagens
	{
		class Goblin : public Inimigo
		{
		private:
			void inicializaAnimacoes();

		public:
			Goblin(Vector2f pos, Vector2f tam, Jogador* jogador);
			~Goblin();
			void atualizarAnimacao();
			void atacar();

		};

	}
}