#include "Menu.hpp"
#include "Configuracao.hpp"
#include "ArvoreHabilidades.hpp"
#include "Gerenciador_Recursos.hpp"
#include "Gerenciador_Grafico.hpp"

Menu::Menu() :
	posicaoTela(gerenciador_grafico->getViewCenter()),
	ordenado(false)
{
	fonte = new sf::Font();
	imagemFundo = new sf::Texture();
	sprite = new sf::Sprite();
	Text titulo;

	if (!fonte->loadFromFile("Menu/antiquity-print.ttf"))
	{
		exit(1);
	}

	titulo.setFont(*fonte);
	titulo.setString("Knight's Quest");
	titulo.setPosition(470, 70);
	titulo.setCharacterSize(75);
	titulo.setOutlineColor(Color::Black);
	titulo.setStyle(sf::Text::Bold);

	telaInicial.setTitulo(titulo);

	titulo.setPosition(470, 70);
	titulo.setString(" Fim de Jogo");

	telaGameOver.setTitulo(titulo);

	if (!imagemFundo->loadFromFile("Menu/menu.png"))
	{
		exit(1);
	}

	sprite->setTexture(*imagemFundo);
	sprite->setScale(2.1f, 2.1f);

	posMouse = { 0,0 };
	coordenadasMouse = { 0,0 };

	inicializaBotoes();
	inicializaTextos();

	pushTela(1);
}

Menu::~Menu()
{

	delete fonte;
	delete imagemFundo;
	delete sprite;

}

void Menu::executar()
{
	while (gerenciador_grafico->getOpen())
	{
		gerenciador_grafico->setCentro(posicaoTela);

		if (!telaAtual.empty())
		{
			int tela = telaAtual.top();
			int evento;

			switch (tela)
			{
			case 1:
				evento = telaInicial.verificaEventoTela();

				if (evento == 0)         // Novo Jogo
				{
					telaAtual.push(2);
				}
				else if (evento == 1)    // Continuar
				{
					telaAtual.push(3);
				}
				else if (evento == 2)    // Habilidades
				{
					executarTelaHabilidades();
				}
				else if (evento == 3)    // Ranking
				{
					telaAtual.push(4);
				}
				else if (evento == 4)    // Configuracoes
				{
					executarTelaConfiguracoes();
				}
				else if (evento == 5)    // Sair
				{
					gerenciador_grafico->fecharJanela();
					exit(1);
				}

				gerenciador_grafico->limpaTela();
				gerenciador_grafico->desenhaSprite(*sprite);
				telaInicial.desenharTela();

				break;
			case 2:
				evento = tela1.verificaEventoTela();

				if (evento == 0 || evento == 1)
				{
					n_jogadores = (evento == 0) ? 1 : 2;
					telaGameOver.setEntradaAtiva(true);
					telaGameOver.setEntradaAtiva2(n_jogadores == 2);

					// Modo roguelike: nao ha mais escolha de fase - vai
					// direto para a primeira fase procedural.
					objPrincipal.executarFase(1, n_jogadores);

					while (!telaAtual.empty())
						popTela();
					pushTela(6);
				}
				else if (evento == 2)
				{
					popTela();
				}

				gerenciador_grafico->limpaTela();
				gerenciador_grafico->desenhaSprite(*sprite);
				tela1.desenharTela();

				break;
			case 3:
				// tela2 reaproveitada como seletor de slot de save para
				// retomar uma run salva. Botoes 0/1/2 = saves 1/2/3.
				evento = tela2.verificaEventoTela();
				if (evento >= 0 && evento <= 2)
				{
					// Para recuperar, precisamos saber quantos jogadores;
					// usamos 1 como padrao (o save guarda numJogadores).
					n_jogadores = 1;
					telaGameOver.setEntradaAtiva(true);
					telaGameOver.setEntradaAtiva2(false);

					objPrincipal.recuperaFase(evento + 1);

					while (!telaAtual.empty())
						popTela();
					pushTela(6);
				}
				else if (evento == 3)
				{
					popTela();
				}

				gerenciador_grafico->limpaTela();
				gerenciador_grafico->desenhaSprite(*sprite);
				tela2.desenharTela();

				break;
			case 4:
				evento = tela3.verificaEventoTela();

				gerenciador_grafico->limpaTela();

				mostrarRanking();


				if (evento == 0)
				{
					limparRanking();
				}
				else if (evento == 1)
				{
					popTela();
				}

				gerenciador_grafico->desenhaSprite(*sprite);


				for (int i = 0; i < ranking.size(); i++)
				{
					ranking.at(i).setFont(*fonte);
					ranking.at(i).setCharacterSize(39);
					ranking.at(i).setOutlineColor(Color::Black);
					ranking.at(i).setOutlineThickness(0);
					ranking.at(i).setPosition(500, 50 + 40 * i);

					gerenciador_grafico->desenhaTexto(ranking.at(i));
				}

				tela3.desenharTela();

				break;
			case 5:
				evento = tela4.verificaEventoTela();

				if (n_jogadores == 1)
				{
					telaGameOver.setEntradaAtiva(true);
					telaGameOver.setEntradaAtiva2(false);
				}
				else if (n_jogadores == 2)
				{
					telaGameOver.setEntradaAtiva(true);
					telaGameOver.setEntradaAtiva2(true);
				}

				if (evento == 0)
				{
					objPrincipal.executarFase(1, n_jogadores);

					while (!telaAtual.empty())
					{
						popTela();
					}
					pushTela(6);

				}
				else if (evento == 1)
				{
					objPrincipal.executarFase(2, n_jogadores);

					while (!telaAtual.empty())
					{
						popTela();
					}
					pushTela(6);
				}
				else if (evento == 2)
				{
					popTela();
				}

				gerenciador_grafico->limpaTela();
				gerenciador_grafico->desenhaSprite(*sprite);
				tela4.desenharTela();

				break;

			case 6:
				evento = telaGameOver.verificaEventoTela();

				if (evento == 0)
				{
					objPrincipal.getMundo().gravarRanking(telaGameOver.coletarNome(), telaGameOver.coletarNome2());

					while (!telaAtual.empty())
					{
						popTela();
					}
					pushTela(1);
					pushTela(4);
				}
				else if (evento == 1)
				{
					while (!telaAtual.empty())
					{
						popTela();
					}
					pushTela(1);
				}

				gerenciador_grafico->limpaTela();
				gerenciador_grafico->desenhaSprite(*sprite);
				telaGameOver.desenharTela();

				break;
			default:
				exit(1);
				break;
			}
		}

		gerenciador_grafico->mostraElemento();
	}
}

