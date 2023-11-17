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
			Skeleton(Vector2f pos, Vector2f tam);
			~Skeleton();
			void atacar();
			void setAnimacao(int anim);
			float getVida();
			float getSize();
			void atualizar();

		};

	}
}