#include "EstadoPausa.hpp"
#include "Gerenciador_Estados.hpp"
#include "Principal.hpp"
#include "Fase.hpp"
#include "Mundo.hpp"
#include "Menu.hpp"
#include "GerenciadorSave.hpp"
#include "Gerenciador_Recursos.hpp"

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

		// Segunda etapa: selecao de slot com informacoes do save.
		if (escolhendoSlot)
		{
			const sf::Font& fonte = Gerenciadores::Gerenciador_Recursos::getGerenciador()
				->getFonte("Menu/antiquity-print.ttf");

			// Le os saves disponíveis
			struct InfoSlot { bool valido = false; int fase = 0; int kills = 0; int numJog = 0; };
			InfoSlot infos[3];
			for (int i = 0; i < 3; ++i)
			{
				Persistencia::DadosSave d;
				if (Persistencia::GerenciadorSave::carregar(i + 1, d))
				{
					infos[i].valido = true;
					infos[i].fase = d.fase;
					infos[i].kills = d.kills;
					infos[i].numJog = d.numJogadores;
				}
			}

			// Desenha o backdrop e os slots
			auto* janela = gerenciadorGrafico->getJanela();
			const sf::View viewAnterior = janela->getView();
			sf::View viewUI(sf::FloatRect(0.0f, 0.0f,
				static_cast<float>(janela->getSize().x),
				static_cast<float>(janela->getSize().y)));
			janela->setView(viewUI);

			// Titulo
			sf::Text titulo;
			titulo.setFont(fonte);
			titulo.setString(modoCarregar ? "Carregar Jogo" : "Salvar Jogo");
			titulo.setCharacterSize(48);
			titulo.setFillColor(sf::Color::White);
			titulo.setOutlineColor(sf::Color::Black);
			titulo.setOutlineThickness(2.0f);
			titulo.setPosition(80.0f, 40.0f);

			// Slots
			struct BotaoSlot { sf::RectangleShape rect; sf::Text label; bool ativo; };
			std::vector<BotaoSlot> botoes;
			for (int i = 0; i < 3; ++i)
			{
				BotaoSlot b;
				b.rect.setSize(sf::Vector2f(700.0f, 65.0f));
				b.rect.setPosition(80.0f, 130.0f + i * 85.0f);

				std::string label;
				if (infos[i].valido)
				{
					label = "Save " + std::to_string(i + 1)
						+ "  |  Fase " + std::to_string(infos[i].fase)
						+ "  |  Kills " + std::to_string(infos[i].kills)
						+ "  |  " + std::to_string(infos[i].numJog) + "P";
					b.rect.setFillColor(sf::Color(60, 60, 100, 200));
					b.ativo = true;
				}
				else
				{
					label = "Save " + std::to_string(i + 1) + "  |  Vazio";
					// Para salvar: slot vazio e valido; para carregar: nao
					b.ativo = !modoCarregar;
					b.rect.setFillColor(modoCarregar
						? sf::Color(40, 40, 40, 160)
						: sf::Color(60, 100, 60, 200));
				}
				b.rect.setOutlineColor(sf::Color::White);
				b.rect.setOutlineThickness(1.5f);

				b.label.setFont(fonte);
				b.label.setString(label);
				b.label.setCharacterSize(24);
				b.label.setFillColor(b.ativo ? sf::Color::White : sf::Color(130, 130, 130));
				b.label.setPosition(95.0f, 148.0f + i * 85.0f);

				botoes.push_back(b);
			}

			// Botao Voltar
			sf::RectangleShape btnVoltar;
			btnVoltar.setSize(sf::Vector2f(200.0f, 55.0f));
			btnVoltar.setPosition(80.0f, 400.0f);
			btnVoltar.setFillColor(sf::Color(100, 30, 30, 200));
			btnVoltar.setOutlineColor(sf::Color::White);
			btnVoltar.setOutlineThickness(1.5f);
			sf::Text txtVoltar;
			txtVoltar.setFont(fonte);
			txtVoltar.setString("Voltar");
			txtVoltar.setCharacterSize(28);
			txtVoltar.setFillColor(sf::Color::White);
			txtVoltar.setPosition(105.0f, 412.0f);

			// Processa eventos
			sf::Event ev;
			int slotEscolhido = -1;
			bool voltar = false;
			while (janela->pollEvent(ev))
			{
				if (ev.type == sf::Event::Closed)
				{
					gerenciadorGrafico->fecharJanela();
					janela->setView(viewAnterior);
					return;
				}
				if (ev.type == sf::Event::MouseButtonPressed &&
					ev.mouseButton.button == sf::Mouse::Left)
				{
					const sf::Vector2f cm = janela->mapPixelToCoords(
						{ ev.mouseButton.x, ev.mouseButton.y });

					for (int i = 0; i < 3; ++i)
					{
						if (botoes[i].ativo &&
							botoes[i].rect.getGlobalBounds().contains(cm))
						{
							slotEscolhido = i + 1;
							break;
						}
					}
					if (btnVoltar.getGlobalBounds().contains(cm))
						voltar = true;
				}
			}

			if (slotEscolhido > 0)
			{
				janela->setView(viewAnterior);
				if (modoCarregar)
				{
					fase->aplicarSave(slotEscolhido);
					Mundo& m = principal->getMundo();
					fase->desalocaEntidades();
					fase->setFase(m.getFaseAtual());
					fase->instanciaProcedural(m.getFaseAtual(), 1);
					fase->aplicarSave(slotEscolhido);
				}
				else
				{
					fase->salvarJogo(slotEscolhido);
				}
				gerenciadorEventos->despausarJogo();
				gerenciadorEstados->desempilhar();
				return;
			}
			if (voltar)
			{
				janela->setView(viewAnterior);
				escolhendoSlot = false;
			}

			// Renderiza
			gerenciadorGrafico->limpaTela();
			janela->draw(titulo);
			for (auto& b : botoes)
			{
				janela->draw(b.rect);
				janela->draw(b.label);
			}
			janela->draw(btnVoltar);
			janela->draw(txtVoltar);
			janela->setView(viewAnterior);
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
