#include "Gerenciador_Grafico.hpp"
#include "Configuracao.hpp"

using namespace sf;

namespace Gerenciadores
{
	Gerenciador_Grafico* Gerenciador_Grafico::pGerenciador = nullptr;

	Gerenciador_Grafico::Gerenciador_Grafico() :
		janela(nullptr),
		view(FloatRect(0.0f, 0.f, TELA_X, TELA_Y))
	{
		// Cria a janela ja respeitando o arquivo de configuracao.
		// A view e mantida em coordenadas logicas (TELA_X x TELA_Y)
		// para que a arte do jogo nao precise ser reescalada quando
		// o usuario muda a resolucao da janela.
		view.zoom(1.0f);
		aplicarConfiguracao();
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

	void Gerenciador_Grafico::aplicarConfiguracao()
	{
		auto* cfg = Configuracao::getInstancia();

		const unsigned int largura = cfg->getLargura();
		const unsigned int altura = cfg->getAltura();
		const unsigned int estilo = cfg->getTelaCheia()
			? sf::Style::Fullscreen
			: sf::Style::Default;

		if (janela)
		{
			janela->close();
			delete janela;
			janela = nullptr;
		}

		// Em tela cheia o modo de video precisa estar entre os
		// suportados pelo monitor; se nao estiver, caimos no modo
		// nativo da area de trabalho. Isso evita uma falha silenciosa
		// (janela em preto) ao escolher uma resolucao customizada.
		VideoMode modo(largura, altura);
		if (cfg->getTelaCheia() && !modo.isValid())
			modo = VideoMode::getDesktopMode();

		janela = new RenderWindow(modo, "Knight's Quest", estilo);
		if (janela == nullptr)
		{
			cout << "Erro alocando a janela!!" << endl;
			exit(1);
		}

		janela->setView(view);
		janela->setFramerateLimit(cfg->getFpsMax());
	}
}