void Menu::inicializaTextos()
{
	Text novoTexto;

	std::vector<const char*> opcoes;
	std::vector<sf::Vector2f> coordenadas;
	std::vector<std::size_t> tamanhos;

	opcoes = { "Novo Jogo", "Continuar", "Habilidades", "Ranking", "Configuracoes", "Sair" };
	coordenadas = { { 70, 250}, { 70, 320 }, { 70, 390 }, { 70, 460 }, { 70, 530 }, { 70, 600 } };
	tamanhos = { 38, 38, 38, 38, 38, 38 };

	for (size_t i = 0; i < opcoes.size(); i++)
	{

		novoTexto.setFont(*fonte);
		novoTexto.setString(opcoes[i]);
		novoTexto.setPosition(coordenadas[i]);
		novoTexto.setCharacterSize(tamanhos[i]);
		novoTexto.setOutlineColor(Color::Black);
		novoTexto.setFillColor(Color::White);

		telaInicial.addTexto(novoTexto);
	}


	opcoes = { "1 Jogador", "2 Jogadores", "Voltar" };
	coordenadas = { { 70, 300}, { 70, 390 }, { 70, 480 } };
	tamanhos = { 38, 38, 38 };

	for (size_t i = 0; i < opcoes.size(); i++)
	{

		novoTexto.setFont(*fonte);
		novoTexto.setString(opcoes[i]);
		novoTexto.setPosition(coordenadas[i]);
		novoTexto.setCharacterSize(tamanhos[i]);
		novoTexto.setOutlineColor(Color::Black);

		tela1.addTexto(novoTexto);
	}


	opcoes = { "Save 1", "Save 2", "Save 3", "Voltar" };
	coordenadas = { { 70, 300}, { 70, 390 }, { 70, 480 }, { 70, 570 } };
	tamanhos = { 38, 38, 38, 38 };

	for (size_t i = 0; i < opcoes.size(); i++)
	{

		novoTexto.setFont(*fonte);
		novoTexto.setString(opcoes[i]);
		novoTexto.setPosition(coordenadas[i]);
		novoTexto.setCharacterSize(tamanhos[i]);
		novoTexto.setOutlineColor(Color::Black);

		tela2.addTexto(novoTexto);

	}


	opcoes = { "Limpar Ranking", "Voltar" };
	coordenadas = { { 70, 300}, {70, 390} };
	tamanhos = { 38, 38 };

	for (size_t i = 0; i < opcoes.size(); i++)
	{

		novoTexto.setFont(*fonte);
		novoTexto.setString(opcoes[i]);
		novoTexto.setPosition(coordenadas[i]);
		novoTexto.setCharacterSize(tamanhos[i]);
		novoTexto.setOutlineColor(Color::Black);

		tela3.addTexto(novoTexto);

	}

	opcoes = { "Fase 1", "Fase 2", "Voltar" };
	coordenadas = { { 70, 300}, { 70, 390 }, { 70, 480 } };
	tamanhos = { 38, 38, 38 };

	for (size_t i = 0; i < opcoes.size(); i++)
	{

		novoTexto.setFont(*fonte);
		novoTexto.setString(opcoes[i]);
		novoTexto.setPosition(coordenadas[i]);
		novoTexto.setCharacterSize(tamanhos[i]);
		novoTexto.setOutlineColor(Color::Black);

		tela4.addTexto(novoTexto);

	}

	opcoes = { "Gravar pontuacao", "Sair"};
	coordenadas = { { 70, 300}, { 70, 390 }};
	tamanhos = { 38, 38 };

	for (size_t i = 0; i < opcoes.size(); i++)
	{

		novoTexto.setFont(*fonte);
		novoTexto.setString(opcoes[i]);
		novoTexto.setPosition(coordenadas[i]);
		novoTexto.setCharacterSize(tamanhos[i]);
		novoTexto.setOutlineColor(Color::Black);

		telaGameOver.addTexto(novoTexto);

	}
}

