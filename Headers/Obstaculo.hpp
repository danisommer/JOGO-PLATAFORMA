#pragma once
#include "Entidade.hpp"
#include "Jogador.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Obstaculo : public Entidade
		{
		protected:
			static Personagens::Jogador* jogador1;
			static Personagens::Jogador* jogador2;
			bool danoso;
			bool colidir;

		public:
			Obstaculo(const Vector2f pos, const Vector2f tam);
			~Obstaculo();
			void atualizar() = 0;
			virtual void danar(int jogador) = 0;
			static void setJogador(Personagens::Jogador* j);
			static void setJogador2(Personagens::Jogador* j);
			bool getColidir();
			bool getDanoso();

		};
	}
}