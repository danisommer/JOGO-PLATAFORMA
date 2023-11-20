#pragma once
#include "Inimigo.hpp"
#include "vector"

namespace Entidades
{
	namespace Personagens
	{
		class Mushroom : public Inimigo
		{
		private:
			void inicializaAnimacoes();
			float forcaVeneno;
			int tempoEnvenenamento;
			bool venenoEspecial;
			sf::Color corJogador;

		public:
			Mushroom(Vector2f pos, Vector2f tam);
			~Mushroom();
			void atacar(int jogador);
			void setAnimacao(int anim);
			float getVida();
			float getSize();
			void atualizar();
			void envenenar(int jogador);


		};

	}
}