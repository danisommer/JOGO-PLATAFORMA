#pragma once
#include "Personagem.hpp"
#include "Jogador.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

#define ALCANCE_X 575.0f
#define ALCANCE_Y 575.0f

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

		public:
			Inimigo(Vector2f pos, Vector2f tam, Jogador* jogador);
			~Inimigo();
			void perseguirJogador(Vector2f posJogador, Vector2f posInimigo);
			void moveAleatorio();
			void atualizar();


		};
	}
}

