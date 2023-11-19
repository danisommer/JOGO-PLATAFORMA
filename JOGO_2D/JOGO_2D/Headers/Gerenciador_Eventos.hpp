#pragma once

#include "Gerenciador_Grafico.hpp"
#include "Jogador.hpp"
#include "Personagem.hpp"

namespace Gerenciadores
{
	class Gerenciador_Eventos
	{
	private:
		static Gerenciador_Grafico* gerenciador_grafico;
		Entidades::Personagens::Jogador* pJogador;

		static Gerenciador_Eventos* pGerenciador;
		Gerenciador_Eventos();
	public:
		~Gerenciador_Eventos();
		static Gerenciador_Eventos* getGerenciador();
		void setJogador(Entidades::Personagens::Jogador* jogador);
		void Executar();
	};

}