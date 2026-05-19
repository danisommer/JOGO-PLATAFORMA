#pragma once

namespace Gerenciadores { class Gerenciador_Estados; }

namespace Estados
{
	// Interface de um estado do jogo (menu, jogo, pausa, ...).
	// O Gerenciador_Estados mantem uma pilha de estados e, a cada quadro,
	// delega processarEventos/atualizar/desenhar para o estado do topo.
	class Estado
	{
	protected:
		Gerenciadores::Gerenciador_Estados* gerenciadorEstados;

	public:
		explicit Estado(Gerenciadores::Gerenciador_Estados* g)
			: gerenciadorEstados(g) {}
		virtual ~Estado() = default;

		virtual void processarEventos() = 0;
		virtual void atualizar() = 0;
		virtual void desenhar() = 0;
	};
}
