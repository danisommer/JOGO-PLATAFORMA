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
			Goblin(Vector2f pos, Vector2f tam);
			~Goblin();
			void atacar();
			void setAnimacao(int anim);
			float getVida();
			float getSize();
			void atualizar();
		};

	}
}