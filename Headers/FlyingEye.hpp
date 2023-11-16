#pragma once
#include "Inimigo.hpp"
#include "vector"

namespace Entidades
{
	namespace Personagens
	{
		class FlyingEye : public Inimigo
		{
		private:

			void inicializaAnimacoes();

		public:
			FlyingEye(Vector2f pos, Vector2f tam);
			~FlyingEye();
			void atacar();
			void setAnimacao(int anim);


		};

	}
}