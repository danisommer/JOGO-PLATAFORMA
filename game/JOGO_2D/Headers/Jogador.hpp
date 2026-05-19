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
			// Indice do jogador (0 = P1, 1 = P2). Substitui o antigo
			// 'static bool jogadorCriado', que tornava o comportamento
			// dependente da ordem de criacao e impedia mais de 2 jogadores.
			const int indiceJogador;

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
			Jogador(Vector2f pos, Vector2f tam, int indiceJogador);
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
			void setConcluiuFase(bool cf);
			bool getConcluiuFase();

			void salvar(int save);
			void limparArquivo(int save);

			// Usados pela camada de serializacao (GerenciadorSave).
			float getVidaAtual() const;
			void carregarEstado(float vidaSalva, Vector2f pos);

		};
	}

}



