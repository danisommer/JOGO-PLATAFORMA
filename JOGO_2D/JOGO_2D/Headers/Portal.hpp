#pragma once
#include "Obstaculo.hpp"
#include "Animacao.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Portal : public Obstaculo
		{
		private:
			Animacao animacaoTeleporte;
			int cont;
			int iteracoes;
			bool terminou;
			bool infinito;

		public:
			Portal(const Vector2f pos, const Vector2f tam, bool i, bool d);
			~Portal();
			void inicializaAnimacoes();
			void atualizarAnimacao();
			bool getTerminou();
			void atualizar();
			void desenharSprite();
			void danar(int jogador);

		};
	}
	

}