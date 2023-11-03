#pragma once
#include "Personagem.hpp"
#include "Animacao.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

namespace Entidades
{
	namespace Personagens
	{
		class Jogador : public Personagem
		{
		private:
			float aceleracao;
			float desaceleracao;
			float velocidadeMaxima;
			float jumpStrength;
			int n_frames;
			int count;
			int lado;
			int animacao;
			int anterior;
			int iteracoes;
			int ataque;
			std::vector<Animacao> animacoes;
			Animacao* animacaoAtual;
			
			void inicializaAnimacoes();

		public:
			Jogador(Vector2f pos, Vector2f tam);
			~Jogador();
			void atualizar();
			void setAnimacao(int anim) override;
		};
	}

}



