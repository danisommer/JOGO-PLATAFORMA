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

	void Gerenciador_Eventos::verificaPressionar(sf::Keyboard::Key tecla)
	{
		if (tecla == Keyboard::A)
		{

		}
		else if (tecla == Keyboard::B)
		{

		}
		else if (tecla == Keyboard::Space)
		{
		
		}
		else if (tecla == Keyboard::C)
		{
		
		}
	}

	void Gerenciador_Eventos::verificaSoltar(sf::Keyboard::Key tecla)
	{
	}

	void Gerenciador_Eventos::Executar()
	{
		Event evento;

		while (gerenciador_grafico->getJanela()->pollEvent(evento))
		{
			if (evento.type == Event::KeyPressed)
			{
				verificaPressionar(evento.key.code);
			}
			else if (evento.type == Event::KeyReleased)
			{
				verificaSoltar(evento.key.code);
			}
			else if (evento.type == sf::Event::Closed) 
			{
				gerenciador_grafico->fecharJanela();
			}
		}


	}

}