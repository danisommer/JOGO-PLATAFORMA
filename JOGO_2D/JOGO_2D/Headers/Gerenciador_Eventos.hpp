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
		Entidades::Personagens::Jogador* pJogador2;

		static Gerenciador_Eventos* pGerenciador;
		Gerenciador_Eventos();

		bool jogoPausado;
		bool escPressionadoAnteriormente;

	public:
		~Gerenciador_Eventos();
		static Gerenciador_Eventos* getGerenciador();
		void setJogador(Entidades::Personagens::Jogador* jogador);
		void setJogador2(Entidades::Personagens::Jogador* jogador);
		void Executar();

		bool getJogoPausado();
		void despausarJogo();

	};

}