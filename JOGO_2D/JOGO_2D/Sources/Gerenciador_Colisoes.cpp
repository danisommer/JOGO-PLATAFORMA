#include "Gerenciador_Colisoes.hpp"

namespace Gerenciadores
{
	Gerenciador_Colisoes* Gerenciador_Colisoes::pGerenciador = nullptr;


	Gerenciador_Colisoes::Gerenciador_Colisoes() :
		pJogador(nullptr),
		pJogador2(nullptr)
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

	}

	Gerenciador_Colisoes* Gerenciador_Colisoes::getGerenciador()
	{
		pGerenciador == nullptr ? pGerenciador = new Gerenciador_Colisoes() : pGerenciador;

		return pGerenciador;
	}

	void Gerenciador_Colisoes::Executar()
	{
		for (int i = 0; i < obstaculos.size(); i++)
		{
			if (pJogador && pJogador->getCorpo()->getGlobalBounds().intersects(obstaculos.at(i)->getCorpo()->getGlobalBounds()))
			{
				obstaculos.at(i)->danar(1);
			}
			if (pJogador2 && pJogador2->getCorpo()->getGlobalBounds().intersects(obstaculos.at(i)->getCorpo()->getGlobalBounds()))
			{
				obstaculos.at(i)->danar(2);
			}
		}

		for (int i = 0; i < corpos.size(); i++)
		{
			if (pJogador)
				verificaColisaoJogador(pJogador, corpos.at(i));

			if (pJogador2)
				verificaColisaoJogador(pJogador2, corpos.at(i));

			for (int j = 0; j < inimigos.size(); j++)
			{
				if (inimigos.at(j) && !inimigos.at(j)->getVoador())
					verificaColisaoInimigo(inimigos.at(j), corpos.at(i));
			}
		}

		for (int i = 0; i < inimigos.size(); i++)
		{

			if (inimigos.at(i) && !inimigos.at(i)->getParado())
			{
				float distanciaX;
				float distanciaY;

				if (pJogador)
				{
					distanciaX = pJogador->getPos().x - inimigos.at(i)->getPos().x;
					distanciaY = pJogador->getPos().y - inimigos.at(i)->getPos().y;

					if (fabs(distanciaY) <= inimigos.at(i)->getDistanciaAtaqueY() && fabs(distanciaX) <= inimigos.at(i)->getDistanciaAtaqueX())
					{
						inimigos.at(i)->atacar(1);
					}
				}
				
				if (pJogador2)
				{
					distanciaX = pJogador2->getPos().x - inimigos.at(i)->getPos().x;
					distanciaY = pJogador2->getPos().y - inimigos.at(i)->getPos().y;

					if (fabs(distanciaY) <= inimigos.at(i)->getDistanciaAtaqueY() && fabs(distanciaX) <= inimigos.at(i)->getDistanciaAtaqueX())
					{
						inimigos.at(i)->atacar(2);
					}
				}


			}

		}
	}

	void Gerenciador_Colisoes::setJogador(Entidades::Personagens::Jogador* jogador)
	{
		this->pJogador = jogador;
	}

	void Gerenciador_Colisoes::setJogador2(Entidades::Personagens::Jogador* jogador)
	{
		this->pJogador2 = jogador;
	}

	void Gerenciador_Colisoes::addInimigo(Entidades::Personagens::Inimigo* inimigo)
	{
		inimigos.push_back(inimigo);
	}

	void Gerenciador_Colisoes::removeInimigo(int index)
	{
		inimigos[index] = nullptr;
	}

	void Gerenciador_Colisoes::addPlataforma(Entidades::Obstaculos::Plataforma* plataforma)
	{
		corpos.push_back(plataforma->getCorpo());
	}

	void Gerenciador_Colisoes::addParede(Entidades::Obstaculos::Parede* parede)
	{
		corpos.push_back(parede->getCorpo());

	}

	void Gerenciador_Colisoes::addCorpo(Entidades::Obstaculos::Obstaculo* obstaculo)
	{
		if (obstaculo->getColidir())
			corpos.push_back(obstaculo->getCorpo());
	}

	void Gerenciador_Colisoes::addObstaculo(Entidades::Obstaculos::Obstaculo* obstaculo)
	{
		if (obstaculo->getDanoso())
			obstaculos.push_back(obstaculo);
	}

	void Gerenciador_Colisoes::verificaColisaoJogador(Entidades::Personagens::Personagem* corpo, RectangleShape* plataforma)
	{
		if (corpo->getCorpo()->getGlobalBounds().intersects(plataforma->getGlobalBounds()))
		{
			corpo->setY(0.0f);

			sf::FloatRect corpoBounds = corpo->getCorpo()->getGlobalBounds();
			sf::FloatRect plataformaBounds = plataforma->getGlobalBounds();

			if (corpoBounds.top < plataformaBounds.top)
			{
				corpo->setIsJumping(false);
				corpo->setPos(corpo->getPos().x, plataformaBounds.top - corpoBounds.height);
			}
			else if (corpoBounds.top + corpoBounds.height > plataformaBounds.top + plataformaBounds.height)
			{
				corpo->setPos(corpo->getPos().x, plataformaBounds.top + plataformaBounds.height);
			}
			else if (corpoBounds.left < plataformaBounds.left)
			{
				corpo->setPos(plataformaBounds.left - corpoBounds.width, corpo->getPos().y);
			}
			else
			{
				corpo->setPos(plataformaBounds.left + plataformaBounds.width, corpo->getPos().y);
			}

		}
	}
	void Gerenciador_Colisoes::verificaColisaoInimigo(Entidades::Personagens::Personagem* corpo, RectangleShape* plataforma)
	{
		if (corpo->getCorpo()->getGlobalBounds().intersects(plataforma->getGlobalBounds()))
		{
			corpo->setY(0.0f);
			corpo->setIsJumping(false);
			corpo->setPos(corpo->getPos().x, plataforma->getGlobalBounds().top - corpo->getCorpo()->getGlobalBounds().height);
		}
	}
}



