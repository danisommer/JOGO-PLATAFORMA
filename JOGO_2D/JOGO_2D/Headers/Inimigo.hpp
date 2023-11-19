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
			
		protected:
			static Jogador* jogador1;
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
			float distanciaAtaqueX;
			float distanciaAtaqueY;
			//bool concluida;
			bool parado;
			bool teleportando;

		public:
			Inimigo(Vector2f pos, Vector2f tam);
			~Inimigo();
			void perseguirJogador(Vector2f posJogador, Vector2f posInimigo);
			void moveAleatorio();
			virtual void atualizar() = 0;
			void atualizarAnimacao();
			virtual void atacar() = 0;
			void setAnimacao(int anim) = 0;
			static void setJogador(Jogador* j);
			bool getVoador();
			virtual float getVida() = 0;
			virtual float getSize() = 0;
			bool getParado();
			float getDistanciaAtaqueX();
			float getDistanciaAtaqueY();
		};
	}
}

