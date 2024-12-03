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

			if (!texturaFundo.loadFromFile("Assets/Cenario/background_1.png")) {
				exit(1);
			}
		}
		~Floresta()
		{
		}
	};
}