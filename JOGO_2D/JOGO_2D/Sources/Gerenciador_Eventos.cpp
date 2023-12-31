#include "Gerenciador_Eventos.hpp"

namespace Gerenciadores
{
	Gerenciador_Eventos* Gerenciador_Eventos::pGerenciador = nullptr;
	Gerenciador_Grafico* Gerenciador_Eventos::gerenciador_grafico = Gerenciador_Grafico::getGerenciador();


	Gerenciador_Eventos::Gerenciador_Eventos() :
		pJogador(nullptr),
		pJogador2(nullptr),
		jogoPausado(false),
		escPressionadoAnteriormente(false)
	{

	}

	Gerenciador_Eventos::~Gerenciador_Eventos()
	{
	}

	Gerenciador_Eventos* Gerenciador_Eventos::getGerenciador()
	{
		pGerenciador == nullptr ? pGerenciador = new Gerenciador_Eventos() : pGerenciador;

		return pGerenciador;
	}

	void Gerenciador_Eventos::setJogador(Entidades::Personagens::Jogador* jogador)
	{
		this->pJogador = jogador;
	}

	void Gerenciador_Eventos::setJogador2(Entidades::Personagens::Jogador* jogador)
	{
		this->pJogador2 = jogador;
	}

	void Gerenciador_Eventos::Executar()
	{
		Event evento;

		gerenciador_grafico->getJanela()->pollEvent(evento);

		bool escPressionadoAtualmente = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

		if (escPressionadoAtualmente && !escPressionadoAnteriormente)
		{
			jogoPausado = !jogoPausado;
		}

		escPressionadoAnteriormente = escPressionadoAtualmente;

		if (!jogoPausado)
		{
			if (pJogador)
			{
				if (Keyboard::isKeyPressed(Keyboard::E))
				{
					pJogador->bater(true);

				}
				else
				{
					pJogador->bater(false);

					if (Keyboard::isKeyPressed(Keyboard::D))
					{
						pJogador->mover(true, false);
					}

					if (Keyboard::isKeyPressed(Keyboard::A))
					{
						pJogador->mover(false, true);

					}

					if (!(Keyboard::isKeyPressed(Keyboard::D)) && !(Keyboard::isKeyPressed(Keyboard::A)))
					{
						pJogador->mover(false, false);
					}

					if (Keyboard::isKeyPressed(Keyboard::W))
					{
						pJogador->pular(true);

					}
					else
					{
						pJogador->pular(false);

					}
				}
			}

			if (pJogador2)
			{
				if (Keyboard::isKeyPressed(Keyboard::M))
				{
					pJogador2->bater(true);

				}
				else
				{
					pJogador2->bater(false);

					if (Keyboard::isKeyPressed(Keyboard::Right))
					{
						pJogador2->mover(true, false);
					}

					if (Keyboard::isKeyPressed(Keyboard::Left))
					{
						pJogador2->mover(false, true);

					}

					if (!(Keyboard::isKeyPressed(Keyboard::Right)) && !(Keyboard::isKeyPressed(Keyboard::Left)))
					{
						pJogador2->mover(false, false);
					}

					if (Keyboard::isKeyPressed(Keyboard::Up))
					{
						pJogador2->pular(true);

					}
					else
					{
						pJogador2->pular(false);

					}
				}
			}

		}
		

		if (evento.type == sf::Event::Closed)
		{
			gerenciador_grafico->fecharJanela();
		}
	}
	bool Gerenciador_Eventos::getJogoPausado()
	{
		return jogoPausado;
	}
	void Gerenciador_Eventos::despausarJogo()
	{
		escPressionadoAnteriormente = false;
		jogoPausado = false;
	}
}

