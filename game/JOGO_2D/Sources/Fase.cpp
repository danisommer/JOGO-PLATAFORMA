#include "../Headers/Fase.hpp"
#include "CarregadorFase.hpp"
#include "GerenciadorSave.hpp"
#include "Gerenciador_Recursos.hpp"
#include "Chefao.hpp"
#include "Jogador.hpp"
#include "Inimigo.hpp"
#include "Personagem.hpp"
#include "Obstaculo.hpp"
#include "Portal.hpp"
#include <iostream>

using namespace sf;
using namespace std;
using namespace Entidades;
using namespace Entidades::Personagens;
using namespace Entidades::Obstaculos;

namespace Fases
{
	Fase::Fase() :
		Ente(),
		gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
		gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
		gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
		listaPersonagem(),
		listaObstaculo(),
		mundo(nullptr),
		camera(),
		sistemaCombate(),
		texturaFundo(),
		numJogadores(0),
		concluida(false),
		derrota(false),
		fase(),
		morreu(false)
	{
		id = 2;
	}

	Fase::~Fase()
	{
	}

	void Fase::setMundo(Mundo* m)
	{
		mundo = m;
		if (mundo)
			mundo->setCamera(&camera);
	}

	namespace
	{
		// Helper compartilhado entre instanciaEntidades (modo classico)
		// e instanciaProcedural (modo roguelike). Faz a ligacao das
		// entidades carregadas com os managers da fase.
		void registrarEntidades(
			std::vector<std::unique_ptr<Entidade>>& entidades,
			Mundo* mundo,
			Gerenciadores::Gerenciador_Eventos* eventos,
			Gerenciadores::Gerenciador_Colisoes* colisoes,
			Lista::ListaEntidade& listaPersonagem,
			Lista::ListaEntidade& listaObstaculo,
			int& numJogadores)
		{
			std::vector<Jogador*> jogadores;

			for (auto& up : entidades)
			{
				Entidade* e = up.get();
				e->setMundo(mundo);

				if (Jogador* jog = dynamic_cast<Jogador*>(e))
				{
					// O jogador so pode consultar a arvore de habilidades
					// apos receber o ponteiro Mundo - por isso e aqui que
					// aplicamos os bonus permanentes da run.
					jog->aplicarHabilidades();

					// HP do roguelike persiste entre fases. Se o Mundo
					// guardou um valor da fase anterior, restauramos
					// (no minimo 1, para nao spawnar morto).
					if (mundo)
					{
						const int idxSlot = static_cast<int>(jogadores.size());
						const float vidaSalva = mundo->getVidaPersistida(idxSlot);
						if (vidaSalva > 0.0f)
							jog->carregarEstado(vidaSalva, jog->getPos());
					}

					if (jogadores.empty())
					{
						eventos->setJogador(jog);
						colisoes->setJogador(jog);
					}
					else
					{
						eventos->setJogador2(jog);
						colisoes->setJogador2(jog);
					}
					jogadores.push_back(jog);
					numJogadores++;
					listaPersonagem.addEntidade(up.release());
				}
				else if (Inimigo* inim = dynamic_cast<Inimigo*>(e))
				{
					// Aplica o escalonamento de dificuldade baseado na
					// fase atual do Mundo: vida e dano dos inimigos
					// crescem com o passar das fases.
					if (mundo)
						inim->aplicarDificuldade(mundo->getFaseAtual());

					colisoes->addInimigo(inim);
					listaPersonagem.addEntidade(up.release());
				}
				else if (Obstaculo* obst = dynamic_cast<Obstaculo*>(e))
				{
					colisoes->addObstaculo(obst);
					colisoes->addCorpo(obst);
					listaObstaculo.addEntidade(up.release());
				}
			}

			if (mundo)
				mundo->definirJogadores(jogadores);
		}
	}

	void Fase::instanciaEntidades(const std::string& arquivoTxt)
	{
		CarregadorFase carregador;
		std::vector<std::unique_ptr<Entidade>> entidades =
			carregador.carregar(arquivoTxt, fase);

		registrarEntidades(entidades, mundo, gerenciador_eventos,
			gerenciador_colisoes, listaPersonagem, listaObstaculo,
			numJogadores);

		if (numJogadores != 1 && numJogadores != 2)
		{
			std::cout << "numero incomum de jogadores" << std::endl;
			exit(1);
		}
	}

