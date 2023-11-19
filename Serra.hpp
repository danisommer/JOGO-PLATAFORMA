#pragma once
#include "Obstaculo.hpp"
#include "Animacao.hpp"
#include "Jogador.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Serra : public Obstaculo
		{
		private:
			Animacao animacao;
			int cont;
			int iteracoes;

		public:
			Serra(const Vector2f pos, const Vector2f tam);
			~Serra();
			void inicializaAnimacoes();
			void atualizaAnimacao();
			void executar();
			//void danar(Jogador *pJogador);
			void atualizar();
		};
	}
}


