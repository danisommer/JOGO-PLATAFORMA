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
		// O gerenciador de colisoes apenas observa as entidades; quem as
		// possui (e portanto as destroi) e a Fase. Nao deletar aqui.
		limparListas();
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

			obstaculos.at(i)->atualizar();
		}

		for (int i = 0; i < corpos.size(); i++)
		{
			if (pJogador)
				verificaColisao(pJogador, corpos.at(i));

			if (pJogador2)
				verificaColisao(pJogador2, corpos.at(i));

			for (int j = 0; j < inimigos.size(); j++)
			{
				if (inimigos.at(j) && !inimigos.at(j)->getVoador())
					verificaColisao(inimigos.at(j), corpos.at(i));
			}
		}

		for (int i = 0; i < inimigos.size(); i++)
		{

			if (inimigos.at(i) && !inimigos.at(i)->getParado())
			{
				if (pJogador && inimigos.at(i)->podeAtacarAlvo(*pJogador->getCorpo()))
					inimigos.at(i)->atacar(1);

				if (pJogador2 && inimigos.at(i)->podeAtacarAlvo(*pJogador2->getCorpo()))
					inimigos.at(i)->atacar(2);
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

	void Gerenciador_Colisoes::removeInimigo(Entidades::Personagens::Inimigo* inimigo)
	{
		inimigos.erase(std::remove(inimigos.begin(), inimigos.end(), inimigo),
			inimigos.end());
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

	void Gerenciador_Colisoes::verificaColisao(Entidades::Personagens::Personagem* corpo, RectangleShape* plataforma)
	{
		const sf::FloatRect a = corpo->getCorpo()->getGlobalBounds();
		const sf::FloatRect b = plataforma->getGlobalBounds();

		sf::FloatRect intersecao;
		if (!a.intersects(b, intersecao))
			return;

		// Resolucao por profundidade de penetracao: empurra o corpo pelo
		// eixo de MENOR sobreposicao. Isso elimina o "teletransporte"
		// que a cadeia if/else antiga causava ao tocar quinas.
		const float penX = intersecao.width;
		const float penY = intersecao.height;

		if (penX < penY)
		{
			// Colisao predominantemente lateral.
			const float centroA = a.left + a.width / 2.0f;
			const float centroB = b.left + b.width / 2.0f;

			if (centroA < centroB)
				corpo->setPos(a.left - penX, corpo->getPos().y);
			else
				corpo->setPos(a.left + penX, corpo->getPos().y);
		}
		else
		{
			// Colisao predominantemente vertical.
			const float centroA = a.top + a.height / 2.0f;
			const float centroB = b.top + b.height / 2.0f;

			corpo->setY(0.0f);   // zera o acumulador vertical

			if (centroA < centroB)
			{
				// Corpo acima: pousa sobre a plataforma.
				corpo->setPos(corpo->getPos().x, a.top - penY);
				corpo->setIsJumping(false);
			}
			else
			{
				// Corpo abaixo: bate a cabeca na plataforma.
				corpo->setPos(corpo->getPos().x, a.top + penY);
			}
		}
	}

	void Gerenciador_Colisoes::limparListas() {

		inimigos.clear();

		corpos.clear(); 

		obstaculos.clear();
	}

}



