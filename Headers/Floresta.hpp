#pragma once

#include "Fase.hpp"

namespace Fases
{
	class Floresta : public Fase
	{
	private:
		bool concluida1;

	public:
		Floresta();
		~Floresta();
		void criarMapa();
	};
}