void Menu::inicializaBotoes()
{
	telaGameOver.configurarTextoEntrada(*fonte, 50, Color::Black, sf::Vector2f(600, 605));
	telaGameOver.configurarCampoTexto(sf::Vector2f(800, 70), Color::White, sf::Vector2f(600, 600));
	telaGameOver.setTextoCaixa(*fonte, 35, Vector2f(180, 620), "Nome do Jogador 1:");

	telaGameOver.configurarTextoEntrada2(*fonte, 50, Color::Black, sf::Vector2f(600, 705));
	telaGameOver.configurarCampoTexto2(sf::Vector2f(800, 70), Color::White, sf::Vector2f(600, 700));
	telaGameOver.setTextoCaixa2(*fonte, 35, Vector2f(180, 720), "Nome do Jogador 2:");



	sf::RectangleShape* novoJogo;
	sf::RectangleShape* continuar;
	sf::RectangleShape* habilidades;
	sf::RectangleShape* ranking;
	sf::RectangleShape* configuracoes;
	sf::RectangleShape* sair;

	sf::RectangleShape* umJogador;
	sf::RectangleShape* doisJogadores;
	sf::RectangleShape* voltar;

	sf::RectangleShape* mundo1;
	sf::RectangleShape* mundo2;
	sf::RectangleShape* mundo3;
	sf::RectangleShape* voltar2;


	sf::RectangleShape* limparRanking;
	sf::RectangleShape* voltar3;

	sf::RectangleShape* fase1;
	sf::RectangleShape* fase2;
	sf::RectangleShape* voltar4;

	sf::RectangleShape* gravarJogada;
	sf::RectangleShape* sair1;

	//TELA INICIAL
	novoJogo = new sf::RectangleShape();
	novoJogo->setSize(sf::Vector2f(250.0f, 50.0f));
	novoJogo->setPosition(sf::Vector2f(70, 250));
	novoJogo->setFillColor(sf::Color::Red);

	continuar = new sf::RectangleShape();
	continuar->setSize(sf::Vector2f(240.0f, 50.0f));
	continuar->setPosition(sf::Vector2f(70, 320));
	continuar->setFillColor(sf::Color::Red);

	habilidades = new sf::RectangleShape();
	habilidades->setSize(sf::Vector2f(270.0f, 50.0f));
	habilidades->setPosition(sf::Vector2f(70, 390));
	habilidades->setFillColor(sf::Color::Red);

	ranking = new sf::RectangleShape();
	ranking->setSize(sf::Vector2f(190.0f, 50.0f));
	ranking->setPosition(sf::Vector2f(70, 460));
	ranking->setFillColor(sf::Color::Red);

	configuracoes = new sf::RectangleShape();
	configuracoes->setSize(sf::Vector2f(310.0f, 50.0f));
	configuracoes->setPosition(sf::Vector2f(70, 530));
	configuracoes->setFillColor(sf::Color::Red);

	sair = new sf::RectangleShape();
	sair->setSize(sf::Vector2f(110.0f, 50.0f));
	sair->setPosition(sf::Vector2f(70, 600));
	sair->setFillColor(sf::Color::Red);

	telaInicial.addBotao(novoJogo);
	telaInicial.addBotao(continuar);
	telaInicial.addBotao(habilidades);
	telaInicial.addBotao(ranking);
	telaInicial.addBotao(configuracoes);
	telaInicial.addBotao(sair);

	//TELA 1
	umJogador = new sf::RectangleShape();
	umJogador->setSize(sf::Vector2f(225.0f, 50.0f));
	umJogador->setPosition(sf::Vector2f(70, 300));
	umJogador->setFillColor(sf::Color::Red);

	doisJogadores = new sf::RectangleShape();
	doisJogadores->setSize(sf::Vector2f(285.0f, 50.0f));
	doisJogadores->setPosition(sf::Vector2f(70, 390));
	doisJogadores->setFillColor(sf::Color::Red);

	voltar = new sf::RectangleShape();
	voltar->setSize(sf::Vector2f(155.0f, 50.0f));
	voltar->setPosition(sf::Vector2f(70, 480));
	voltar->setFillColor(sf::Color::Red);

	tela1.addBotao(umJogador);
	tela1.addBotao(doisJogadores);
	tela1.addBotao(voltar);


	//TELA 2
	mundo1 = new sf::RectangleShape();
	mundo1->setSize(sf::Vector2f(190.0f, 50.0f));
	mundo1->setPosition(sf::Vector2f(70, 300));
	mundo1->setFillColor(sf::Color::Red);

	mundo2 = new sf::RectangleShape();
	mundo2->setSize(sf::Vector2f(190.0f, 50.0f));
	mundo2->setPosition(sf::Vector2f(70, 390));
	mundo2->setFillColor(sf::Color::Red);

	mundo3 = new sf::RectangleShape();
	mundo3->setSize(sf::Vector2f(190.0f, 50.0f));
	mundo3->setPosition(sf::Vector2f(70, 480));
	mundo3->setFillColor(sf::Color::Red);

	voltar2 = new sf::RectangleShape();
	voltar2->setSize(sf::Vector2f(155.0f, 50.0f));
	voltar2->setPosition(sf::Vector2f(70, 570));
	voltar2->setFillColor(sf::Color::Red);


	tela2.addBotao(mundo1);
	tela2.addBotao(mundo2);
	tela2.addBotao(mundo3);
	tela2.addBotao(voltar2);


	//TELA 3
	limparRanking = new sf::RectangleShape();
	limparRanking->setSize(sf::Vector2f(380.0f, 50.0f));
	limparRanking->setPosition(sf::Vector2f(70, 300));
	limparRanking->setFillColor(sf::Color::Red);

	voltar3 = new sf::RectangleShape();
	voltar3->setSize(sf::Vector2f(155.0f, 50.0f));
	voltar3->setPosition(sf::Vector2f(70, 390));
	voltar3->setFillColor(sf::Color::Red);

	tela3.addBotao(limparRanking);
	tela3.addBotao(voltar3);

	//TELA 4
	fase1 = new sf::RectangleShape();
	fase1->setSize(sf::Vector2f(155.0f, 50.0f));
	fase1->setPosition(sf::Vector2f(70, 300));
	fase1->setFillColor(sf::Color::Red);

	fase2 = new sf::RectangleShape();
	fase2->setSize(sf::Vector2f(155.0f, 50.0f));
	fase2->setPosition(sf::Vector2f(70, 390));
	fase2->setFillColor(sf::Color::Red);

	voltar4 = new sf::RectangleShape();
	voltar4->setSize(sf::Vector2f(155.0f, 50.0f));
	voltar4->setPosition(sf::Vector2f(70, 480));
	voltar4->setFillColor(sf::Color::Red);

	tela4.addBotao(fase1);
	tela4.addBotao(fase2);
	tela4.addBotao(voltar4);

	//TELA 4

	gravarJogada = new sf::RectangleShape();
	gravarJogada->setSize(sf::Vector2f(420.0f, 50.0f));
	gravarJogada->setPosition(sf::Vector2f(70, 300));
	gravarJogada->setFillColor(sf::Color::Red);

	sair1 = new sf::RectangleShape();
	sair1->setSize(sf::Vector2f(120.0f, 50.0f));
	sair1->setPosition(sf::Vector2f(70, 390));
	sair1->setFillColor(sf::Color::Red);

	telaGameOver.addBotao(gravarJogada);
	telaGameOver.addBotao(sair1);
}

