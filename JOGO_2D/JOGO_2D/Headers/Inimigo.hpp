#pragma once
#include "Personagem.hpp"
#include "Jogador.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

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
			static Jogador* jogador2;
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
			bool parado;
			bool teleportando;
			static bool chefaoMorreu;
			static Vector2f posChefao;

		public:
			Inimigo(Vector2f pos, Vector2f tam);
			~Inimigo();
			void perseguirJogador(Vector2f posJogador, Vector2f posInimigo);
			void moveAleatorio();
			virtual void atualizar() = 0;
			void atualizarAnimacao();
			virtual void atacar(int jogador) = 0;
			void setAnimacao(int anim) = 0;
			static void setJogador(Jogador* j);
			static void setJogador2(Jogador* j);
			const bool getVoador() const;
			virtual float getVida() = 0;
			virtual float getSize() = 0;
			const bool getParado() const;
			const float getDistanciaAtaqueX() const;
			const float getDistanciaAtaqueY() const;
			static bool getChefaoMorreu();
			static void setChefaoMorreu(bool cm);
			static Vector2f getPosChefao();
		};
	}
}

