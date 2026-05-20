#include "Camera.hpp"
#include "Mundo.hpp"
#include "Jogador.hpp"
#include <cmath>
#include <cstdlib>

namespace Sistemas
{
	namespace
	{
		// Fracao do caminho ate o alvo percorrida por passo: da um
		// acompanhamento suave sem deixar a camera "presa" ao jogador.
		constexpr float SUAVIZACAO = 0.22f;
	}

	Camera::Camera() :
		gerenciadorGrafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
		centroX(0.0f),
		inicializada(false),
		limiteMinX(0.0f),
		limiteMaxX(0.0f),
		temLimites(false),
		shakeFrames(0),
		shakeIntensidade(0.0f),
		flashFrames(0),
		flashFramesMax(0),
		flashIntensidade(0.0f),
		venenoAtivo(false),
		centroX2(0.0f),
		inicializada2(false)
	{
	}

	void Camera::definirLimites(float minX, float maxX)
	{
		limiteMinX = minX;
		limiteMaxX = maxX;
		temLimites = (minX < maxX);
	}

	void Camera::limparLimites()
	{
		temLimites = false;
	}

	void Camera::dispararTremor(int frames, float intensidade)
	{
		// Mantem o mais intenso entre o pedido atual e o que ja estava
		// rolando, para que cliques rapidos de ataque nao "cortem" o
		// tremor anterior.
		if (frames > shakeFrames) shakeFrames = frames;
		if (intensidade > shakeIntensidade) shakeIntensidade = intensidade;
	}

	void Camera::dispararFlashDano(int frames, int alphaInicial)
	{
		if (frames > flashFrames)
		{
			flashFrames = frames;
			flashFramesMax = frames;
		}
		if (static_cast<float>(alphaInicial) > flashIntensidade)
			flashIntensidade = static_cast<float>(alphaInicial);
	}

	void Camera::setEnvenenado(bool ativo)
	{
		venenoAtivo = ativo;
	}

	void Camera::atualizar(Mundo& mundo, const sf::Texture& texturaFundo)
	{
		Entidades::Personagens::Jogador* jogador = mundo.getJogador(0);
		Entidades::Personagens::Jogador* jogador2 = mundo.getJogador(1);

		// Alvo horizontal: jogador, ou o ponto medio entre os dois.
		float alvoX = centroX;
		if (jogador && jogador2)
			alvoX = (jogador->getPos().x + jogador2->getPos().x) / 2.0f;
		else if (jogador2)
			alvoX = jogador2->getPos().x;
		else if (jogador)
			alvoX = jogador->getPos().x;

		// Primeiro quadro: encaixa direto, sem deslizar do canto da tela.
		if (!inicializada)
		{
			centroX = alvoX;
			inicializada = true;
		}
		else
		{
			centroX += (alvoX - centroX) * SUAVIZACAO;
		}

		// Clampa a camera dentro dos limites da fase para nao revelar
		// o "vazio" antes da parede inicial ou depois do portal.
		float centroEfetivo = centroX;
		if (temLimites)
		{
			const float metade = TELA_X / 2.0f;
			const float minimo = limiteMinX + metade;
			const float maximo = limiteMaxX - metade;
			if (minimo < maximo)
			{
				if (centroEfetivo < minimo) centroEfetivo = minimo;
				if (centroEfetivo > maximo) centroEfetivo = maximo;
			}
			else
			{
				// Fase mais estreita que a tela: fixa no centro.
				centroEfetivo = (limiteMinX + limiteMaxX) / 2.0f;
			}
		}

		// Aplica o tremor antes de arredondar para nao ver-lo "stutter".
		float offsetShakeX = 0.0f;
		float offsetShakeY = 0.0f;
		if (shakeFrames > 0)
		{
			offsetShakeX = (static_cast<float>(std::rand()) / RAND_MAX - 0.5f)
				* 2.0f * shakeIntensidade;
			offsetShakeY = (static_cast<float>(std::rand()) / RAND_MAX - 0.5f)
				* 2.0f * shakeIntensidade;
			shakeFrames--;
			if (shakeFrames == 0)
				shakeIntensidade = 0.0f;
		}

		const float centroXInteiro = std::round(centroEfetivo + offsetShakeX);
		const float centroYInteiro = std::round(TELA_Y / 2.0f + offsetShakeY);

		sf::Vector2f cameraCenter(centroXInteiro, centroYInteiro);

		// Fundo: usa centroEfetivo (sem o shake) para a textura nao
		// pular junto, dando uma sensacao de "camera tremula" sobre
		// um cenario estavel.
		sf::Sprite sprite(texturaFundo);
		sprite.setScale(0.9f, 0.9f);
		sprite.setPosition(std::round(centroEfetivo) - (TELA_X / 2.0f), -30.0f);

		gerenciadorGrafico->desenhaSprite(sprite);
		gerenciadorGrafico->setCentro(cameraCenter);
	}

