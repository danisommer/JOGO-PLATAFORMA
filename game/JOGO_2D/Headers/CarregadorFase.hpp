#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Entidade.hpp"

namespace Fases
{
	// Le um arquivo de fase (.txt) e CONSTROI as entidades correspondentes,
	// aplicando a aleatorizacao de spawns. Nao registra nada em managers:
	// devolve as entidades para a Fase fazer a ligacao. Separa o
	// carregamento (parsing + fabrica) da orquestracao (Passo 6).
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

	public:
		// Lanca std::runtime_error se o arquivo nao puder ser aberto.
		std::vector<std::unique_ptr<Entidades::Entidade>> carregar(
			const std::string& arquivo, int numeroFase);
	};
}
