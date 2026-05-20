#include "EstadoPausa.hpp"
#include "Gerenciador_Estados.hpp"
#include "Principal.hpp"
#include "Fase.hpp"
#include "Mundo.hpp"
#include "Menu.hpp"

namespace Estados
{
	EstadoPausa::EstadoPausa(Gerenciadores::Gerenciador_Estados* g, Principal* p,
		Fases::Fase* fase) :
		Estado(g),
		principal(p),
		fase(fase),
		gerenciadorGrafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
		gerenciadorEventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
		escolhendoSlot(false),
		modoCarregar(false)
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

		// Segunda etapa: escolher o slot de save (para salvar ou carregar).
		if (escolhendoSlot)
		{
			Tela& telaMundos = principal->getTelaMundos();
			telaMundos.setPosX(posX);

			int slot = telaMundos.verificaEventoTela();
			if (slot != -1 && slot != 3)
			{
				if (modoCarregar)
				{
					// Aplica o save: primeiro restaura o Mundo (faseAtual,
					// pontuacao, kills, arvore), depois regenera a fase
					// no numero salvo e finalmente reposiciona o(s)
					// jogador(es). Sem a regeneracao, o player seria
					// "teleportado" para o cenario da fase atual com a
					// posicao salva, ficando fora dela.
					fase->aplicarSave(slot + 1);

					Mundo& m = principal->getMundo();
					fase->desalocaEntidades();
					fase->setFase(m.getFaseAtual());
					// Numero de jogadores e recuperado do save tambem;
					// se for diferente do atual, o aplicarSave em si
					// nao trata isso - fica como simplificacao.
					fase->instanciaProcedural(m.getFaseAtual(), 1);
					fase->aplicarSave(slot + 1);
				}
				else
				{
					fase->salvarJogo(slot + 1);
				}
				gerenciadorEventos->despausarJogo();
				gerenciadorEstados->desempilhar();
				return;
			}
			else if (slot == 3) // "Sair" volta sem aplicar
			{
				escolhendoSlot = false;
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

		case 1:   // Recuperar (carregar save)
			escolhendoSlot = true;
			modoCarregar = true;
			break;

		case 2:   // Salvar
			escolhendoSlot = true;
			modoCarregar = false;
			break;

		case 3:   // Habilidades: abre a skill tree no Mundo atual
			Menu::abrirTelaHabilidades(principal->getMundo());
			break;

		case 4:   // Sair da fase
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
