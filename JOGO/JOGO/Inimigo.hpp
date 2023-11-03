#pragma once
#include "Personagem.hpp"
#include "Jogador.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

//#define ALCANCE_X 800.0f
//#define ALCANCE_Y 100.0f

#define DISTANCIA_MINIMA_ANIMACAO 0.0001f

namespace Entidades
{
	namespace Personagens
	{
		class Inimigo : public Personagem
		{
		private:
			
			Clock relogio;
			void inicializa();
			int random_number;
			float distanciaX;
			float distanciaY;
			int atualizacao;
			float posAnteriorInimigo;
			bool parado;

		protected:
			Jogador* jogador;
			bool direita;
			float distanciaAlvo;
			std::vector<Animacao> animacoes;
			Animacao* animacaoAtual;
			int n_frames;
			int count;
			int lado;
			int anterior;
			int iteracoes;
			float ALCANCE_X;
			float ALCANCE_Y;
			float vidaAnterior;
			bool concluida;


		public:
			Inimigo(Vector2f pos, Vector2f tam, Jogador* jogador);
			~Inimigo();
			void perseguirJogador(Vector2f posJogador, Vector2f posInimigo);
			void moveAleatorio();
			void atualizar();
			void atualizarAnimacao();
			virtual void atacar() = 0;
			void setAnimacao(int anim) = 0;

		};
	}
}

