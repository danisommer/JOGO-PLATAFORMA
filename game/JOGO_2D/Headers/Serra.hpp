#pragma once
#include "Obstaculo.hpp"
#include "Animacao.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Serra : public Obstaculo
		{
		private:
			float dano;
			Animacao animacao;
			int cont;
			int iteracoes;

		public:
			Serra(const Vector2f pos, const Vector2f tam);
			~Serra();
			void inicializaAnimacoes();
			void atualizaAnimacao();
			void atualizar();
			void danar(int jogador);
			void salvar(int save);
			void limparArquivo(int save);
		};
	}
}


