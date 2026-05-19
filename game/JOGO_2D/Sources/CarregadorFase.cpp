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

	std::vector<std::unique_ptr<Entidades::Entidade>> CarregadorFase::carregar(
		const std::string& arquivo, int numeroFase)
	{
		using namespace Entidades;
		using namespace Entidades::Personagens;
		using namespace Entidades::Obstaculos;

		std::ifstream f(arquivo);
		if (!f.is_open())
			throw std::runtime_error("Nao foi possivel abrir a fase: " + arquivo);

		std::vector<std::string> linhas;
		std::string linha;
		while (std::getline(f, linha))
			linhas.push_back(linha);
		f.close();

		std::vector<std::unique_ptr<Entidade>> entidades;
		int indiceJogador = 0;

		for (std::size_t x = 0; x < linhas.size(); x++)
		{
			for (std::size_t y = 0; y < linhas[x].size(); y++)
			{
				const char c = linhas[x][y];
				if (!aleatorizar(c))
					continue;

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
					entidades.emplace_back(e);
			}
		}

		return entidades;
	}
}
