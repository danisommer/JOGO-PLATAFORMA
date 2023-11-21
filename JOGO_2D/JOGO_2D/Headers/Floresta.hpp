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
			fase = 1;
		}
		~Floresta()
		{
		}
	};
}