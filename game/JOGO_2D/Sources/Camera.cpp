#include "Camera.hpp"
#include "Mundo.hpp"
#include "Jogador.hpp"
#include <cmath>

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
		inicializada(false)
	{
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

		// Arredondar para pixel inteiro elimina o tremor sub-pixel.
		const float centroXInteiro = std::round(centroX);

		sf::Vector2f cameraCenter = gerenciadorGrafico->getViewCenter();
		cameraCenter.x = centroXInteiro;

		sf::Sprite sprite(texturaFundo);
		sprite.setScale(0.9f, 0.9f);
		sprite.setPosition(centroXInteiro - (TELA_X / 2.0f), -30.0f);

		gerenciadorGrafico->desenhaSprite(sprite);
		gerenciadorGrafico->setCentro(cameraCenter);
	}
}