void Menu::pushTela(int tela)
{
	telaAtual.push(tela);
}

void Menu::popTela()
{
	if (!telaAtual.empty())
	{
		telaAtual.pop();
	}
}

void Menu::mostrarRanking()
{
	std::ifstream arquivo("Saves/ranking.txt");
	std::vector<PlayerScore> scores; 

	if (arquivo.is_open())
	{
		std::string linha;
		while (std::getline(arquivo, linha))
		{
			size_t pos = linha.find('-');
			if (pos != std::string::npos)
			{
				std::string scoreStr = linha.substr(0, pos);
				std::string name = linha.substr(pos + 1);

				int score;
				std::istringstream iss(scoreStr);
				if (iss >> score)
				{
					scores.push_back({ score, name });
				}
			}
		}

		arquivo.close();

		std::sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b)
			{
				return a.score > b.score;
			});

		ranking.clear(); 

		for (const PlayerScore& player : scores)
		{
			sf::Text texto;
			texto.setString(std::to_string(player.score) + " - " + player.name);
			ranking.push_back(texto);
		}
	}
	else
	{
		std::cerr << "Erro ao abrir o arquivo de ranking." << std::endl;
	}
}

void Menu::limparRanking()
{
	std::ofstream arquivo("Saves/ranking.txt", std::ofstream::out);

	if (arquivo.is_open())
	{
		arquivo.close();
	}
}

