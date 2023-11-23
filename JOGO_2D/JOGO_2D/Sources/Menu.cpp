#include "Menu.hpp"

Menu::Menu():
	posicaoTela(gerenciador_grafico->getViewCenter())
{


	fonte = new sf::Font();
	imagemFundo = new sf::Texture();
	sprite = new sf::Sprite();

	titulo.setFont(*fonte);
	titulo.setString("Knight's Quest");
	titulo.setPosition(470, 70);
	titulo.setCharacterSize(75);
	titulo.setOutlineColor(Color::Black);
	titulo.setStyle(sf::Text::Bold);



	if (!fonte->loadFromFile("Menu/antiquity-print.ttf"))
	{
		exit(1);
	}

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
				gerenciador_grafico->desenhaTexto(titulo);
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

				}
				else if (evento == 1)
				{

				}
				else if (evento == 2)
				{
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
				if (evento == 0)
				{
					popTela();
				}

				gerenciador_grafico->limpaTela();
				gerenciador_grafico->desenhaSprite(*sprite);
				tela3.desenharTela();

				break;
			case 5:
				evento = tela4.verificaEventoTela();
				if (evento == 0)
				{
					objPrincipal.executarFase1(n_jogadores);

					for (int i = 0; i < telaAtual.size(); i++)
					{
						popTela();
					}
					pushTela(1);
				}
				else if (evento == 1)
				{
					objPrincipal.executarFase2(n_jogadores);

					for (int i = 0; i < telaAtual.size(); i++)
					{
						popTela();
					}
					pushTela(1);
				}
				else if (evento == 2)
				{
					popTela();
				}

				gerenciador_grafico->limpaTela();
				gerenciador_grafico->desenhaSprite(*sprite);
				tela4.desenharTela();

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
	
	opcoes = { "Fase 1", "Fase 2", "Voltar"};
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
}

void Menu::inicializaBotoes()
{
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