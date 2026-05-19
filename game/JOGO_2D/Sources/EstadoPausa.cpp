#include "EstadoPausa.hpp"
#include "Gerenciador_Estados.hpp"
#include "Principal.hpp"
#include "Fase.hpp"

namespace Estados
{
	EstadoPausa::EstadoPausa(Gerenciadores::Gerenciador_Estados* g, Principal* p,
		Fases::Fase* fase) :
		Estado(g),
		principal(p),
		fase(fase),
		gerenciadorGrafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
		gerenciadorEventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
		escolhendoSlot(false)
	{
	}

	void EstadoPausa::processarEventos()
	{
		// As telas (Tela::verificaEventoTela) consomem a propria fila de
		// eventos; nada a fazer aqui.
	}

	void EstadoPausa::atualizar()
	{
		const float posX = gerenciadorGrafico->getViewCenter().x - 700.0f;

		// Segunda etapa: escolher o slot de save.
		if (escolhendoSlot)
		{
			Tela& telaMundos = principal->getTelaMundos();
			telaMundos.setPosX(posX);

			int slot = telaMundos.verificaEventoTela();
			if (slot != -1)
			{
				fase->salvarJogo(slot + 1);
				gerenciadorEventos->despausarJogo();
				gerenciadorEstados->desempilhar();
				return;
			}

			gerenciadorGrafico->limpaTela();
			telaMundos.desenharTela();
			gerenciadorGrafico->mostraElemento();
			return;
		}

		// Primeira etapa: menu de pausa.
		Tela& telaPausa = principal->getTelaPausa();
		telaPausa.setPosX(posX);

		switch (telaPausa.verificaEventoTela())
		{
		case 0:   // Continuar
			gerenciadorEventos->despausarJogo();
			gerenciadorEstados->desempilhar();
			return;

		case 1:   // Recuperar (carregamento ainda nao implementado)
			gerenciadorEventos->despausarJogo();
			gerenciadorEstados->desempilhar();
			return;

		case 2:   // Salvar
			escolhendoSlot = true;
			break;

		case 3:   // Sair da fase
			fase->setDerrota(true);
			gerenciadorEventos->despausarJogo();
			gerenciadorEstados->desempilhar();
			return;
		}

		gerenciadorGrafico->limpaTela();
		telaPausa.desenharTela();
		gerenciadorGrafico->mostraElemento();
	}

	void EstadoPausa::desenhar()
	{
		// Desenho feito em atualizar().
	}
}
