#pragma once

#include "Estado.hpp"
#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Eventos.hpp"

class Principal;
namespace Fases { class Fase; }

namespace Estados
{
	// Estado de pausa, empilhado sobre o EstadoJogo. Concentra a logica
	// do menu de pausa (continuar / recuperar / salvar / sair) que antes
	// vivia espalhada em Principal como flags booleanas.
	class EstadoPausa : public Estado
	{
	private:
		Principal* principal;
		Fases::Fase* fase;

		Gerenciadores::Gerenciador_Grafico* gerenciadorGrafico;
		Gerenciadores::Gerenciador_Eventos* gerenciadorEventos;

		bool escolhendoSlot;   // true enquanto a tela de mundos esta ativa

	public:
		EstadoPausa(Gerenciadores::Gerenciador_Estados* g, Principal* p,
			Fases::Fase* fase);

		void processarEventos() override;
		void atualizar() override;
		void desenhar() override;
	};
}
