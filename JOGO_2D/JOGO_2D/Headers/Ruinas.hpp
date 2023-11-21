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
			fase = 2;
		}
		~Ruinas()
		{
		}
	};
}