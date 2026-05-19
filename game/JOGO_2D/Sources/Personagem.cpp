#include "Personagem.hpp"
#include <iostream>

namespace Entidades
{
	namespace Personagens
	{
		namespace
		{
			// Constantes de fisica, em unidades por passo de 1/60 s.
			// GRAVIDADE     : aceleracao vertical somada a velocity.y por passo.
			// VEL_TERMINAL  : limite de queda, evita atravessar plataformas.
			constexpr float GRAVIDADE = 0.9f;
			constexpr float VEL_TERMINAL = 22.0f;
			constexpr float VIDA_INICIAL = 100.0f;

			// Combate. Apos um golpe, o personagem fica INVUL_FRAMES passos
			// sem poder receber dano; os primeiros STUN_FRAMES desse periodo
			// sao de hit-stun (atordoado). Como STUN < INVUL, sempre sobra
			// uma janela em que o inimigo esta livre para revidar mesmo sob
			// pressao continua. KNOCKBACK_DIST e o empurrao do golpe.
			constexpr int   INVUL_FRAMES = 34;
			constexpr int   STUN_FRAMES = 15;
			constexpr float KNOCKBACK_DIST = 14.0f;
		}

		Personagem::Personagem() :
			Entidade(Vector2f(), Vector2f()),
			vel(),
			velocity(0.0f, 0.0f),
			posAnterior(),
			isJumping(true),
			voador(false),
			morto(false),
			moveu(true),
			concluida(true),
			inimigo(false),
			vida(VIDA_INICIAL),
			dano(),
			animacao(0),
			tempoInvulneravel(0)
		{
			if (!healthBarTexture.loadFromFile("Assets/vida.png")) {
				exit(1);
			}

			if (!borderTexture.loadFromFile("Assets/moldura.png")) {
				exit(1);
			}
		}

		Personagem::~Personagem()
			= default;

		void Personagem::cair()
		{
			// Integracao simples da gravidade: aceleracao constante somada
			// a velocidade vertical, com limite de velocidade terminal.
			// Substitui o antigo hack que mutava o membro 'gravity'.
			if (voador)
				return;

			velocity.y += GRAVIDADE;
			if (velocity.y > VEL_TERMINAL)
				velocity.y = VEL_TERMINAL;

			corpo.move(0.0f, velocity.y);
		}

		void Personagem::setIsJumping(bool IJ)
		{
			isJumping = IJ;
		}

		void Personagem::setY(float Y)
		{
			// Usado pela resolucao de colisao para zerar a queda ao pousar.
			velocity.y = Y;
		}

		void Personagem::setPos(float X, float Y)
		{
			corpo.setPosition(X, Y);
		}

		bool Personagem::tomarDano(float dano, int dirKnockback)
		{
			// Ja em animacao de morte: ignora.
			if (animacao == 2)
				return false;

			// Quadros de invulnerabilidade ativos: o golpe nao acerta.
			// E isto que impede o jogador de "derreter" o inimigo segurando
			// o botao - cada golpe so conta a cada INVUL_FRAMES passos.
			if (tempoInvulneravel > 0)
				return false;

			vida -= dano;
			animacao = 1;
			tempoInvulneravel = INVUL_FRAMES;

			if (dirKnockback != 0)
				corpo.move(static_cast<float>(dirKnockback) * KNOCKBACK_DIST, 0.0f);

			return true;
		}

		void Personagem::atualizarEstadoCombate()
		{
			if (tempoInvulneravel > 0)
				tempoInvulneravel--;
		}

		bool Personagem::estaAtordoado() const
		{
			// Atordoado durante os STUN_FRAMES iniciais da invulnerabilidade.
			return tempoInvulneravel > (INVUL_FRAMES - STUN_FRAMES);
		}

		void Personagem::morrer()
		{
			morto = true;
		}

		bool Personagem::getMorte()
		{
			return morto;
		}

		bool Personagem::getMoveu()
		{
			Vector2f posAtual = corpo.getPosition();

			if (posAtual.x != posAnterior.x || posAtual.y != posAnterior.y)
				moveu = true;
			else
				moveu = false;
			
			posAnterior = posAtual;

			return moveu;
		}
		void Personagem::atualizarBarraVida()
		{
			float vidaMax = getVida();

			if (vida >= 0.0f)
			{
				healthBar.setTexture(healthBarTexture);
				border.setTexture(borderTexture);

				sf::Vector2f pos = corpo.getPosition();
				pos.x -= 25.0f;
				pos.y -= 25.0f;

				healthBar.setPosition(pos.x, pos.y);
				border.setPosition(pos.x, pos.y);

				healthBar.setScale((vida / vidaMax) * 0.2f, 0.2f);

				border.setScale(0.2f, 0.2f);
			}
		}

		void Personagem::desenharSprite()
		{
			atualizarBarraVida();

			gerenciador_grafico->desenhaSprite(sprite);
		
			if (animacao != 2)
			{
				gerenciador_grafico->desenhaSprite(border);
				gerenciador_grafico->desenhaSprite(healthBar);
			}
		}
	}

}