namespace
{
	// Helper para o desenho das telas de configuracao: monta um botao
	// (retangulo + texto) em uma posicao, devolvendo o retangulo para
	// teste de colisao do mouse e o texto pronto para desenhar.
	struct ItemConfig
	{
		sf::RectangleShape retangulo;
		sf::Text rotulo;
	};

	ItemConfig criarItem(const sf::Font& fonte, const std::string& texto,
		sf::Vector2f pos, sf::Vector2f tamanho, unsigned int tamFonte = 32)
	{
		ItemConfig item;
		item.retangulo.setPosition(pos);
		item.retangulo.setSize(tamanho);
		item.retangulo.setFillColor(sf::Color(0, 0, 0, 160));
		item.retangulo.setOutlineColor(sf::Color::White);
		item.retangulo.setOutlineThickness(1.5f);

		item.rotulo.setFont(fonte);
		item.rotulo.setString(texto);
		item.rotulo.setCharacterSize(tamFonte);
		item.rotulo.setOutlineColor(sf::Color::Black);
		item.rotulo.setFillColor(sf::Color::White);
		item.rotulo.setPosition(pos.x + 10.0f, pos.y + 5.0f);
		return item;
	}
}

void Menu::executarTelaConfiguracoes()
{
	auto* cfg = Gerenciadores::Configuracao::getInstancia();

	sf::Text titulo;
	titulo.setFont(*fonte);
	titulo.setString("Configuracoes");
	titulo.setPosition(470, 70);
	titulo.setCharacterSize(75);
	titulo.setOutlineColor(sf::Color::Black);
	titulo.setStyle(sf::Text::Bold);

	ItemConfig btnControles = criarItem(*fonte, "Controles", {70, 300}, {300, 50}, 38);
	ItemConfig btnTela = criarItem(*fonte, "Tela", {70, 390}, {300, 50}, 38);
	ItemConfig btnVoltar = criarItem(*fonte, "Voltar", {70, 480}, {300, 50}, 38);

	while (gerenciador_grafico->getOpen())
	{
		sf::Event evento;
		while (gerenciador_grafico->getJanela()->pollEvent(evento))
		{
			if (evento.type == sf::Event::Closed)
			{
				gerenciador_grafico->fecharJanela();
				return;
			}

			if (evento.type == sf::Event::MouseButtonReleased &&
				evento.mouseButton.button == sf::Mouse::Left)
			{
				const sf::Vector2i mp = sf::Mouse::getPosition(*gerenciador_grafico->getJanela());
				const sf::Vector2f cm = gerenciador_grafico->getJanela()->mapPixelToCoords(mp);

				if (btnControles.retangulo.getGlobalBounds().contains(cm))
				{
					executarTelaControles();
				}
				else if (btnTela.retangulo.getGlobalBounds().contains(cm))
				{
					executarTelaTela();
				}
				else if (btnVoltar.retangulo.getGlobalBounds().contains(cm))
				{
					cfg->salvar();
					return;
				}
			}
		}

		gerenciador_grafico->limpaTela();
		gerenciador_grafico->desenhaSprite(*sprite);

		gerenciador_grafico->desenhaTexto(titulo);
		gerenciador_grafico->desenhaTela(&btnControles.retangulo);
		gerenciador_grafico->desenhaTexto(btnControles.rotulo);
		gerenciador_grafico->desenhaTela(&btnTela.retangulo);
		gerenciador_grafico->desenhaTexto(btnTela.rotulo);
		gerenciador_grafico->desenhaTela(&btnVoltar.retangulo);
		gerenciador_grafico->desenhaTexto(btnVoltar.rotulo);

		gerenciador_grafico->mostraElemento();
	}
}

