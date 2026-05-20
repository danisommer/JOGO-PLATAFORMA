#include "SistemaCombate.hpp"
#include "Mundo.hpp"
#include "ListaEntidade.hpp"
#include "Jogador.hpp"
#include "Inimigo.hpp"
#include <cmath>

namespace Sistemas
{
	namespace
	{
		// Aplica o dano de um jogador atacante aos inimigos ao alcance.
		void aplicarAtaque(Entidades::Personagens::Jogador* jogador,
			int indiceJogador, Mundo& mundo, Lista::ListaEntidade& personagens)
		{
			if (!jogador || !jogador->getAtacando())
				return;

			const sf::Vector2f regiao = jogador->getRegiaoAtaque();
			const float jogadorX = jogador->getPos().x;

			for (int i = 0; i < personagens.getTam(); i++)
			{
				auto* inimigo = dynamic_cast<Entidades::Personagens::Inimigo*>(personagens[i]);
				if (inimigo &&
					std::fabs(inimigo->getPos().x - regiao.x) < 80.0f &&
					std::fabs(inimigo->getPos().y - regiao.y) < 80.0f)
				{
					// Empurra o inimigo no sentido oposto ao do jogador.
					const int dir = (inimigo->getPos().x >= jogadorX) ? 1 : -1;

					// So pontua quando o golpe realmente acerta - tomarDano
					// devolve false durante os quadros de invulnerabilidade.
					if (inimigo->tomarDano(jogador->getDano(), dir))
						mundo.adicionarPontos(indiceJogador, 3);
				}
			}
		}
	}

	void SistemaCombate::resolver(Mundo& mundo, Lista::ListaEntidade& personagens)
	{
		aplicarAtaque(mundo.getJogador(0), 0, mundo, personagens);
		aplicarAtaque(mundo.getJogador(1), 1, mundo, personagens);
	}
}
