#include "Menu.hpp"

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

				if (evento == 0)
				{
					telaAtual.push(2);
				}
				else if (evento == 1)
				{
					telaAtual.push(3);
				}
				else if (evento == 2)
				{
					telaAtual.push(4);
				}
				else if (evento == 3)
				{
					gerenciador_grafico->fecharJanela();
				}

				gerenciador_grafico->limpaTela();
				gerenciador_grafico->desenhaSprite(*sprite);
				telaInicial.desenharTela();

				break;
			case 2:
				evento = tela1.verificaEventoTela();

				if (evento == 0)
				{
					n_jogadores = 1;
					telaAtual.push(5);
				}
				else if (evento == 1)
				{
					n_jogadores = 2;
					telaAtual.push(5);
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
				evento = tela2.verificaEventoTela();
				if (evento == 0)
				{
					//objPrincipal.recuperaFase(1);
				}
				else if (evento == 1)
				{
					//objPrincipal.recuperaFase(2);
				}
				else if (evento == 2)
				{
					//objPrincipal.recuperaFase(3);
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

				if (evento == 0)
				{
					popTela();
					ordenado = false;
				}


				if (!ordenado)
				{
					mostrarRanking();
					ordenado = true;
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

					for (int i = 0; i < telaAtual.size(); i++)
					{
						popTela();
					}
					pushTela(6);

				}
				else if (evento == 1)
				{
					objPrincipal.executarFase(2, n_jogadores);

					for (int i = 0; i < telaAtual.size(); i++)
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

					Fases::Fase::gravarPontuacao(telaGameOver.coletarNome(), telaGameOver.coletarNome2());
				}
				else if (evento == 1)
				{
					for (int i = 0; i < telaAtual.size(); i++)
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

	opcoes = { "Novo Jogo", "Continuar", "Ranking", "Sair" };
	coordenadas = { { 70, 300}, { 70, 390 }, { 70, 480 }, { 70, 570 } };
	tamanhos = { 38, 38, 38, 38 };

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


	opcoes = { "Mundo 1", "Mundo 2","Mundo 3", "Voltar" };
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


	opcoes = { "Voltar" };
	coordenadas = { { 70, 300} };
	tamanhos = { 38 };

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
	telaGameOver.setTextoCaixa(*fonte, 35, Vector2f(180, 605), "Nome do Jogador 1:");

	telaGameOver.configurarTextoEntrada2(*fonte, 50, Color::Black, sf::Vector2f(600, 705));
	telaGameOver.configurarCampoTexto2(sf::Vector2f(800, 70), Color::White, sf::Vector2f(600, 700));
	telaGameOver.setTextoCaixa2(*fonte, 35, Vector2f(180, 705), "Nome do Jogador 2:");



	sf::RectangleShape* novoJogo;
	sf::RectangleShape* continuar;
	sf::RectangleShape* ranking;
	sf::RectangleShape* sair;

	sf::RectangleShape* umJogador;
	sf::RectangleShape* doisJogadores;
	sf::RectangleShape* voltar;

	sf::RectangleShape* mundo1;
	sf::RectangleShape* mundo2;
	sf::RectangleShape* mundo3;
	sf::RectangleShape* voltar2;

	sf::RectangleShape* voltar3;

	sf::RectangleShape* fase1;
	sf::RectangleShape* fase2;
	sf::RectangleShape* voltar4;

	sf::RectangleShape* gravarJogada;
	sf::RectangleShape* sair1;

	//TELA INICIAL
	novoJogo = new sf::RectangleShape();
	novoJogo->setSize(sf::Vector2f(250.0f, 50.0f));
	novoJogo->setPosition(sf::Vector2f(70, 300));
	novoJogo->setFillColor(sf::Color::Red);

	continuar = new sf::RectangleShape();
	continuar->setSize(sf::Vector2f(240.0f, 50.0f));
	continuar->setPosition(sf::Vector2f(70, 390));
	continuar->setFillColor(sf::Color::Red);

	ranking = new sf::RectangleShape();
	ranking->setSize(sf::Vector2f(190.0f, 50.0f));
	ranking->setPosition(sf::Vector2f(70, 480));
	ranking->setFillColor(sf::Color::Red);

	sair = new sf::RectangleShape();
	sair->setSize(sf::Vector2f(110.0f, 50.0f));
	sair->setPosition(sf::Vector2f(70, 570));
	sair->setFillColor(sf::Color::Red);

	telaInicial.addBotao(novoJogo);
	telaInicial.addBotao(continuar);
	telaInicial.addBotao(ranking);
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
	voltar3 = new sf::RectangleShape();
	voltar3->setSize(sf::Vector2f(155.0f, 50.0f));
	voltar3->setPosition(sf::Vector2f(70, 300));
	voltar3->setFillColor(sf::Color::Red);

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
