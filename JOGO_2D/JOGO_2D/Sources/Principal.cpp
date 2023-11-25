#include "../Headers/Principal.hpp"

Principal::Principal() : gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
fase1(),
fase2(),
derrota(false),
concluida(false),
salvar(false),
carregar(false)
{

	fonte = new sf::Font();
	if (!fonte->loadFromFile("Menu/antiquity-print.ttf"))
	{
		exit(1);
	}

	inicializaMenu();
	inicializaMundos();


	textoCarregamento.setFont(*fonte);
	textoCarregamento.setString("Carregando...");
	textoCarregamento.setPosition(50, 800);
	textoCarregamento.setCharacterSize(50);
	textoCarregamento.setOutlineColor(Color::Black);
	textoCarregamento.setStyle(sf::Text::Bold);
	textoCarregamento.setFillColor(Color::White);

}

Principal::~Principal()
{
}
void Principal::telaCarregamento()
{
	textoCarregamento.setPosition(gerenciador_grafico->getViewCenter().x - 750.0f, textoCarregamento.getPosition().y);
	gerenciador_grafico->limpaTela();
	gerenciador_grafico->desenhaTexto(textoCarregamento);
	gerenciador_grafico->mostraElemento();
}

void Principal::alocaFase1(int n_jogadores)
{
	fase1.desalocaEntidades();
	fase1.instanciaEntidades("Fases/fase1-" + to_string(n_jogadores) + "p.txt");
}

void Principal::alocaFase2(int n_jogadores)
{
	fase2.desalocaEntidades();
	fase2.instanciaEntidades("Fases/fase2-" + to_string(n_jogadores) + "p.txt");
}

void Principal::recuperaFase(int save)
{
	std::ifstream arquivo("Saves/save" + std::to_string(save) + ".txt");

	if (arquivo.is_open())
	{
		int f;
		int nj;

		arquivo >> f;
		arquivo >> nj;

		if (f == 1)
		{
			fase1.recuperarJogo(save, f, nj);
		}
		else if (f == 2)
		{
			fase2.recuperarJogo(save, f, nj);
		}

		arquivo.close();

		executarFase(f, nj);
	}
	else
	{
		cout << "save inexistente" << endl;
	}

	
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
			carregar = true;
			gerenciador_eventos->despausarJogo();

			break;

		case 2:
			salvar = true;
			gerenciador_eventos->despausarJogo();

			break;
		}

		gerenciador_grafico->limpaTela();
		telaPausa.desenharTela();
		gerenciador_grafico->mostraElemento();
	}
	
	return opcao;
}

void Principal::executarFase(int fase, int n_jogadores)
{
	Fases::Fase::ZeraPontuacao();

	if (fase == 1)
	{
		telaCarregamento();

		derrota = false;

		alocaFase1(n_jogadores);

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
					fase = 2;
			}
			else
			{
				telaPausa.setPosX(gerenciador_grafico->getViewCenter().x - 700.0f);

				int opcao = exibirMenuPausa();

				if (salvar)
				{
					telaMundos.setPosX(gerenciador_grafico->getViewCenter().x - 700.0f);

					int op = exibirMenuMundos();
					op++;
					fase1.salvarJogo(op);
				}
				else if (carregar)
				{
					telaMundos.setPosX(gerenciador_grafico->getViewCenter().x - 700.0f);

					int op = exibirMenuMundos();
					op++;
					//fase1.recuperarJogo(op, fase, n_jogadores);
				}
			}
			fase1.setDerrota(derrota);

		}
	}
	
	if (fase == 2)
	{
		telaCarregamento();

		derrota = false;

		alocaFase2(n_jogadores);

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

				int opcao = exibirMenuPausa();

				if (salvar)
				{
					telaMundos.setPosX(gerenciador_grafico->getViewCenter().x - 700.0f);

					int op = exibirMenuMundos();
					op++;
					fase2.salvarJogo(op);
				}
				else if (carregar)
				{
					int op = exibirMenuMundos();
					op++;
					//fase2.recuperarJogo(op, fase, n_jogadores);
				}


			}
			fase2.setDerrota(derrota);

		}
		concluida = false;

	}
}

int Principal::exibirMenuMundos()
{
	int opcao = -1;

	while (salvar)
	{
		opcao = telaMundos.verificaEventoTela();

		if (opcao != -1)
		{
			salvar = false;
			return opcao;
		}

		gerenciador_grafico->limpaTela();
		telaMundos.desenharTela();
		gerenciador_grafico->mostraElemento();
	}

	while (carregar)
	{
		opcao = telaMundos.verificaEventoTela();

		if (opcao != -1)
		{
			carregar = false;
			return opcao;
		}

		gerenciador_grafico->limpaTela();
		telaMundos.desenharTela();
		gerenciador_grafico->mostraElemento();
	}

	return opcao;
}

void Principal::inicializaMenu()
{
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

	Text tituloPausa;

	tituloPausa.setFont(*fonte);
	tituloPausa.setString("Jogo Pausado");
	tituloPausa.setPosition(1100, 170);
	tituloPausa.setCharacterSize(55);
	tituloPausa.setOutlineColor(Color::Black);
	tituloPausa.setFillColor(Color::White);

	telaPausa.setTitulo(tituloPausa);

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

void Principal::inicializaMundos()
{
	sf::Text tituloMundos;

	tituloMundos.setFont(*fonte);
	tituloMundos.setString("Escolha um mundo:");
	tituloMundos.setPosition(1100, 170);
	tituloMundos.setCharacterSize(55);
	tituloMundos.setOutlineColor(Color::Black);
	tituloMundos.setFillColor(Color::White);

	telaMundos.setTitulo(tituloMundos);

	Text novoTexto;

	std::vector<const char*> opcoes;
	std::vector<sf::Vector2f> coordenadas;
	std::vector<std::size_t> tamanhos;

	sf::RectangleShape* mundo1;
	sf::RectangleShape* mundo2;
	sf::RectangleShape* mundo3;
	sf::RectangleShape* sair;

	opcoes = { "Mundo 1", "Mundo 2 ","Mundo 3", "Sair" };
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

		telaMundos.addTexto(novoTexto);
	}
	telaMundos.setLarguraSelecionado(4);
	telaMundos.setLarguraPadrao(2);

	mundo1 = new sf::RectangleShape();
	mundo1->setSize(sf::Vector2f(200.0f, 50.0f));
	mundo1->setPosition(sf::Vector2f(300, 300));
	mundo1->setFillColor(sf::Color::Red);

	mundo2 = new sf::RectangleShape();
	mundo2->setSize(sf::Vector2f(205.0f, 50.0f));
	mundo2->setPosition(sf::Vector2f(300, 390));
	mundo2->setFillColor(sf::Color::Red);

	mundo3 = new sf::RectangleShape();
	mundo3->setSize(sf::Vector2f(145.0f, 50.0f));
	mundo3->setPosition(sf::Vector2f(300, 480));
	mundo3->setFillColor(sf::Color::Red);

	sair = new sf::RectangleShape();
	sair->setSize(sf::Vector2f(95.0f, 50.0f));
	sair->setPosition(sf::Vector2f(300, 570));
	sair->setFillColor(sf::Color::Red);

	telaMundos.addBotao(mundo1);
	telaMundos.addBotao(mundo2);
	telaMundos.addBotao(mundo3);
	telaMundos.addBotao(sair);

}

bool Principal::getConcluida()
{
	return concluida;
}
