#pragma once

#include "Estado.hpp"
#include "Relogio.hpp"
#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Eventos.hpp"
#include "Gerenciador_Colisoes.hpp"

class Principal;
namespace Fases { class Fase; }

namespace Estados
{
	// Estado que executa uma fase. Substitui os while-loops aninhados e o
	// "flag soup" (derrota/salvar/carregar) que existiam em Principal.
	// Ao concluir a fase 1, troca-se por um EstadoJogo da fase 2;
	// ao perder ou terminar, o estado se desempilha (voltando ao menu).
	class EstadoJogo : public Estado
	{
	private:
		Principal* principal;
		Fases::Fase* fase;
		int numFase;
		int numJogadores;

		Gerenciadores::Gerenciador_Grafico* gerenciadorGrafico;
		Gerenciadores::Gerenciador_Eventos* gerenciadorEventos;
		Gerenciadores::Gerenciador_Colisoes* gerenciadorColisoes;

		// Passo fixo: a simulacao avanca sempre em fatias de PASSO_FIXO
		// segundos, independentemente da taxa de quadros da maquina.
		Relogio relogio;
		float acumulador;
		bool finalizado;

		void executarPasso();

	public:
		// slotCarregar > 0 aplica o save daquele slot apos montar a fase.
		EstadoJogo(Gerenciadores::Gerenciador_Estados* g, Principal* p,
			int numFase, int numJogadores, bool zerarPontuacao,
			int slotCarregar = 0);

		void processarEventos() override;
		void atualizar() override;
		void desenhar() override;
	};
}
