#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Entidade.hpp"

namespace Fases
{
	// Constroi as entidades de uma fase. Possui duas estrategias:
	// - carregar() le um .txt do diretorio Fases/ (modo classico);
	// - gerarProcedural() cria a fase aleatoriamente em memoria,
	//   usado pelo modo roguelike para emitir fases infinitas com
	//   dificuldade escalonada e chefao a cada N fases.
	class CarregadorFase
	{
	private:
		int numCogumelos = 0;
		int numVoadores = 0;
		int numSerras = 0;
		int numEspinhos = 0;
		int numSlimes = 0;

		// Garante um minimo de cada inimigo/armadilha; alem disso, sorteia.
		bool aleatorizar(char character);

		// Helper para gerarProcedural(): instancia a entidade
		// correspondente ao caractere c, na posicao indicada, e
		// adiciona ao vetor saida. Reusa o switch usado por carregar.
		void instanciarChar(char c, std::size_t x, std::size_t y,
			int numeroFase,
			std::vector<std::unique_ptr<Entidades::Entidade>>& saida,
			int& indiceJogador) const;

	public:
		// Lanca std::runtime_error se o arquivo nao puder ser aberto.
		std::vector<std::unique_ptr<Entidades::Entidade>> carregar(
			const std::string& arquivo, int numeroFase);

		// Gera uma fase do modo roguelike. Sempre coloca o(s) jogador(es),
		// piso, paredes e um portal de saida; preenche com plataformas
		// e inimigos cuja quantidade cresce com numeroFase. A cada 5
		// fases gera o chefao no lugar de inimigos comuns.
		std::vector<std::unique_ptr<Entidades::Entidade>> gerarProcedural(
			int numeroFase, int numJogadores);
	};
}
