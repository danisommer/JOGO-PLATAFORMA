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
			std::vector<Animacao> animacoes;
			Animacao* animacaoAtual;
			int n_frames;
			int count;
			int lado;
			int animacao;
			int anterior;
			int iteracoes;

		public:
			Goblin(Vector2f pos, Vector2f tam, Jogador* jogador);
			~Goblin();
			void atualizarAnimacao();

		};

	}
}