#pragma once
#include "Inimigo.hpp"
#include "vector"

namespace Entidades
{
	namespace Personagens
	{
		class OlhoVoador : public Inimigo
		{
		private:

			void inicializaAnimacoes();
			float forcaLentidao;
			int tempoLentidao;
			float forcaPulo;

		public:
			OlhoVoador(Vector2f pos, Vector2f tam);
			~OlhoVoador();
			void atacar(int jogador);
			void setAnimacao(int anim);
			float getVida();
			float getSize();
			void atualizar();
		};

	}
}