#pragma once
#include "Personagem.hpp"
#include "Jogador.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

#define ALCANCE_X 800.0f
#define ALCANCE_Y 800.0f

namespace Entidades
{
	namespace Personagens
	{
		class Inimigo : public Personagem
		{
		private:
			Jogador* jogador;
			Clock relogio;
			void inicializa();
			int iteracoes;
			int random_number;
			float distancia;

		protected:
			bool direita;
			float distanciaAlvo;

		public:
			Inimigo(Vector2f pos, Vector2f tam, Jogador* jogador);
			~Inimigo();
			void perseguirJogador(Vector2f posJogador, Vector2f posInimigo);
			void moveAleatorio();
			void atualizar();

			void setAnimacao(int anim) override;
			virtual void atualizarAnimacao() = 0;

		};
	}
}

