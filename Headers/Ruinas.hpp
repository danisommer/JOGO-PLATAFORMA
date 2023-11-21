#pragma once

#include "Fase.hpp"

namespace Fases
{
	class Ruinas : public Fase
	{
	private:
		bool concluida2;

	public:
		Ruinas();
		~Ruinas();
		void criarMapa();
		void setGerenciador();
	};
}