#pragma once
#include "Entidade.hpp"
#include "Animacao.hpp"
#define MAX_DIST 1000.0f;

namespace Entidades
{
	class Projetil : public Entidade
	{
	private:
		float vel;
		const float dano;
		float distanciaPercorrida;
		bool colidiu;
		bool explodiu;
		int n_frames;
		int count;
		int lado;
		int anterior;
		int iteracoes;
		std::vector<Animacao> animacoes;
		Animacao* animacaoAtual;
		bool concluida;

		// Modo homing: projetil gira lentamente em direcao ao alvo.
		bool guiado;
		sf::Vector2f posAlvo;   // posicao do alvo, atualizada externamente
		float angulo;           // direcao atual em radianos
		float velAngular;       // velocidade max de giro (rad/frame)
		int vidaUtil;           // frames restantes antes de sumir (0 = sem limite)

	public:
		Projetil(const Vector2f pos, const Vector2f tam, bool direita);
		~Projetil();
		void atualizar();
		void inicializaAnimacoes();
		void atualizarAnimacao(int animacao);
		void setColidiu(bool c);
		// Ativa modo homing com posicao inicial do alvo.
		void setHoming(sf::Vector2f alvoInicial, float velAngularRad, int frames);
		// Atualiza a posicao do alvo (chamar a cada frame enquanto o alvo viver).
		void atualizarAlvo(sf::Vector2f pos);
		bool isGuiado() const;
		const float getDano() const;
		const bool getExplodiu() const;
		const bool getColidiu() const;
		void desenharSprite();

		void salvar(int save);
		void limparArquivo(int save);

	};

}