	void Fase::instanciaProcedural(int numeroFase, int nJogadores)
	{
		CarregadorFase carregador;
		std::vector<std::unique_ptr<Entidade>> entidades =
			carregador.gerarProcedural(numeroFase, nJogadores);

		registrarEntidades(entidades, mundo, gerenciador_eventos,
			gerenciador_colisoes, listaPersonagem, listaObstaculo,
			numJogadores);

		// Configura os limites horizontais da camera de acordo com o
		// tamanho real da fase gerada. Sem isto, a camera revelaria o
		// vazio antes da parede esquerda ou apos a direita.
		float minX = 0.0f;
		float maxX = 0.0f;
		CarregadorFase::calcularLimites(numeroFase, minX, maxX);
		camera.definirLimites(minX, maxX);
	}

	Sistemas::Camera& Fase::getCamera()
	{
		return camera;
	}

	void Fase::redesenharEntidades()
	{
		// Pass de renderizacao "puro": nenhuma logica de jogo avanca.
		// Reusa o desenharSprite ja existente em cada entidade, que
		// usa a posicao corrente do sprite (atualizada na primeira
		// passada de AtualizarPersonagens).
		for (int i = 0; i < listaObstaculo.getTam(); i++)
		{
			if (Obstaculo* o = dynamic_cast<Obstaculo*>(listaObstaculo[i]))
				o->desenharSprite();
		}
		for (int i = 0; i < listaPersonagem.getTam(); i++)
		{
			if (Personagem* p = dynamic_cast<Personagem*>(listaPersonagem[i]))
				p->desenharSprite();
		}
	}

	void Fase::setFase(int f)
	{
		fase = f;
		// Alterna o cenario entre os dois temas existentes. Sem isto,
		// a textura do background da primeira fase ficaria fixa para
		// sempre no modo roguelike.
		const int tema = ((f - 1) % 2) + 1;
		const std::string caminho = "Assets/Cenario/background_"
			+ std::to_string(tema) + ".png";
		if (!texturaFundo.loadFromFile(caminho))
		{
			// Em caso de falha mantemos a textura anterior - melhor que
			// derrubar o jogo no meio de uma run.
		}
	}

	void Fase::desalocaEntidades()
	{
		// Antes de destruir a fase, registra o HP de cada jogador no
		// Mundo para que ele persista na proxima fase. Se um jogador
		// ja estava marcado como morto (slot nullptr), ignoramos.
		if (mundo)
		{
			for (int i = 0; i < mundo->getNumSlots(); ++i)
			{
				if (Jogador* j = mundo->getJogador(i))
					mundo->setVidaPersistida(i, j->getVidaAtual());
			}
		}

		gerenciador_eventos->setJogador(nullptr);
		gerenciador_eventos->setJogador2(nullptr);
		gerenciador_colisoes->setJogador(nullptr);
		gerenciador_colisoes->setJogador2(nullptr);
		gerenciador_colisoes->limparListas();

		listaPersonagem.limpar();
		listaObstaculo.limpar();

		if (mundo)
		{
			mundo->limparJogadores();
			mundo->setChefaoMorreu(false, Vector2f(0.0f, 0.0f));
		}

		numJogadores = 0;
		concluida = false;
		derrota = false;
		morreu = false;
	}

