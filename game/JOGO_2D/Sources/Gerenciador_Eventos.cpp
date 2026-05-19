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

		// Drena toda a fila de eventos por quadro; processar apenas um
		// evento por quadro descarta entradas e pode perder o Closed.
		while (gerenciador_grafico->getJanela()->pollEvent(evento))
		{
			if (evento.type == sf::Event::Closed)
				gerenciador_grafico->fecharJanela();
		}

		bool escPressionadoAtualmente = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);

		if (escPressionadoAtualmente && !escPressionadoAnteriormente)
		{
			jogoPausado = !jogoPausado;
		}

		escPressionadoAnteriormente = escPressionadoAtualmente;

		// Cada jogador interpreta a entrada com seu proprio mapeamento de
		// teclas; o gerenciador apenas delega, sem codificar teclas aqui.
		if (!jogoPausado)
		{
			if (pJogador)
				pJogador->processarEntrada();

			if (pJogador2)
				pJogador2->processarEntrada();
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