void Menu::executarTelaControles()
{
	using Acao = Gerenciadores::Configuracao::Acao;
	auto* cfg = Gerenciadores::Configuracao::getInstancia();

	sf::Text titulo;
	titulo.setFont(*fonte);
	titulo.setString("Controles");
	titulo.setPosition(470, 70);
	titulo.setCharacterSize(75);
	titulo.setOutlineColor(sf::Color::Black);
	titulo.setStyle(sf::Text::Bold);

	// 8 acoes (4 por jogador) + Voltar. Cada linha possui um retangulo
	// clicavel que abre a captura da proxima tecla pressionada.
	struct Linha
	{
		int jogador;
		Acao acao;
		std::string descricao;
	};

	const Linha linhas[] = {
		{0, Acao::ESQUERDA, "P1 Esquerda"},
		{0, Acao::DIREITA, "P1 Direita"},
		{0, Acao::PULAR, "P1 Pulo"},
		{0, Acao::ATACAR, "P1 Ataque"},
		{1, Acao::ESQUERDA, "P2 Esquerda"},
		{1, Acao::DIREITA, "P2 Direita"},
		{1, Acao::PULAR, "P2 Pulo"},
		{1, Acao::ATACAR, "P2 Ataque"},
	};

	int aguardando = -1; // indice da linha aguardando tecla, ou -1
	sf::Text aviso;
	aviso.setFont(*fonte);
	aviso.setCharacterSize(26);
	aviso.setFillColor(sf::Color::Yellow);
	aviso.setPosition(60, 220);

	while (gerenciador_grafico->getOpen())
	{
		// Reconstroi as caixas a cada frame para refletir mudancas
		// imediatas no rotulo da tecla apos a captura.
		std::vector<ItemConfig> itens;
		for (std::size_t i = 0; i < sizeof(linhas) / sizeof(linhas[0]); ++i)
		{
			const std::string texto = linhas[i].descricao + ":  " +
				Gerenciadores::Configuracao::nomeTecla(cfg->getTecla(linhas[i].jogador, linhas[i].acao));
			const float y = 280.0f + i * 60.0f;
			itens.push_back(criarItem(*fonte, texto, {60, y}, {520, 48}, 28));
		}
		ItemConfig btnVoltar = criarItem(*fonte, "Voltar",
			{60, 280.0f + sizeof(linhas) / sizeof(linhas[0]) * 60.0f + 20.0f},
			{300, 48}, 32);

		if (aguardando >= 0)
		{
			itens[aguardando].rotulo.setString(
				linhas[aguardando].descricao + ":  [pressione uma tecla]");
			itens[aguardando].rotulo.setFillColor(sf::Color::Yellow);
		}

		sf::Event evento;
		while (gerenciador_grafico->getJanela()->pollEvent(evento))
		{
			if (evento.type == sf::Event::Closed)
			{
				gerenciador_grafico->fecharJanela();
				return;
			}

			if (aguardando >= 0)
			{
				// Aguardando uma tecla: a proxima KeyPressed remapeia.
				// ESC cancela sem alterar.
				if (evento.type == sf::Event::KeyPressed)
				{
					if (evento.key.code != sf::Keyboard::Escape)
					{
						cfg->setTecla(linhas[aguardando].jogador,
							linhas[aguardando].acao, evento.key.code);
					}
					aguardando = -1;
				}
				continue;
			}

			if (evento.type == sf::Event::MouseButtonReleased &&
				evento.mouseButton.button == sf::Mouse::Left)
			{
				const sf::Vector2i mp = sf::Mouse::getPosition(*gerenciador_grafico->getJanela());
				const sf::Vector2f cm = gerenciador_grafico->getJanela()->mapPixelToCoords(mp);

				bool clicouLinha = false;
				for (std::size_t i = 0; i < itens.size(); ++i)
				{
					if (itens[i].retangulo.getGlobalBounds().contains(cm))
					{
						aguardando = static_cast<int>(i);
						clicouLinha = true;
						break;
					}
				}

				if (!clicouLinha && btnVoltar.retangulo.getGlobalBounds().contains(cm))
				{
					cfg->salvar();
					return;
				}
			}
		}

		aviso.setString(aguardando >= 0
			? std::string("Pressione a nova tecla (ESC para cancelar)")
			: std::string("Clique em uma acao para remapear"));

		gerenciador_grafico->limpaTela();
		gerenciador_grafico->desenhaSprite(*sprite);
		gerenciador_grafico->desenhaTexto(titulo);
		gerenciador_grafico->desenhaTexto(aviso);

		for (auto& it : itens)
		{
			gerenciador_grafico->desenhaTela(&it.retangulo);
			gerenciador_grafico->desenhaTexto(it.rotulo);
		}
		gerenciador_grafico->desenhaTela(&btnVoltar.retangulo);
		gerenciador_grafico->desenhaTexto(btnVoltar.rotulo);

		gerenciador_grafico->mostraElemento();
	}
}

void Menu::executarTelaHabilidades(Mundo* mundoExterno)
{
	// Usa o Mundo de Principal por padrao (acumulado entre runs); pode
	// receber um Mundo externo (ex.: chamado a partir do menu de pausa
	// com o Mundo da run em curso).
	abrirTelaHabilidades(mundoExterno ? *mundoExterno : objPrincipal.getMundo());
}

