#pragma once
#include "Inimigo.hpp"
#include "vector"

namespace Entidades
{
	namespace Personagens
	{
		class Skeleton : public Inimigo
		{
		private:
			void inicializaAnimacoes();

		public:
			Skeleton(Vector2f pos, Vector2f tam, Jogador* jogador);
			~Skeleton();
			void atualizarAnimacao();
			void atacar();

		};

	}
}