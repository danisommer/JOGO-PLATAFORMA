#include "Portal.hpp"

namespace Entidades
{
	Portal::Portal(const Vector2f pos, const Vector2f tam) :
		Entidade(pos, tam),
		cont(0),
		terminou(false),
		iteracoes(0)
	{
		sprite.setPosition(pos);
		inicializaAnimacoes();
	}

	Portal::~Portal()
	{
	}

	void Portal::inicializaAnimacoes()
	{

		sf::Texture texture;

		for (int i = 1; i < 41; i++) {
			if (!texture.loadFromFile("Assets/Portal/Portal (" + std::to_string(i) + ").png"))
			{
				exit(1);
			}

			animacaoTeleporte.setAnimationSpeed(10.0f);

			animacaoTeleporte.addFrame(texture);
		}
	}

	void Portal::atualizarAnimacao()
	{
		if (iteracoes > animacaoTeleporte.getAnimationSpeed())
		{
			if (cont < 40)
			{
				sprite.setTexture(animacaoTeleporte.getFrame(cont));
				cont++;
			}
			else
			{
				terminou = true;
			}

			iteracoes = 0;
		}

		iteracoes++;

		sprite.setScale(1.5f, 1.5f);

		desenharSprite();
	}

	bool Portal::getTerminou()
	{
		return terminou;
	}
	void Portal::atualizar()
	{
		atualizarAnimacao();
	}

	void Portal::desenharSprite()
	{
		gerenciador_grafico->desenhaSprite(sprite);
	}

}