	void Camera::renderizarViewport(float focoX, const sf::FloatRect& viewport,
		const sf::Texture& texturaFundo, bool primario)
	{
		// Cada viewport renderiza meia tela. Para evitar distorcao
		// horizontal (compressao da imagem para caber no viewport),
		// o tamanho LOGICO da view tambem e reduzido pela mesma
		// proporcao.
		const float largura = TELA_X * viewport.width;
		const float altura  = TELA_Y * viewport.height;

		// Suavizacao + clamp para o centro especifico desta viewport.
		float& centroSuave = primario ? centroX : centroX2;
		bool& iniciado = primario ? inicializada : inicializada2;

		if (!iniciado)
		{
			centroSuave = focoX;
			iniciado = true;
		}
		else
		{
			centroSuave += (focoX - centroSuave) * SUAVIZACAO;
		}

		float centroEfetivo = centroSuave;
		if (temLimites)
		{
			const float metade = largura / 2.0f;
			const float minimo = limiteMinX + metade;
			const float maximo = limiteMaxX - metade;
			if (minimo < maximo)
			{
				if (centroEfetivo < minimo) centroEfetivo = minimo;
				if (centroEfetivo > maximo) centroEfetivo = maximo;
			}
			else
			{
				centroEfetivo = (limiteMinX + limiteMaxX) / 2.0f;
			}
		}

		// Tremor da camera primaria so afeta a viewport primaria; a
		// secundaria fica estavel (decisao de design para nao virar
		// confuso no split).
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		if (primario && shakeFrames > 0)
		{
			offsetX = (static_cast<float>(std::rand()) / RAND_MAX - 0.5f)
				* 2.0f * shakeIntensidade;
			offsetY = (static_cast<float>(std::rand()) / RAND_MAX - 0.5f)
				* 2.0f * shakeIntensidade;
			shakeFrames--;
			if (shakeFrames == 0)
				shakeIntensidade = 0.0f;
		}

		const float centroXFinal = std::round(centroEfetivo + offsetX);
		const float centroYFinal = std::round(altura / 2.0f + offsetY);

		// Configura a view do SFML.
		sf::View view(sf::FloatRect(0.0f, 0.0f, largura, altura));
		view.setCenter(centroXFinal, centroYFinal);
		view.setViewport(viewport);

		auto* janela = gerenciadorGrafico->getJanela();
		janela->setView(view);

		// Background. Escalado para cobrir a largura logica da view.
		sf::Sprite sprite(texturaFundo);
		sprite.setScale(0.9f, 0.9f);
		sprite.setPosition(std::round(centroEfetivo) - (largura / 2.0f), -30.0f);
		janela->draw(sprite);
	}

	void Camera::resetarViewportCheio()
	{
		// Reativa uma view padrao em viewport(0,0,1,1) - usada quando
		// o split desliga ou quando o HUD/overlays sao desenhados.
		sf::View view(sf::FloatRect(0.0f, 0.0f, TELA_X, TELA_Y));
		view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
		gerenciadorGrafico->getJanela()->setView(view);
	}

	void Camera::desenharOverlays()
	{
		auto* janela = gerenciadorGrafico->getJanela();
		const sf::View viewJogo = janela->getView();
		sf::View viewUI(sf::FloatRect(0.0f, 0.0f, TELA_X, TELA_Y));
		janela->setView(viewUI);

		if (venenoAtivo)
		{
			// Tint verde sutil: cobre a tela toda mas com alpha baixo,
			// suficiente para sinalizar o status sem comprometer a
			// leitura do cenario.
			sf::RectangleShape veneno;
			veneno.setSize(sf::Vector2f(TELA_X, TELA_Y));
			veneno.setFillColor(sf::Color(40, 200, 60, 50));
			janela->draw(veneno);
		}

		if (flashFrames > 0)
		{
			const float t = static_cast<float>(flashFrames) /
				std::max(1, flashFramesMax);
			const sf::Uint8 alpha = static_cast<sf::Uint8>(flashIntensidade * t);

			sf::RectangleShape flash;
			flash.setSize(sf::Vector2f(TELA_X, TELA_Y));
			flash.setFillColor(sf::Color(220, 30, 30, alpha));
			janela->draw(flash);

			flashFrames--;
			if (flashFrames == 0)
				flashIntensidade = 0.0f;
		}

		janela->setView(viewJogo);
	}
}
