#pragma once

#include <memory>
#include <vector>
#include "Estado.hpp"

namespace Gerenciadores
{
	// Pilha de estados do jogo (State Pattern + Scene stack).
	// As transicoes (empilhar/desempilhar/trocar) sao ADIADAS e aplicadas
	// ao final de cada quadro, para que um estado possa solicitar com
	// seguranca sua propria remocao ou substituicao de dentro de si mesmo.
	class Gerenciador_Estados
	{
	private:
		std::vector<std::unique_ptr<Estados::Estado>> pilha;

		enum class TipoTransicao { Empilhar, Desempilhar, Trocar };
		struct Transicao
		{
			TipoTransicao tipo;
			std::unique_ptr<Estados::Estado> estado;
		};
		std::vector<Transicao> pendentes;

		void aplicarTransicoes();

	public:
		void empilhar(std::unique_ptr<Estados::Estado> estado);
		void desempilhar();
		void trocar(std::unique_ptr<Estados::Estado> estado);

		bool vazio() const;

		// Roda um quadro do estado do topo e aplica as transicoes pendentes.
		void executarQuadro();
	};
}
