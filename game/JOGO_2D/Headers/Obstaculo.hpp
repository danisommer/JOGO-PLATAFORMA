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
			bool danoso;
			bool colidir;

		public:
			Obstaculo(const Vector2f pos, const Vector2f tam);
			~Obstaculo();
			void atualizar() = 0;
			virtual void danar(int jogador) = 0;
			bool getColidir();
			bool getDanoso();
			void desenharSprite();
			void salvar(int save) = 0;
			void limparArquivo(int save) = 0;
		};
	}
}