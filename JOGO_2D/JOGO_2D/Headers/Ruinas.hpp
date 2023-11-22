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

			if (!texturaFundo.loadFromFile("Assets/Cenario/background_2.png")) {
				exit(1);
			}
		}
		~Ruinas()
		{
		}
	};
}