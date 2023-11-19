#pragma once
#include "Entidade.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		class Obstaculo : public Entidade
		{
		private:

		public:
			Obstaculo(const Vector2f pos, const Vector2f tam);
			~Obstaculo();
			virtual void executar() = 0;
			//virtual void danar() = 0;

		};
	}
}




