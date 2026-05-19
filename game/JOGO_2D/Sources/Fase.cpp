#include "../Headers/Fase.hpp"
#include "CarregadorFase.hpp"
#include "GerenciadorSave.hpp"
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
	}

	void Fase::instanciaEntidades(const std::string& arquivoTxt)
	{
		CarregadorFase carregador;
		std::vector<std::unique_ptr<Entidade>> entidades =
			carregador.carregar(arquivoTxt, fase);

		std::vector<Jogador*> jogadores;

		for (auto& up : entidades)
		{
			Entidade* e = up.get();
			e->setMundo(mundo);

			if (Jogador* jog = dynamic_cast<Jogador*>(e))
			{
				if (jogadores.empty())
				{
					gerenciador_eventos->setJogador(jog);
					gerenciador_colisoes->setJogador(jog);
				}
				else
				{
					gerenciador_eventos->setJogador2(jog);
					gerenciador_colisoes->setJogador2(jog);
				}
				jogadores.push_back(jog);
				numJogadores++;
				listaPersonagem.addEntidade(up.release());
			}
			else if (Inimigo* inim = dynamic_cast<Inimigo*>(e))
			{
				gerenciador_colisoes->addInimigo(inim);
				listaPersonagem.addEntidade(up.release());
			}
			else if (Obstaculo* obst = dynamic_cast<Obstaculo*>(e))
			{
				gerenciador_colisoes->addObstaculo(obst);
				gerenciador_colisoes->addCorpo(obst);
				listaObstaculo.addEntidade(up.release());
			}
		}

		if (numJogadores != 1 && numJogadores != 2)
		{
			std::cout << "numero incomum de jogadores" << std::endl;
			exit(1);
		}

		if (mundo)
			mundo->definirJogadores(jogadores);
	}

	void Fase::desalocaEntidades()
	{
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
		for (int i = 0; i < listaPersonagem.getTam(); i++)
		{
			Personagem* p = dynamic_cast<Personagem*>(listaPersonagem[i]);
			if (p && p->getMorte())
			{
				if (Inimigo* inim = dynamic_cast<Inimigo*>(p))
					gerenciador_colisoes->removeInimigo(inim);

				listaPersonagem.removerEntidade(p);
			}
		}

		// Atualizar personagens e aplicar gravidade.
		for (int i = 0; i < listaPersonagem.getTam(); i++)
		{
			if (Personagem* p = dynamic_cast<Personagem*>(listaPersonagem[i]))
			{
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
		dados.fase = fase;
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
	}
}
