#include "Gerenciador_Eventos.hpp"

namespace Gerenciadores
{
	Gerenciador_Eventos* Gerenciador_Eventos::pGerenciador = nullptr;
	Gerenciador_Grafico* Gerenciador_Eventos::gerenciador_grafico = Gerenciador_Grafico::getGerenciador();


	Gerenciador_Eventos::Gerenciador_Eventos():
		pJogador(nullptr)
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

	void Gerenciador_Eventos::Executar()
	{
		Event evento;

		gerenciador_grafico->getJanela()->pollEvent(evento);

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
		
		if (evento.type == sf::Event::Closed) 
			{
				gerenciador_grafico->fecharJanela();
			}
		}
	}