void Menu::abrirTelaHabilidades(Mundo& mundoRef)
{
	auto* gerGraf = Gerenciadores::Gerenciador_Grafico::getGerenciador();
	const sf::Font& fonte = Gerenciadores::Gerenciador_Recursos::getGerenciador()
		->getFonte("Menu/antiquity-print.ttf");

	ArvoreHabilidades& arvore = mundoRef.getArvore();

	sf::Text titulo;
	titulo.setFont(fonte);
	titulo.setString("Habilidades");
	titulo.setPosition(470, 30);
	titulo.setCharacterSize(60);
	titulo.setOutlineColor(sf::Color::Black);
	titulo.setStyle(sf::Text::Bold);

	// Backdrop semi-transparente em vez do sprite do menu (este metodo
	// pode ser chamado durante o jogo, sem o sprite do menu disponivel).
	sf::RectangleShape backdrop;
	backdrop.setSize(sf::Vector2f(TELA_X, TELA_Y));
	backdrop.setFillColor(sf::Color(0, 0, 0, 220));

	while (gerGraf->getOpen())
	{
		// Vista em coordenadas de tela enquanto a skill tree esta aberta.
		auto* janela = gerGraf->getJanela();
		const sf::View viewAnterior = janela->getView();
		sf::View viewUI(sf::FloatRect(0.0f, 0.0f, TELA_X, TELA_Y));
		janela->setView(viewUI);

		const int pontos = arvore.getPontos();

		sf::Text infoPontos;
		infoPontos.setFont(fonte);
		infoPontos.setString("Pontos disponiveis: " + std::to_string(pontos));
		infoPontos.setCharacterSize(28);
		infoPontos.setFillColor(sf::Color::Yellow);
		infoPontos.setOutlineColor(sf::Color::Black);
		infoPontos.setPosition(70, 130);

		std::vector<ItemConfig> linhas;
		std::vector<ArvoreHabilidades::Habilidade> mapHabilidade;
		const float yBase = 200.0f;
		const float passoY = 60.0f;

		for (int i = 0; i < ArvoreHabilidades::N_HABILIDADES; ++i)
		{
			auto h = static_cast<ArvoreHabilidades::Habilidade>(i);
			const auto& info = ArvoreHabilidades::getInfo(h);
			const bool desbloqueada = arvore.foiDesbloqueada(h);
			const bool podeComprar = !desbloqueada && pontos >= info.custo;

			std::string sufixo;
			if (desbloqueada)
				sufixo = "  [DESBLOQUEADA]";
			else
				sufixo = "  [custo " + std::to_string(info.custo) + "]";

			const std::string texto = std::string(info.nome) + " - " + info.descricao + sufixo;
			ItemConfig item = criarItem(fonte, texto,
				{60, yBase + i * passoY}, {1280, 50}, 22);

			if (desbloqueada)
				item.rotulo.setFillColor(sf::Color(120, 255, 120));
			else if (podeComprar)
				item.rotulo.setFillColor(sf::Color::White);
			else
				item.rotulo.setFillColor(sf::Color(180, 180, 180));

			linhas.push_back(item);
			mapHabilidade.push_back(h);
		}

		ItemConfig btnVoltar = criarItem(fonte, "Voltar",
			{60, yBase + ArvoreHabilidades::N_HABILIDADES * passoY + 20},
			{260, 50}, 32);

		sf::Event evento;
		while (janela->pollEvent(evento))
		{
			if (evento.type == sf::Event::Closed)
			{
				gerGraf->fecharJanela();
				janela->setView(viewAnterior);
				return;
			}

			if (evento.type == sf::Event::MouseButtonReleased &&
				evento.mouseButton.button == sf::Mouse::Left)
			{
				const sf::Vector2i mp = sf::Mouse::getPosition(*janela);
				const sf::Vector2f cm = janela->mapPixelToCoords(mp);

				bool clicouLinha = false;
				for (std::size_t i = 0; i < linhas.size(); ++i)
				{
					if (linhas[i].retangulo.getGlobalBounds().contains(cm))
					{
						arvore.comprar(mapHabilidade[i]);
						clicouLinha = true;
						break;
					}
				}

				if (!clicouLinha && btnVoltar.retangulo.getGlobalBounds().contains(cm))
				{
					janela->setView(viewAnterior);
					return;
				}
			}
		}

		gerGraf->limpaTela();
		gerGraf->desenhaTela(&backdrop);
		gerGraf->desenhaTexto(titulo);
		gerGraf->desenhaTexto(infoPontos);
		for (auto& it : linhas)
		{
			gerGraf->desenhaTela(&it.retangulo);
			gerGraf->desenhaTexto(it.rotulo);
		}
		gerGraf->desenhaTela(&btnVoltar.retangulo);
		gerGraf->desenhaTexto(btnVoltar.rotulo);

		gerGraf->mostraElemento();

		// Restaura a view do chamador antes de polling de proxima vez.
		janela->setView(viewAnterior);
	}
}

