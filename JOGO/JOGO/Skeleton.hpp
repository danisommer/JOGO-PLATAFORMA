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
			std::vector<Animacao> animacoes;
			Animacao* animacaoAtual;
			int n_frames;
			int count;
			int lado;
			int animacao;
			int anterior;
			int iteracoes;

		public:
			Skeleton(Vector2f pos, Vector2f tam, Jogador* jogador);
			~Skeleton();
			void atualizarAnimacao();

		};

	}
}