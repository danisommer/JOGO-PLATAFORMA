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
			// Nivel do inimigo - acompanha a fase em que foi instanciado.
			// E exibido no HUD do inimigo (ao lado da barra de vida).
			int nivel;

		public:
			Inimigo(Vector2f pos, Vector2f tam);
			~Inimigo();
			void perseguirJogador(Vector2f posJogador, Vector2f posInimigo);
			void moveAleatorio();
			virtual void atualizar() = 0;
			void atualizarAnimacao();
			virtual void atacar(int jogador) = 0;
			void setAnimacao(int anim) = 0;
			const bool getVoador() const;
			virtual float getVida() = 0;
			virtual float getSize() = 0;
			const bool getParado() const;
			const float getDistanciaAtaqueX() const;
			const float getDistanciaAtaqueY() const;
			// Verifica se o inimigo pode atacar o alvo, considerando o
			// alinhamento adequado: inimigos terrestres so atacam quando
			// estao aproximadamente na mesma altura dos "pes"; voadores
			// usam uma tolerancia vertical em torno do centro. Em ambos
			// os casos a sobreposicao horizontal e exigida (com base no
			// alcance de ataque).
			bool podeAtacarAlvo(const sf::RectangleShape& alvo) const;

			// Aplica o escalonamento de dificuldade baseado no numero
			// da fase. Vida cresce ~12% e dano ~8% por fase, fazendo
			// com que fases mais altas exijam mais habilidade mesmo
			// quando o jogador esta com a arvore de skills carregada.
			void aplicarDificuldade(int numeroFase);

			int getNivel() const { return nivel; }
			// Desenha o rotulo "Lv N" ao lado da barra de vida. Chamado
			// pelas classes derivadas em desenharSprite/atualizar para
			// que toda subclasse tenha o indicador automatico.
			void desenharNivel();

			// Sobrescreve para incluir o "Lv N" alem do sprite e da
			// barra padroes do Personagem.
			void desenharSprite() override;
			void limparArquivo(int save) = 0;
			void salvar(int save) = 0;
		};
	}
}

