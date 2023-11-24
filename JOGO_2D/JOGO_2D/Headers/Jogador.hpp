#pragma once
#include "Personagem.hpp"
#include "Animacao.hpp"
#include <fstream>

#include <SFML/Graphics.hpp>
using namespace sf;

namespace Entidades
{
	namespace Personagens
	{
		class Jogador : public Personagem
		{
		private:
			static bool jogadorCriado;

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
			float forcaPulo;
		
			Vector2f regiaoAtaque;
			std::vector<Animacao> animacoes;
			std::vector<Keyboard::Key> teclas;
			Animacao* animacaoAtual;
			const Vector2f tamanhoCorpo;

			bool concluiuFase;
			
			void inicializaAnimacoes();
			void inicializaTeclas();

		public:
			Jogador(Vector2f pos, Vector2f tam);
			~Jogador();
			void atualizar();
			void atualizarAnimacao(int animacao);
			void setAnimacao(int anim);
			const Vector2f getRegiaoAtaque() const;
			const float getDano() const;
			const bool getAtacando() const;
			float getVida();
			void setEnvenenado(bool veneno, int tempo, float intensidade, Color cor);
			void setLento(bool lentidao, int tempo, float fL, float fP);
			void mover(bool direita, bool esquerda);
			void bater(bool batendo);
			void pular(bool pulando);
			static void setJogadorCriado(bool jc);
			void setConcluiuFase(bool cf);
			bool getConcluiuFase();

			void salvar(int save);
			void limparArquivo(int save);


		};
	}

}



