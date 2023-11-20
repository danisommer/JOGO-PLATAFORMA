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
			float forcaLentidao;
			int tempoLentidao;
			float forcaPulo;

		public:
			FlyingEye(Vector2f pos, Vector2f tam);
			~FlyingEye();
			void atacar(int jogador);
			void setAnimacao(int anim);
			float getVida();
			float getSize();
			void atualizar();
		};

	}
}