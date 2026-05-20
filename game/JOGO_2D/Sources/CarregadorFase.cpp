#include "CarregadorFase.hpp"
#include "Jogador.hpp"
#include "OlhoVoador.hpp"
#include "Chefao.hpp"
#include "Cogumelo.hpp"
#include "Plataforma.hpp"
#include "Parede.hpp"
#include "Serra.hpp"
#include "Slime.hpp"
#include "Espinho.hpp"
#include "Portal.hpp"
#include <fstream>
#include <random>
#include <stdexcept>

namespace Fases
{
	bool CarregadorFase::aleatorizar(char character)
	{
		const int minimo = 3;

		switch (character)
		{
		case 'f':
			if (numVoadores < minimo) { numVoadores++; return true; }
			return std::rand() % 2 != 0;
		case 'm':
			if (numCogumelos < minimo) { numCogumelos++; return true; }
			return std::rand() % 2 != 0;
		case 's':
			if (numSerras < minimo) { numSerras++; return true; }
			return std::rand() % 2 != 0;
		case 'e':
			if (numEspinhos < minimo) { numEspinhos++; return true; }
			return std::rand() % 2 != 0;
		case 'g':
			if (numSlimes < minimo) { numSlimes++; return true; }
			return std::rand() % 2 != 0;
		default:
			return true;
		}
	}

	void CarregadorFase::instanciarChar(char c, std::size_t x, std::size_t y,
		int numeroFase,
		std::vector<std::unique_ptr<Entidades::Entidade>>& saida,
		int& indiceJogador) const
	{
		using namespace Entidades;
		using namespace Entidades::Personagens;
		using namespace Entidades::Obstaculos;

		const sf::Vector2f pos(x * 50.0f, y * 50.0f);
		Entidade* e = nullptr;

		switch (c)
		{
		case 'j':
			e = new Jogador(pos, sf::Vector2f(30.0f, 90.0f), indiceJogador++);
			break;
		case 'f':
			e = new OlhoVoador(pos, sf::Vector2f(40.0f, 50.0f));
			break;
		case 'c':
			e = new Chefao(pos, sf::Vector2f(100.0f, 120.0f));
			break;
		case 'm':
			e = new Cogumelo(pos, sf::Vector2f(40.0f, 80.0f));
			break;
		case 'p':
		{
			auto* plat = new Plataforma(pos, sf::Vector2f(300.0f, 50.0f), false);
			plat->setFase(numeroFase);
			e = plat;
			break;
		}
		case 'P':
		{
			auto* plat = new Plataforma(pos, sf::Vector2f(1900.0f, 50.0f), true);
			plat->setFase(numeroFase);
			e = plat;
			break;
		}
		case 'w':
			e = new Parede(pos, sf::Vector2f(50.0f, 3600.0f));
			break;
		case 's':
			e = new Serra(pos, sf::Vector2f(100.0f, 100.0f));
			break;
		case 'g':
			e = new Slime(pos, sf::Vector2f(300.0f, 50.0f));
			break;
		case 'e':
			e = new Espinho(pos, sf::Vector2f(100.0f, 50.0f));
			break;
		case 't':
			e = new Portal(pos, sf::Vector2f(50.0f, 50.0f), true, true);
			break;
		case 'T':
			e = new Portal(pos, sf::Vector2f(50.0f, 50.0f), true, false);
			break;
		default:
			break;
		}

		if (e)
			saida.emplace_back(e);
	}

	std::vector<std::unique_ptr<Entidades::Entidade>> CarregadorFase::carregar(
		const std::string& arquivo, int numeroFase)
	{
		std::ifstream f(arquivo);
		if (!f.is_open())
			throw std::runtime_error("Nao foi possivel abrir a fase: " + arquivo);

		std::vector<std::string> linhas;
		std::string linha;
		while (std::getline(f, linha))
			linhas.push_back(linha);
		f.close();

		std::vector<std::unique_ptr<Entidades::Entidade>> entidades;
		int indiceJogador = 0;

		for (std::size_t x = 0; x < linhas.size(); x++)
		{
			for (std::size_t y = 0; y < linhas[x].size(); y++)
			{
				const char c = linhas[x][y];
				if (!aleatorizar(c))
					continue;
				instanciarChar(c, x, y, numeroFase, entidades, indiceJogador);
			}
		}

		return entidades;
	}

