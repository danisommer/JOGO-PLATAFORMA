#include "../Headers/Principal.hpp"
#include "Gerenciador_Estados.hpp"
#include "Gerenciador_Recursos.hpp"
#include "GerenciadorSave.hpp"
#include "EstadoJogo.hpp"
#include "Fase.hpp"
#include <memory>

Principal::Principal() : gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
fase(),
fonte(nullptr)
{
	fonte = &Gerenciadores::Gerenciador_Recursos::getGerenciador()
		->getFonte("Menu/antiquity-print.ttf");

	fase.setMundo(&mundo);

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

void Principal::recuperaFase(int save)
{
	Persistencia::DadosSave dados;
	if (!Persistencia::GerenciadorSave::carregar(save, dados))
	{
		cout << "save inexistente" << endl;
		return;
	}

	// Aplica o estado salvo no Mundo antes de iniciar a fase, para
	// que a fase reaberta ja tenha a pontuacao, kills e arvore.
	mundo.setFaseAtual(dados.fase);
	mundo.setKills(dados.kills);
	for (int i = 0; i < 2; ++i)
		mundo.adicionarPontos(i, dados.pontuacao[i] - mundo.getPontuacao(i));

	mundo.getArvore().setPontos(dados.pontosArvore);
	for (int i = 0; i < ArvoreHabilidades::N_HABILIDADES; ++i)
	{
		mundo.getArvore().setDesbloqueada(
			static_cast<ArvoreHabilidades::Habilidade>(i),
			i < static_cast<int>(dados.skillsDesbloqueadas.size())
				? dados.skillsDesbloqueadas[i]
				: false);
	}

	Gerenciadores::Gerenciador_Estados estados;
	estados.empilhar(std::make_unique<Estados::EstadoJogo>(
		&estados, this, dados.fase, dados.numJogadores, false, save));

	while (!estados.vazio() && gerenciador_grafico->getOpen())
		estados.executarQuadro();
}

void Principal::executarFase(int faseInicial, int n_jogadores)
{
	// EstadoJogo cuida do loop infinito de fases proceduralmente
	// geradas. Aqui apenas iniciamos a maquina de estados.
	Gerenciadores::Gerenciador_Estados estados;
	estados.empilhar(std::make_unique<Estados::EstadoJogo>(
		&estados, this, faseInicial, n_jogadores, true));

	while (!estados.vazio() && gerenciador_grafico->getOpen())
		estados.executarQuadro();
}

Fases::Fase* Principal::prepararFase(int numFase, int n_jogadores)
{
	telaCarregamento();
	fase.desalocaEntidades();
	fase.setFase(numFase);
	fase.instanciaProcedural(numFase, n_jogadores);
	return &fase;
}

Tela& Principal::getTelaPausa()
{
	return telaPausa;
}

Tela& Principal::getTelaMundos()
{
	return telaMundos;
}

Mundo& Principal::getMundo()
{
	return mundo;
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
	sf::RectangleShape* habilidades;
	sf::RectangleShape* sair;

	opcoes = { "Continuar", "Recuperar ", "Salvar", "Habilidades", "Sair" };
	coordenadas = { { 300, 250}, { 300, 330 }, { 300, 410 }, { 300, 490 }, { 300, 570 } };
	tamanhos = { 30, 30, 30, 30, 30 };

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
	continuar->setPosition(sf::Vector2f(300, 250));
	continuar->setFillColor(sf::Color::Red);

	recuperar = new sf::RectangleShape();
	recuperar->setSize(sf::Vector2f(205.0f, 50.0f));
	recuperar->setPosition(sf::Vector2f(300, 330));
	recuperar->setFillColor(sf::Color::Red);

	salvar = new sf::RectangleShape();
	salvar->setSize(sf::Vector2f(145.0f, 50.0f));
	salvar->setPosition(sf::Vector2f(300, 410));
	salvar->setFillColor(sf::Color::Red);

	habilidades = new sf::RectangleShape();
	habilidades->setSize(sf::Vector2f(240.0f, 50.0f));
	habilidades->setPosition(sf::Vector2f(300, 490));
	habilidades->setFillColor(sf::Color::Red);

	sair = new sf::RectangleShape();
	sair->setSize(sf::Vector2f(95.0f, 50.0f));
	sair->setPosition(sf::Vector2f(300, 570));
	sair->setFillColor(sf::Color::Red);

	telaPausa.addBotao(continuar);
	telaPausa.addBotao(recuperar);
	telaPausa.addBotao(salvar);
	telaPausa.addBotao(habilidades);
	telaPausa.addBotao(sair);
}

void Principal::inicializaMundos()
{
	sf::Text tituloMundos;

	tituloMundos.setFont(*fonte);
	tituloMundos.setString("Escolha um slot:");
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

	opcoes = { "Save 1", "Save 2 ","Save 3", "Sair" };
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
