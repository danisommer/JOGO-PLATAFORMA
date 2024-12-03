#include "Gerenciador_Grafico.hpp"

using namespace sf;

namespace Gerenciadores
{
	Gerenciador_Grafico* Gerenciador_Grafico::pGerenciador = nullptr;

	Gerenciador_Grafico::Gerenciador_Grafico() :
		janela(new RenderWindow(VideoMode(TELA_X, TELA_Y), "Knight's Quest")),
		view(FloatRect(0.0f, 0.f, TELA_X, TELA_Y))
	{
		if (janela == nullptr)
		{
			cout << "Erro alocando a janela!!" << endl;
			exit(1);
		}
		view.zoom(1.0f);
		janela->setView(view);
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

	void Gerenciador_Grafico::desenhaTela(sf::Drawable* tela)
	{
		janela->draw(*tela);
	}

	void Gerenciador_Grafico::desenhaTexto(sf::Text texto)
	{
		janela->draw(texto);
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

	bool Gerenciador_Grafico::getOpen()
	{
		return janela->isOpen();
	}

	void Gerenciador_Grafico::setCentro(sf::Vector2f centro)
	{
		view.setCenter(centro);
		janela->setView(view);
	}
	Vector2f Gerenciador_Grafico::getViewCenter()
	{
		return view.getCenter();
	}
}
