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
			

		public:
			Personagem();
			~Personagem();
			void atualizar() = 0;

			void cair();
			void setIsJumping(bool IJ);
			void setY(float Y);
			void setPos(float X, float Y);
			void tomarDano(float dano);
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


