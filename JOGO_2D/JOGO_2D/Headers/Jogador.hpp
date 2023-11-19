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
			int anterior;
			int iteracoes;
			int ataque;
			float vidaAnterior;
			bool atacando;
			bool tomandoDano;

			bool envenenado;
			int tempoVeneno;
			int tempoDecorridoVeneno;
			float forcaVeneno;
			sf::Color corEnvenenado;

			bool lento;
			int tempoLentidao;
			int tempoDecorridoLentidao;
			float forcaLentidao;
		
			Vector2f regiaoAtaque;
			std::vector<Animacao> animacoes;
			Animacao* animacaoAtual;
			
			void inicializaAnimacoes();

		public:
			Jogador(Vector2f pos, Vector2f tam);
			~Jogador();
			void atualizar();
			void atualizarAnimacao(int animacao);
			void setAnimacao(int anim);
			Vector2f getRegiaoAtaque();
			float getDano();
			bool getAtacando();
			float getVida();
			void setEnvenenado(bool veneno, int tempo, float intensidade, Color cor);
			void setLento(bool lentidao, int tempo, float forca);
			void mover(bool direita, bool esquerda);
			void bater(bool batendo);
			void pular(bool pulando);

		};
	}

}


