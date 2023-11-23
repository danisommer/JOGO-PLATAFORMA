#include "../Headers/Principal.hpp"

Principal::Principal() : gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
fase1(),
fase2(),
derrota(false),
concluida(false),
salvar(false)
{
	inicializaMenu();
}

Principal::~Principal()
{
}

void Principal::executar()
{
}

void Principal::executarFase1(int n_jogadores)
{
	derrota = false;

	fase1.desalocaEntidades();
	fase1.instanciaEntidades("Fases/fase1-" + to_string(n_jogadores) + "p.txt");

	while (!fase1.getConcluida() && !fase1.getDerrota() && gerenciador_grafico->getOpen())
	{
		gerenciador_eventos->Executar();

		if (!gerenciador_eventos->getJogoPausado())
		{
			gerenciador_grafico->limpaTela();
			fase1.atualizaCamera();
			fase1.AtualizarPersonagens();
			gerenciador_colisoes->Executar();
			gerenciador_grafico->mostraElemento();
			fase1.verificaFase();

			if (fase1.getConcluida())
				concluida = true;
		}
		else
		{
			telaPausa.setPosX(gerenciador_grafico->getViewCenter().x - 700.0f);
			tituloPausa.setPosition(gerenciador_grafico->getViewCenter().x - 250.0f, 170);

			int opcao = exibirMenuPausa();

			if (salvar)
			{
				fase1.salvarJogo();
				salvar = false;
			}
				

		}
		fase1.setDerrota(derrota);

	}
}

void Principal::executarFase2(int n_jogadores)
{
	derrota = false;

	fase2.desalocaEntidades();
	fase2.instanciaEntidades("Fases/fase2-" + to_string(n_jogadores) + "p.txt");

	while (!fase2.getConcluida() && !fase2.getDerrota() && gerenciador_grafico->getOpen())
	{
		gerenciador_eventos->Executar();

		if (!gerenciador_eventos->getJogoPausado())
		{
			gerenciador_grafico->limpaTela();
			fase2.atualizaCamera();
			fase2.AtualizarPersonagens();
			gerenciador_colisoes->Executar();
			gerenciador_grafico->mostraElemento();
			fase2.verificaFase();
		}
		else
		{
			telaPausa.setPosX(gerenciador_grafico->getViewCenter().x - 700.0f);
			tituloPausa.setPosition(gerenciador_grafico->getViewCenter().x - 250.0f, 170);

			int opcao = exibirMenuPausa();

			if (salvar)
			{
				fase2.salvarJogo();
				salvar = false;
			}
				

		}
		fase2.setDerrota(derrota);

	}
	concluida = false;

}

int Principal::exibirMenuPausa()
{
	int opcao = -1;

	while (gerenciador_eventos->getJogoPausado())
	{
		opcao = telaPausa.verificaEventoTela();

		switch (opcao)
		{
		case 3:
			derrota = true;
			gerenciador_eventos->despausarJogo();
			break;
		case 0:
			gerenciador_eventos->despausarJogo();
			break;

		case 1:

			break;

		case 2:
			salvar = true;
			gerenciador_eventos->despausarJogo();
			break;
		}

		gerenciador_grafico->limpaTela();
		gerenciador_grafico->desenhaTexto(tituloPausa);
		telaPausa.desenharTela();
		gerenciador_grafico->mostraElemento();
	}
	
	return opcao;
}

void Principal::inicializaMenu()
{

	sf::Font* fonte = new sf::Font();
	if (!fonte->loadFromFile("Menu/antiquity-print.ttf"))
	{
		exit(1);
	}

	Text novoTexto;

	std::vector<const char*> opcoes;
	std::vector<sf::Vector2f> coordenadas;
	std::vector<std::size_t> tamanhos;

	sf::RectangleShape* continuar;
	sf::RectangleShape* recuperar;
	sf::RectangleShape* salvar;
	sf::RectangleShape* sair;

	opcoes = { "Continuar", "Recuperar ","Salvar", "Sair" };
	coordenadas = { { 300, 300}, { 300, 390 }, { 300, 480 }, { 300,570 } };
	tamanhos = { 30, 30, 30, 30 };

	for (size_t i = 0; i < opcoes.size(); i++)
	{

		novoTexto.setFont(*fonte);
		novoTexto.setString(opcoes[i]);
		novoTexto.setPosition(coordenadas[i]);
		novoTexto.setCharacterSize(tamanhos[i]);
		novoTexto.setOutlineColor(Color::White);
		novoTexto.setFillColor(Color::Black);

		telaPausa.addTexto(novoTexto);
	}
	telaPausa.setLarguraSelecionado(4);
	telaPausa.setLarguraPadrao(2);

	tituloPausa.setFont(*fonte);
	tituloPausa.setString("Jogo Pausado");
	tituloPausa.setPosition(1100, 170);
	tituloPausa.setCharacterSize(55);
	tituloPausa.setOutlineColor(Color::Black);
	tituloPausa.setFillColor(Color::White);

	continuar = new sf::RectangleShape();
	continuar->setSize(sf::Vector2f(200.0f, 50.0f));
	continuar->setPosition(sf::Vector2f(300, 300));
	continuar->setFillColor(sf::Color::Red);

	recuperar = new sf::RectangleShape();
	recuperar->setSize(sf::Vector2f(205.0f, 50.0f));
	recuperar->setPosition(sf::Vector2f(300, 390));
	recuperar->setFillColor(sf::Color::Red);

	salvar = new sf::RectangleShape();
	salvar->setSize(sf::Vector2f(145.0f, 50.0f));
	salvar->setPosition(sf::Vector2f(300, 480));
	salvar->setFillColor(sf::Color::Red);

	sair = new sf::RectangleShape();
	sair->setSize(sf::Vector2f(95.0f, 50.0f));
	sair->setPosition(sf::Vector2f(300, 570));
	sair->setFillColor(sf::Color::Red);

	telaPausa.addBotao(continuar);
	telaPausa.addBotao(recuperar);
	telaPausa.addBotao(salvar);
	telaPausa.addBotao(sair);
}

bool Principal::getConcluida()
{
	return concluida;
}
