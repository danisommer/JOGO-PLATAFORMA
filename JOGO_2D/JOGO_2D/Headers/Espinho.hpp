#pragma once
#include "Obstaculo.hpp"
#include "Animacao.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Espinho : public Obstaculo
		{
		private:
			float veneno;
			int tempoEnvenenamento;
			float danoInstantaneo;
			bool pisou;
			Animacao animacao;
			int cont;
			int iteracoes;

		public:
			Espinho(const Vector2f pos, const Vector2f tam);
			~Espinho();
			void inicializaAnimacoes();
			void atualizaAnimacao();
			void atualizar();
			void danar(int jogador);
			void salvar();

		};
	}
}


