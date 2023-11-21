#pragma once

#include "Fase.hpp"

namespace Fases
{
	class Ruinas : public Fase
	{
	private:
		bool concluida2;

	public:
		Ruinas():Fase(), concluida2(false)
		{
			instanciaEntidades("Fases/fase1-2p.txt");
		}
		~Ruinas()
		{

		}
	};
}