#include "Gerenciador_Colisoes.hpp"

namespace Gerenciadores
{
	Gerenciador_Colisoes* Gerenciador_Colisoes::pGerenciador = nullptr;


	Gerenciador_Colisoes::Gerenciador_Colisoes() :
		pJogador(nullptr)
	{
	}

	Gerenciador_Colisoes::~Gerenciador_Colisoes()
	{
		delete pJogador;

		for (auto inimigo : inimigos)
		{
			delete inimigo;
		}
		inimigos.clear();

		for (auto plataforma : plataformas)
		{
			delete plataforma;
		}
		plataformas.clear();
	}

	Gerenciador_Colisoes* Gerenciador_Colisoes::getGerenciador()
	{
		pGerenciador == nullptr ? pGerenciador = new Gerenciador_Colisoes() : pGerenciador;

		return pGerenciador;
	}

	void Gerenciador_Colisoes::Executar()
	{
		for (int i = 0; i < plataformas.size(); i++)
		{
			verificaColisao(pJogador, plataformas.at(i));
		}

		for (int i = 0; i < inimigos.size(); i++)
		{
			for (int j = 0; j < plataformas.size(); j++)
			{
				verificaColisao(inimigos.at(i), plataformas.at(j));
			}
		}
	}


	void Gerenciador_Colisoes::setJogador(Entidades::Personagens::Jogador* jogador)
	{
		this->pJogador = jogador;
	}

	void Gerenciador_Colisoes::addInimigo(Entidades::Personagens::Inimigo* inimigo)
	{
		inimigos.push_back(inimigo);
	}

	void Gerenciador_Colisoes::addPlataforma(Entidades::Obstaculos::Plataforma* plataforma)
	{
		plataformas.push_back(plataforma);
	}

	void Gerenciador_Colisoes::verificaColisao(Entidades::Personagens::Personagem* corpo, Entidades::Obstaculos::Plataforma* plataforma)
	{
		if (corpo->getCorpo().getGlobalBounds().intersects(plataforma->getCorpo().getGlobalBounds())) {

			corpo->setIsJumping(false);
			corpo->setY(0.0f);

			sf::FloatRect corpoBounds = corpo->getCorpo().getGlobalBounds();
			sf::FloatRect plataformaBounds = plataforma->getCorpo().getGlobalBounds();

			if (corpoBounds.top < plataformaBounds.top) {
				corpo->setPos(corpo->getPos().x, plataformaBounds.top - corpoBounds.height);
			}
			else if (corpoBounds.top + corpoBounds.height > plataformaBounds.top + plataformaBounds.height) {
				corpo->setPos(corpo->getPos().x, plataformaBounds.top + plataformaBounds.height);
			}
			else if (corpoBounds.left < plataformaBounds.left) {
				corpo->setPos(plataformaBounds.left - corpoBounds.width, corpo->getPos().y);
			}
			else {
				corpo->setPos(plataformaBounds.left + plataformaBounds.width, corpo->getPos().y);
			}
		}
	}

}


