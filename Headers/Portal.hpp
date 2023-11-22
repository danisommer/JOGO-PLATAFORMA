#pragma once
#include "Entidade.hpp"
#include "Animacao.hpp"

namespace Entidades
{

	class Portal : public Entidade
	{
	private:
		Animacao animacaoTeleporte;
		int cont;
		int iteracoes;
		bool terminou;

	public:
		Portal(const Vector2f pos, const Vector2f tam);
		~Portal();
		void inicializaAnimacoes();
		void atualizarAnimacao();
		bool getTerminou();
		void atualizar();
		void desenharSprite();

	};

}