void Menu::executarTelaTela()
{
	auto* cfg = Gerenciadores::Configuracao::getInstancia();

	// Resolucoes oferecidas. Mantemos uma lista curta para evitar
	// confusao com modos exoticos; a opcao tela cheia desce ao modo
	// nativo do monitor automaticamente quando precisa.
	struct Resolucao { unsigned int largura; unsigned int altura; };
	const Resolucao resolucoes[] = {
		{1280, 720}, {1366, 768}, {1600, 900}, {1920, 1080}, {2560, 1440}
	};
	const std::size_t nResolucoes = sizeof(resolucoes) / sizeof(resolucoes[0]);

	const unsigned int fpsOpcoes[] = {30, 60, 75, 120, 144, 240};
	const std::size_t nFps = sizeof(fpsOpcoes) / sizeof(fpsOpcoes[0]);

	auto indiceResolucao = [&]() -> std::size_t {
		for (std::size_t i = 0; i < nResolucoes; ++i)
		{
			if (resolucoes[i].largura == cfg->getLargura() &&
				resolucoes[i].altura == cfg->getAltura())
				return i;
		}
		return 2; // default 1600x900
	};

	auto indiceFps = [&]() -> std::size_t {
		for (std::size_t i = 0; i < nFps; ++i)
			if (fpsOpcoes[i] == cfg->getFpsMax())
				return i;
		return 1; // default 60
	};

	std::size_t idxRes = indiceResolucao();
	std::size_t idxFps = indiceFps();
	bool tcheia = cfg->getTelaCheia();

	sf::Text titulo;
	titulo.setFont(*fonte);
	titulo.setString("Tela");
	titulo.setPosition(470, 70);
	titulo.setCharacterSize(75);
	titulo.setOutlineColor(sf::Color::Black);
	titulo.setStyle(sf::Text::Bold);

	while (gerenciador_grafico->getOpen())
	{
		const std::string txtRes = "Resolucao:  " +
			std::to_string(resolucoes[idxRes].largura) + " x " +
			std::to_string(resolucoes[idxRes].altura);
		const std::string txtTC = std::string("Tela cheia:  ") + (tcheia ? "Sim" : "Nao");
		const std::string txtFps = "FPS maximo:  " + std::to_string(fpsOpcoes[idxFps]);

		ItemConfig itemRes = criarItem(*fonte, txtRes, {70, 280}, {520, 50}, 32);
		ItemConfig itemTC = criarItem(*fonte, txtTC, {70, 360}, {520, 50}, 32);
		ItemConfig itemFps = criarItem(*fonte, txtFps, {70, 440}, {520, 50}, 32);
		ItemConfig btnAplicar = criarItem(*fonte, "Aplicar", {70, 540}, {220, 50}, 32);
		ItemConfig btnVoltar = criarItem(*fonte, "Voltar", {310, 540}, {220, 50}, 32);

		sf::Text dica;
		dica.setFont(*fonte);
		dica.setString("(clique em cada opcao para alternar)");
		dica.setCharacterSize(22);
		dica.setFillColor(sf::Color::White);
		dica.setOutlineColor(sf::Color::Black);
		dica.setPosition(70, 230);

		sf::Event evento;
		while (gerenciador_grafico->getJanela()->pollEvent(evento))
		{
			if (evento.type == sf::Event::Closed)
			{
				gerenciador_grafico->fecharJanela();
				return;
			}

			if (evento.type == sf::Event::MouseButtonReleased &&
				evento.mouseButton.button == sf::Mouse::Left)
			{
				const sf::Vector2i mp = sf::Mouse::getPosition(*gerenciador_grafico->getJanela());
				const sf::Vector2f cm = gerenciador_grafico->getJanela()->mapPixelToCoords(mp);

				if (itemRes.retangulo.getGlobalBounds().contains(cm))
					idxRes = (idxRes + 1) % nResolucoes;
				else if (itemTC.retangulo.getGlobalBounds().contains(cm))
					tcheia = !tcheia;
				else if (itemFps.retangulo.getGlobalBounds().contains(cm))
					idxFps = (idxFps + 1) % nFps;
				else if (btnAplicar.retangulo.getGlobalBounds().contains(cm))
				{
					cfg->setResolucao(resolucoes[idxRes].largura, resolucoes[idxRes].altura);
					cfg->setTelaCheia(tcheia);
					cfg->setFpsMax(fpsOpcoes[idxFps]);
					cfg->salvar();
					gerenciador_grafico->aplicarConfiguracao();
				}
				else if (btnVoltar.retangulo.getGlobalBounds().contains(cm))
				{
					cfg->salvar();
					return;
				}
			}
		}

		gerenciador_grafico->limpaTela();
		gerenciador_grafico->desenhaSprite(*sprite);
		gerenciador_grafico->desenhaTexto(titulo);
		gerenciador_grafico->desenhaTexto(dica);
		gerenciador_grafico->desenhaTela(&itemRes.retangulo);
		gerenciador_grafico->desenhaTexto(itemRes.rotulo);
		gerenciador_grafico->desenhaTela(&itemTC.retangulo);
		gerenciador_grafico->desenhaTexto(itemTC.rotulo);
		gerenciador_grafico->desenhaTela(&itemFps.retangulo);
		gerenciador_grafico->desenhaTexto(itemFps.rotulo);
		gerenciador_grafico->desenhaTela(&btnAplicar.retangulo);
		gerenciador_grafico->desenhaTexto(btnAplicar.rotulo);
		gerenciador_grafico->desenhaTela(&btnVoltar.retangulo);
		gerenciador_grafico->desenhaTexto(btnVoltar.rotulo);

		gerenciador_grafico->mostraElemento();
	}
}
