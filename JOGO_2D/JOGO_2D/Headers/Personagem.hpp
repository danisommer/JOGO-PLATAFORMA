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
			Vector2f vel;
			Vector2f direcao;
			Vector2f position;
			Vector2f velocity;
			Vector2f posAnterior;
			Sprite healthBar;
			Sprite border;
			Texture healthBarTexture;
			Texture borderTexture;
			Clock relogio;
			bool isJumping;
			bool voador;
			bool morto;
			bool moveu;
			bool concluida;
			bool inimigo;
			float gravity;
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
			void salvar() = 0;

		};
	}

}


