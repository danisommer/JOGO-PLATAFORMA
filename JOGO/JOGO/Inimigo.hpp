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

#define DISTANCIA_MINIMA_ANIMACAO 0.0f

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
			int random_number;
			float distanciaX;
			float distanciaY;
			sf::Vector2f posAnteriorInimigo = sf::Vector2f(0.0f, 0.0f); // Inicialize com a posição inicial desejada


		protected:

			bool direita;
			float distanciaAlvo;
			std::vector<Animacao> animacoes;
			Animacao* animacaoAtual;
			int n_frames;
			int count;
			int lado;
			int animacao;
			int anterior;
			int iteracoes;

		public:
			Inimigo(Vector2f pos, Vector2f tam, Jogador* jogador);
			~Inimigo();
			void perseguirJogador(Vector2f posJogador, Vector2f posInimigo);
			void moveAleatorio();
			void atualizar();

			void setAnimacao(int anim) override;
			virtual void atualizarAnimacao() = 0;
			virtual void atacar() = 0;

		};
	}
}

