#pragma once
#include "Obstaculo.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Parede : public Obstaculo
		{
		public:
			Parede(const Vector2f pos, const Vector2f tam);
			~Parede();
			void atualizar();
			void danar(int jogador);
			void salvar(int save);
			void limparArquivo(int save);
		};
	}
}