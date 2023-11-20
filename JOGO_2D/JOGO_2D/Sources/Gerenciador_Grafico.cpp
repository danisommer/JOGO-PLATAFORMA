#include "Gerenciador_Grafico.hpp"

using namespace sf;

namespace Gerenciadores
{
	Gerenciador_Grafico* Gerenciador_Grafico::pGerenciador = nullptr;

	Gerenciador_Grafico::Gerenciador_Grafico() :
		janela(new RenderWindow(VideoMode(TELA_X, TELA_Y), "Knight's Quest")),
		view(FloatRect(0.0f, 0.f, TELA_X, TELA_Y)),
		jogador(nullptr),
		jogador2(nullptr)

	{
		int fase = 2;

		if (janela == nullptr)
		{
			cout << "Erro alocando a janela!!" << endl;
			exit(1);
		}
		view.zoom(1.0f);
		janela->setView(view);

		if (!backgroundTexture.loadFromFile("Assets/Cenario/background_" + std::to_string(fase) + ".png")) {
			exit(1);
		}
	}

	Gerenciador_Grafico::~Gerenciador_Grafico()
	{
		if (janela)
		{
			delete(janela);
			janela = nullptr;
		}

	}

	Gerenciador_Grafico* Gerenciador_Grafico::getGerenciador()
	{
		pGerenciador == nullptr ? pGerenciador = new Gerenciador_Grafico() : pGerenciador;

		return pGerenciador;
	}

	sf::RenderWindow* Gerenciador_Grafico::getJanela()
	{
		return janela;
	}

	void Gerenciador_Grafico::limpaTela()
	{
		janela->clear();
	}

	void Gerenciador_Grafico::desenhaHitbox(const RectangleShape& corpo)
	{
		janela->draw(corpo);
	}

	void Gerenciador_Grafico::desenhaSprite(const Sprite& sprite)
	{
		janela->draw(sprite);
	}

	void Gerenciador_Grafico::mostraElemento()
	{
		janela->display();
	}

	void Gerenciador_Grafico::fecharJanela()
	{
		if (janela->isOpen())
			janela->close();
	}

	void Gerenciador_Grafico::atualizaCamera()
	{
		sf::Vector2f cameraCenter = view.getCenter();
		sf::Sprite sprite(backgroundTexture);
		sprite.setScale(0.9f, 0.9f);

		if (jogador2 && jogador) 
		{
			sf::Vector2f playerPosition = jogador->getPos();
			sf::Vector2f player2Position = jogador2->getPos();

			float middleX = (playerPosition.x + player2Position.x) / 2.0f;

			cameraCenter.x = middleX;

			sprite.setPosition(middleX - (TELA_X / 2), -30.0f);
		}
		else if (jogador2) 
		{
			sf::Vector2f player2Position = jogador2->getPos();

			cameraCenter.x = player2Position.x;

			sprite.setPosition(player2Position.x - (TELA_X / 2), -30.0f);

		}
		else if (jogador) 
		{
			sf::Vector2f playerPosition = jogador->getPos();

			cameraCenter.x = playerPosition.x;

			sprite.setPosition(playerPosition.x - (TELA_X / 2), -30.0f);
		}

		view.setCenter(cameraCenter);
		janela->setView(view);
		janela->draw(sprite);
	}



	bool Gerenciador_Grafico::getOpen()
	{
		return janela->isOpen();
	}

	void Gerenciador_Grafico::setJogador(Entidades::Personagens::Jogador* jogador)
	{
		this->jogador = jogador;
	}
	void Gerenciador_Grafico::setJogador2(Entidades::Personagens::Jogador* jogador)
	{
		this->jogador2 = jogador;
	}
}