	void Fase::AtualizarPersonagens()
	{
		Jogador* jogador = mundo ? mundo->getJogador(0) : nullptr;
		Jogador* jogador2 = mundo ? mundo->getJogador(1) : nullptr;

		// Ao morrer o chefao, abre-se um portal de saida na posicao dele.
		if (mundo && mundo->getChefaoMorreu() && !morreu)
		{
			Portal* p = new Portal(mundo->getPosChefao(), Vector2f(50.0f, 50.0f), true, true);
			p->setMundo(mundo);
			gerenciador_colisoes->addObstaculo(p);
			gerenciador_colisoes->addCorpo(p);
			listaObstaculo.addEntidade(p);
			morreu = true;
		}

		// Combate corpo-a-corpo dos jogadores.
		if (mundo)
			sistemaCombate.resolver(*mundo, listaPersonagem);

		// Morte dos jogadores: avisa managers e mundo.
		if (jogador && jogador->getMorte())
		{
			gerenciador_eventos->setJogador(nullptr);
			gerenciador_colisoes->setJogador(nullptr);
			mundo->removerJogador(jogador);
		}
		if (jogador2 && jogador2->getMorte())
		{
			gerenciador_eventos->setJogador2(nullptr);
			gerenciador_colisoes->setJogador2(nullptr);
			mundo->removerJogador(jogador2);
		}

		// Atualizar obstaculos.
		for (int i = 0; i < listaObstaculo.getTam(); i++)
		{
			if (Obstaculo* o = dynamic_cast<Obstaculo*>(listaObstaculo[i]))
				o->atualizar();
		}

		// Remover personagens mortos (avisando o gerenciador de colisoes,
		// que guarda ponteiros observadores, antes de destruir a entidade).
		// Cada inimigo abatido conta como kill no Mundo - serve tanto para
		// o HUD quanto para alimentar pontos da arvore de habilidades.
		for (int i = 0; i < listaPersonagem.getTam(); i++)
		{
			Personagem* p = dynamic_cast<Personagem*>(listaPersonagem[i]);
			if (p && p->getMorte())
			{
				if (Inimigo* inim = dynamic_cast<Inimigo*>(p))
				{
					if (mundo)
					{
						const bool ehChefao = dynamic_cast<Chefao*>(inim) != nullptr;
						mundo->registrarKill(ehChefao);

						// Habilidade Vampiro: cura cada jogador vivo que
						// possui a skill em 5 HP a cada inimigo abatido.
						for (int s = 0; s < mundo->getNumSlots(); ++s)
						{
							if (Jogador* j = mundo->getJogador(s))
							{
								if (j->getVampiro())
									j->curar(5.0f);
							}
						}
					}
					gerenciador_colisoes->removeInimigo(inim);
				}

				listaPersonagem.removerEntidade(p);
			}
		}

		// Atualizar personagens: temporizadores de combate, logica e gravidade.
		// processarEntrada e chamado aqui (dentro do passo de fisica) para que
		// o movimento horizontal seja aplicado uma vez por passo, nao uma vez
		// por frame — o que fazia o jogador ficar na metade da velocidade
		// quando o FPS caia abaixo de 60 (2 passos por frame).
		for (int i = 0; i < listaPersonagem.getTam(); i++)
		{
			if (Personagem* p = dynamic_cast<Personagem*>(listaPersonagem[i]))
			{
				p->atualizarEstadoCombate();

				if (Jogador* j = dynamic_cast<Jogador*>(p))
					j->processarEntrada();

				p->atualizar();
				p->cair();
			}
		}
	}

	void Fase::atualizaCamera()
	{
		if (mundo)
			camera.atualizar(*mundo, texturaFundo);
	}

	void Fase::desenharHUD()
	{
		if (!mundo)
			return;

		// HUD em coordenadas de tela (canto superior esquerdo). Como a
		// camera move a view, usamos uma view fixa em (0,0)-(TELA_X,TELA_Y)
		// apenas para o HUD e restauramos a view do jogo em seguida.
		const sf::Font& fonte = Gerenciadores::Gerenciador_Recursos::getGerenciador()
			->getFonte("Menu/antiquity-print.ttf");

		auto* janela = gerenciador_grafico->getJanela();
		const sf::View viewJogo = janela->getView();

		sf::View viewHUD(sf::FloatRect(0.0f, 0.0f, TELA_X, TELA_Y));
		janela->setView(viewHUD);

		sf::RectangleShape painel;
		painel.setPosition(15.0f, 15.0f);
		painel.setSize(sf::Vector2f(320.0f, 130.0f));
		painel.setFillColor(sf::Color(0, 0, 0, 160));
		painel.setOutlineColor(sf::Color::White);
		painel.setOutlineThickness(1.5f);
		janela->draw(painel);

		sf::Text texto;
		texto.setFont(fonte);
		texto.setCharacterSize(24);
		texto.setFillColor(sf::Color::White);
		texto.setOutlineColor(sf::Color::Black);
		texto.setOutlineThickness(1.0f);

		const bool ehChefao = mundo->getFaseAtual() % 5 == 0;
		const std::string rotuloFase = ehChefao
			? "Fase " + std::to_string(mundo->getFaseAtual()) + " - CHEFAO"
			: "Fase " + std::to_string(mundo->getFaseAtual());

		texto.setString(rotuloFase);
		texto.setPosition(28.0f, 22.0f);
		janela->draw(texto);

		texto.setString("Kills: " + std::to_string(mundo->getKills()));
		texto.setPosition(28.0f, 58.0f);
		janela->draw(texto);

		texto.setString("Pontos arvore: " +
			std::to_string(mundo->getArvore().getPontos()));
		texto.setPosition(28.0f, 94.0f);
		janela->draw(texto);

		janela->setView(viewJogo);
	}

