#include "Serra.hpp"


namespace Entidades
{
	namespace Obstaculos
	{
		Serra::Serra(const Vector2f pos, const Vector2f tam) :
			Obstaculo(pos, tam),
			cont(0),
			iteracoes(0),
			dano(0.2f)
		{
			corpo.setFillColor(Color::Red);
			corpo.setPosition(pos);
			corpo.setSize(tam);
			sprite.setPosition(pos);
			inicializaAnimacoes();
			danoso = true;
			colidir = false;
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

			for (unsigned int x = 0; x < texture.getSize().x; x += largura) {
				sf::IntRect pedacoRect(x, 0, largura, largura);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacao.addFrame(pedacoTexture);
			}

			animacao.setAnimationSpeed(2.0f);

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

		void Serra::atualizar()
		{
			atualizaAnimacao();
			desenharSprite();
		}

		void Serra::danar(int jogador)
		{
			if (jogador == 1)
			{
				jogador1->tomarDano(dano);
			}
			else if (jogador == 2)
			{
				jogador2->tomarDano(dano);

			}
		}
		void Serra::limparArquivo(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_serra.txt", std::ofstream::out);

			if (arquivo.is_open())
			{
				arquivo.close();
			}
		}
		void Serra::salvar(int save)
		{
			std::ofstream arquivo("Saves/save" + to_string(save) + "_serra.txt", std::ios::app);

			if (arquivo.is_open())
			{
				arquivo << "Posicao: " << corpo.getPosition().x << " " << corpo.getPosition().y << "\n";

				arquivo.close();
			}
		}

	}
}