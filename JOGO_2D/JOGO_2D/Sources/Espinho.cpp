#include "Espinho.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		Espinho::Espinho(const Vector2f pos, const Vector2f tam) :
			Obstaculo(pos, tam),
			cont(0),
			iteracoes(0),
			danoInstantaneo(0.55f),
			pisou(false),
			tempoEnvenenamento(850),
			veneno(0.035f)
		{
			corpo.setFillColor(Color::Red);
			corpo.setPosition(pos.x, pos.y + 30.0f);
			corpo.setSize(tam);
			sprite.setPosition(pos.x,pos.y - 12.0f);
			inicializaAnimacoes();
			danoso = true;
			colidir = false;
		}

		Espinho::~Espinho()
		{
		}

		void Espinho::inicializaAnimacoes()
		{
			sf::Texture texture;
			int largura = 32;

			if (!texture.loadFromFile("Assets/Cenario/Trap/Espinho.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += largura) {
				sf::IntRect pedacoRect(x, 0, largura, largura);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacao.addFrame(pedacoTexture);
			}

			animacao.setAnimationSpeed(15.0f);

		}

		void Espinho::atualizaAnimacao()
		{
			if (pisou)
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
						pisou = false;
					}

					iteracoes = 0;
				}

				iteracoes++;

			}
			else
			{
				sprite.setTexture(animacao.getFrame(0));
			}

			sprite.setScale(3.0f, 3.0f);

		}

		void Espinho::atualizar()
		{
			atualizaAnimacao();
			desenharSprite();
		}

		void Espinho::danar(int jogador)
		{
			if (!pisou) 
			{
				if (jogador == 1)
				{

					jogador1->tomarDano(danoInstantaneo);
					jogador1->setEnvenenado(true, tempoEnvenenamento, veneno, Color{ 100, 255, 100 });
				}
				else if (jogador == 2)
				{

					jogador2->tomarDano(danoInstantaneo);
					jogador2->setEnvenenado(true, tempoEnvenenamento, veneno, Color{ 100, 255, 100 });
				}

			}
			pisou = true;
		}
		void Espinho::limparArquivo(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_espinho.txt", std::ofstream::out);

			if (arquivo.is_open())
			{
				arquivo.close();
			}
		}
		void Espinho::salvar(int save)
		{
			std::ofstream arquivo("Saves/save" + to_string(save) + "_espinho.txt", std::ios::app);

			if (arquivo.is_open())
			{
				arquivo << "Posicao: " << corpo.getPosition().x << " " << corpo.getPosition().y << "\n";

				arquivo.close();
			}
		}
	}
}