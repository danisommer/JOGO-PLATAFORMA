#include "Serra.hpp"


namespace Entidades
{
	namespace Obstaculos
	{
		Serra::Serra(const Vector2f pos, const Vector2f tam) :
			Obstaculo(pos, tam),
			cont(0),
			iteracoes(0)
		{
			corpo.setFillColor(Color::Red);
			corpo.setPosition(pos);
			corpo.setSize(tam);
			sprite.setPosition(pos);
			inicializaAnimacoes();
		}

		Serra::~Serra()
		{
		}

		void Serra::inicializaAnimacoes()
		{
			sf::Texture texture;
			int largura = 32;

			if (!texture.loadFromFile("Assets/Cenario/Trap/Serra.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += largura) {
				sf::IntRect pedacoRect(x, 0, largura, largura);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacao.addFrame(pedacoTexture);
			}

			animacao.setAnimationSpeed(5.0f);

		}

		void Serra::atualizaAnimacao()
		{
			if (iteracoes > animacao.getAnimationSpeed())
			{
				if (cont < animacao.getNumFrames())
				{
					sprite.setTexture(animacao.getFrame(cont));
					cont++;
				}
				else
				{
					cont = 0;
				}

				iteracoes = 0;
			}

			iteracoes++;

			sprite.setScale(3.0f, 3.0f);

		}

		void Serra::executar()
		{
			atualizaAnimacao();
		}
		//void Serra::danar(Jogador* pJogador)
		//{
		//}
	}
}