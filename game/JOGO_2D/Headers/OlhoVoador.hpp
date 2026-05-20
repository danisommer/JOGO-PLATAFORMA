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

			// Variante rara: tint roxo, aplica veneno alem de lentidao.
			bool raro;

		public:
			OlhoVoador(Vector2f pos, Vector2f tam, bool raro = false);
			~OlhoVoador();
			void atacar(int jogador);
			void setAnimacao(int anim);
			float getVida();
			float getSize();
			void atualizar();

			void salvar(int save);
			void limparArquivo(int save);
		};

	}
}