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

	public:
		Projetil(const Vector2f pos, const Vector2f tam, bool direita);
		~Projetil();
		void atualizar();
		void inicializaAnimacoes();
		void atualizarAnimacao(int animacao);
		void setColidiu(bool c);
		const float getDano() const;
		const bool getExplodiu() const;
		const bool getColidiu() const;
		void desenharSprite();


	};

}