	void CarregadorFase::calcularLimites(int numeroFase, float& minX, float& maxX)
	{
		const bool ehFaseChefao = (numeroFase > 0 && numeroFase % 5 == 0);
		int numSecoes;
		if (ehFaseChefao)
			numSecoes = 2;
		else
			numSecoes = 3 + (numeroFase / 3);
		if (numSecoes > 7) numSecoes = 7;

		const int tilesPorSecao = 38;
		const int xFim = numSecoes * tilesPorSecao;

		// Mundo cobre de x=0 (parede esquerda) ate x=(xFim+1)*50
		// (logo apos a parede direita). A camera so anda dentro deste
		// intervalo - alem dele so ha o vazio do background.
		minX = 0.0f;
		maxX = static_cast<float>((xFim + 1) * 50);
	}

	std::vector<std::unique_ptr<Entidades::Entidade>> CarregadorFase::gerarProcedural(
		int numeroFase, int numJogadores)
	{
		// Coordenadas em "tiles" de 50 px. O eixo x da fase original
		// corresponde a indice de linha no .txt; aqui mantemos a mesma
		// convencao (1 unidade x = 50 px horizontais) para poder reusar
		// instanciarChar sem mudar tamanhos.
		//
		// Layout da fase:
		// - colunas y = 0..17, com piso 'P' em y=17 (px 850).
		// - paredes 'w' em x=0 e x=Xfim para conter o jogador.
		// - jogador comeca na proxima coluna; portal no final.

		std::vector<std::unique_ptr<Entidades::Entidade>> entidades;
		int indiceJogador = 0;

		// O "sabor" visual (background) e tema das plataformas alterna
		// entre 1 e 2 de acordo com a fase, reusando as duas Fases ja
		// existentes.
		const int tema = ((numeroFase - 1) % 2) + 1;

		const bool ehFaseChefao = (numeroFase > 0 && numeroFase % 5 == 0);

		// Largura da fase em "secoes" de piso (cada secao = 38 tiles,
		// largura da plataforma 'P'). Cresce levemente com a fase,
		// limitada para nao virar maratona.
		int numSecoes;
		if (ehFaseChefao)
			numSecoes = 2; // arena curta
		else
			numSecoes = 3 + (numeroFase / 3);
		if (numSecoes > 7) numSecoes = 7;

		const int tilesPorSecao = 38;
		const int xFim = numSecoes * tilesPorSecao;

		// RNG dedicado, semeado a partir da fase (mesma fase = mesmo
		// layout). Permite reprodutibilidade no save/load.
		std::mt19937 rng(static_cast<unsigned int>(numeroFase * 7919u + 1));
		auto sortear = [&](int min, int max) {
			std::uniform_int_distribution<int> d(min, max);
			return d(rng);
		};

		// Piso: uma 'P' por secao, sempre em y=17.
		for (int s = 0; s < numSecoes; ++s)
		{
			const int x = s * tilesPorSecao + 1; // levemente deslocada
			instanciarChar('P', x, 17, tema, entidades, indiceJogador);
		}

		// Paredes nas extremidades para impedir saidas.
		instanciarChar('w', 0, 0, tema, entidades, indiceJogador);
		instanciarChar('w', xFim, 0, tema, entidades, indiceJogador);

		// Jogadores comecam logo apos a parede esquerda, sobre o piso.
		instanciarChar('j', 2, 11, tema, entidades, indiceJogador);
		if (numJogadores >= 2)
			instanciarChar('j', 4, 11, tema, entidades, indiceJogador);

		// Portal de saida no fim, sobre o piso.
		instanciarChar('t', xFim - 2, 14, tema, entidades, indiceJogador);

		if (ehFaseChefao)
		{
			// Apenas o chefao no centro da arena.
			const int xMeio = xFim / 2;
			instanciarChar('c', xMeio, 8, tema, entidades, indiceJogador);
			return entidades;
		}

		// Geracao logica de plataformas. Em cada secao construimos uma
		// "torre" de 1 a 3 plataformas, cada degrau ~3 tiles acima do
		// anterior - alturas que cabem no pulo do jogador (~4 tiles).
		// O degrau mais baixo fica em y=14 (3 tiles acima do piso 17),
		// nao tao alto como antes (quando podia spawnar em y=6).
		//
		// Sob cada torre colocamos uma serra ou espinho no chao, dando
		// um motivo concreto para o jogador subir nas plataformas em
		// vez de passar correndo pelo solo.

		struct Torre
		{
			int xBase;
			int alturaDegraus;
			int xHazard;  // ponto onde o hazard fica no piso
			char hazard;  // 's' (serra) ou 'e' (espinho)
		};

		std::vector<Torre> torres;

		for (int s = 0; s < numSecoes; ++s)
		{
			// Cada secao ganha de 1 a 2 torres, deslocadas no eixo X.
			const int torresNaSecao = 1 + sortear(0, 1);
			for (int t = 0; t < torresNaSecao; ++t)
			{
				Torre tor;
				tor.alturaDegraus = 1 + sortear(0, 2); // 1..3
				tor.xBase = s * tilesPorSecao + sortear(7, tilesPorSecao - 7);

				// Hazard centrado embaixo do degrau mais baixo.
				tor.xHazard = tor.xBase;
				tor.hazard = (sortear(0, 1) == 0) ? 's' : 'e';
				torres.push_back(tor);

				int xAtual = tor.xBase;
				for (int d = 0; d < tor.alturaDegraus; ++d)
				{
					// Degrau d: y = 14 - d*3, xAtual ligeiramente
					// deslocado a cada degrau para nao virar uma coluna.
					const int y = 14 - d * 3;
					instanciarChar('p', xAtual, y, tema, entidades, indiceJogador);
					xAtual += sortear(-2, 2);
				}
			}
		}

		// Coloca os hazards do solo embaixo das torres. Garante uma
		// brecha de 4 tiles entre torres para nao deixar a fase
		// intransponivel (jogador precisa de espaco para pousar).
		for (const Torre& tor : torres)
		{
			// 'e' (espinho) tem 100x50 = 2 tiles de largura; o sprite
			// nasce uma posicao a esquerda do x indicado.
			instanciarChar(tor.hazard, tor.xHazard, 16, tema, entidades, indiceJogador);
		}

		// Inimigos: cresce a quantidade com a fase. Distribuidos pelo
		// nivel; voadores no alto, terrestres perto do piso. Slime e
		// armadilha pegajosa esparsa.
		// Em fases altas (>= 8) a contagem cresce de forma mais
		// agressiva para que a dificuldade nao estabilize.
		const int extra = (numeroFase >= 8) ? (numeroFase - 7) : 0;
		const int nVoadores = 1 + numeroFase / 2 + extra;
		const int nCogumelos = 1 + numeroFase / 2 + extra;
		const int nSlimes = numeroFase / 4;

		auto colocarEntreParedes = [&](char tipo, int yMin, int yMax) {
			const int x = sortear(6, xFim - 6);
			const int y = sortear(yMin, yMax);
			instanciarChar(tipo, x, y, tema, entidades, indiceJogador);
		};

		for (int i = 0; i < nVoadores; ++i)
			colocarEntreParedes('f', 4, 10);
		for (int i = 0; i < nCogumelos; ++i)
			colocarEntreParedes('m', 14, 15);
		for (int i = 0; i < nSlimes; ++i)
			colocarEntreParedes('g', 16, 16);

		return entidades;
	}
}
