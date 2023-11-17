#pragma once
#include "Entidade.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Obstaculo : public Entidade
		{
		private:
			bool danoso;

		private:
			virtual void executar() = 0;
		};
	}
}



