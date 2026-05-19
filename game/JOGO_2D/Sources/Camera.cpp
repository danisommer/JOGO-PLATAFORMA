#include "Camera.hpp"
#include "Mundo.hpp"
#include "Jogador.hpp"

namespace Sistemas
{
	Camera::Camera() :
		gerenciadorGrafico(Gerenciadores::Gerenciador_Grafico::getGerenciador())
	{
	}

	void Camera::atualizar(Mundo& mundo, const sf::Texture& texturaFundo)
	{
		Entidades::Personagens::Jogador* jogador = mundo.getJogador(0);
		Entidades::Personagens::Jogador* jogador2 = mundo.getJogador(1);

		sf::Vector2f cameraCenter = gerenciadorGrafico->getViewCenter();
		sf::Vector2f pos;

		if (jogador && jogador2)
		{
			float middleX = (jogador->getPos().x + jogador2->getPos().x) / 2.0f;
			cameraCenter.x = middleX;
			pos.x = middleX - (TELA_X / 2);
			pos.y = -30.0f;
		}
		else if (jogador2)
		{
			cameraCenter.x = jogador2->getPos().x;
			pos.x = jogador2->getPos().x - (TELA_X / 2);
			pos.y = -30.0f;
		}
		else if (jogador)
		{
			cameraCenter.x = jogador->getPos().x;
			pos.x = jogador->getPos().x - (TELA_X / 2);
			pos.y = -30.0f;
		}

		sf::Sprite sprite(texturaFundo);
		sprite.setScale(0.9f, 0.9f);
		sprite.setPosition(pos);

		gerenciadorGrafico->desenhaSprite(sprite);
		gerenciadorGrafico->setCentro(cameraCenter);
	}
}