	int Fase::getFase()
	{
		return fase;
	}

	bool Fase::getConcluida()
	{
		return concluida;
	}

	bool Fase::getDerrota()
	{
		return derrota;
	}

	void Fase::setDerrota(bool d)
	{
		if (!derrota)
			derrota = d;
	}

	void Fase::verificaFase()
	{
		Jogador* jogador = mundo ? mundo->getJogador(0) : nullptr;
		Jogador* jogador2 = mundo ? mundo->getJogador(1) : nullptr;

		if ((jogador && jogador->getConcluiuFase()) ||
			(jogador2 && jogador2->getConcluiuFase()))
		{
			concluida = true;
		}

		if (jogador == nullptr && jogador2 == nullptr)
			derrota = true;
	}

	void Fase::salvarJogo(int slot)
	{
		if (!mundo)
			return;

		Persistencia::DadosSave dados;
		dados.fase = mundo->getFaseAtual();
		dados.numJogadores = numJogadores;

		for (int i = 0; i < mundo->getNumSlots(); i++)
		{
			if (Jogador* j = mundo->getJogador(i))
			{
				Persistencia::EstadoJogador estado;
				estado.vida = j->getVidaAtual();
				estado.x = j->getPos().x;
				estado.y = j->getPos().y;
				dados.jogadores.push_back(estado);
			}
		}

		// Estado da run do modo roguelike.
		dados.pontuacao[0] = mundo->getPontuacao(0);
		dados.pontuacao[1] = mundo->getPontuacao(1);
		dados.kills = mundo->getKills();
		dados.pontosArvore = mundo->getArvore().getPontos();

		for (int i = 0; i < ArvoreHabilidades::N_HABILIDADES; ++i)
		{
			dados.skillsDesbloqueadas.push_back(
				mundo->getArvore().foiDesbloqueada(
					static_cast<ArvoreHabilidades::Habilidade>(i)));
		}

		Persistencia::GerenciadorSave::salvar(slot, dados);
	}

	void Fase::aplicarSave(int slot)
	{
		Persistencia::DadosSave dados;
		if (!mundo || !Persistencia::GerenciadorSave::carregar(slot, dados))
			return;

		for (std::size_t i = 0; i < dados.jogadores.size(); i++)
		{
			if (Jogador* j = mundo->getJogador(static_cast<int>(i)))
			{
				const Persistencia::EstadoJogador& e = dados.jogadores[i];
				j->carregarEstado(e.vida, sf::Vector2f(e.x, e.y));
			}
		}

		// Restaura o estado da run roguelike. A fase em si nao e
		// regenerada aqui (aplicarSave so atua sobre a fase ja
		// montada); para carregar comecando de outra fase, usar
		// Principal::recuperaFase, que reinicia o EstadoJogo.
		mundo->setFaseAtual(dados.fase);
		mundo->setKills(dados.kills);
		mundo->zerarPontuacao();
		mundo->adicionarPontos(0, dados.pontuacao[0]);
		mundo->adicionarPontos(1, dados.pontuacao[1]);
		mundo->getArvore().setPontos(dados.pontosArvore);
		for (int i = 0; i < ArvoreHabilidades::N_HABILIDADES; ++i)
		{
			const bool v = (i < static_cast<int>(dados.skillsDesbloqueadas.size()))
				? dados.skillsDesbloqueadas[i] : false;
			mundo->getArvore().setDesbloqueada(
				static_cast<ArvoreHabilidades::Habilidade>(i), v);
		}
	}
}
