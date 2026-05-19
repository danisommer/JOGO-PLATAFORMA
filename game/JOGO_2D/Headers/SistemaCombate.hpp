#pragma once

class Mundo;
namespace Lista { class ListaEntidade; }

namespace Sistemas
{
	// Resolve o combate corpo-a-corpo: aplica o dano dos ataques dos
	// jogadores aos inimigos dentro do alcance e pontua os acertos.
	// Extraido de Fase::AtualizarPersonagens (Passo 6).
	class SistemaCombate
	{
	public:
		void resolver(Mundo& mundo, Lista::ListaEntidade& personagens);
	};
}
