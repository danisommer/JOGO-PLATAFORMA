#pragma once

#include "Fase.hpp"

namespace Fases
{
	class Floresta : public Fase
	{
	private:
		bool concluida1;

	public:
		Floresta():Fase(), concluida1(false)
		{
			instanciaEntidades("Fases/fase2-2p.txt");
		}
		~Floresta()
		{

		}
	};
}