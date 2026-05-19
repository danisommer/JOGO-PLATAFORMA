		#pragma once
#include <SFML/Graphics.hpp>
#include "Entidade.hpp"
#include "Animacao.hpp"

using namespace sf;

namespace Entidades
{
	namespace Personagens
	{
		class Personagem : public Entidade
		{
		protected:
			// vel      = velocidade configurada (modulo), em px por passo.
			// velocity = velocidade ATUAL do corpo, em px por passo;
			//            velocity.y e controlada por gravidade e pulo.
			// A simulacao roda em passo fixo de 1/60 s (ver EstadoJogo),
			// portanto "por passo" equivale a "por 1/60 de segundo".
			Vector2f vel;
			Vector2f velocity;
			Vector2f posAnterior;
			Sprite healthBar;
			Sprite border;
			Texture healthBarTexture;
			Texture borderTexture;
			bool isJumping;
			bool voador;
			bool morto;
			bool moveu;
			bool concluida;
			bool inimigo;
			float vida;
			float dano;
			int animacao;
			// Contador de invulnerabilidade pos-dano (em passos). Enquanto
			// > 0 o personagem nao recebe novo dano; durante a parte inicial
			// ele fica atordoado (ver estaAtordoado). Isso da ao inimigo uma
			// janela para reagir em vez de morrer preso em hit-stun.
			int tempoInvulneravel;
			

		public:
			Personagem();
			~Personagem();
			void atualizar() = 0;

			void cair();
			void setIsJumping(bool IJ);
			void setY(float Y);
			void setPos(float X, float Y);
			// Aplica dano respeitando os quadros de invulnerabilidade.
			// dirKnockback (-1/0/+1) empurra a vitima nessa direcao.
			// Devolve true se o golpe efetivamente acertou.
			bool tomarDano(float dano, int dirKnockback = 0);
			// Decrementa os temporizadores de combate; chamar uma vez por passo.
			void atualizarEstadoCombate();
			// true durante a parte inicial da invulnerabilidade (hit-stun).
			bool estaAtordoado() const;
			virtual void setAnimacao(int anim) = 0;
			void morrer();
			bool getMorte();
			bool getMoveu();
			void atualizarBarraVida();
			virtual float getVida() = 0;
			void desenharSprite();
			void salvar(int save) = 0;
			void limparArquivo(int save) = 0;

		};
	}

}


