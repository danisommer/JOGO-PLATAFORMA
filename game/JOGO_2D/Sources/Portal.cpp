#include "Portal.hpp"

namespace Entidades
{
	namespace Obstaculos
	{

		Portal::Portal(const Vector2f pos, const Vector2f tam, bool i, bool d) :
			Obstaculo(pos, tam),
			cont(0),
			terminou(false),
			iteracoes(0),
			infinito(i)
		{
			corpo.setPosition(pos);
			corpo.setSize(tam);
			corpo.setFillColor(Color::Red);
			sprite.setPosition(pos.x - 40.0f,  pos.y - 40.0f);
			inicializaAnimacoes();

			danoso = d;
			colidir = false;
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
					if (!infinito)
						terminou = true;
					else
						cont = 0;
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

		void Portal::danar(int jogador)
		{
			if (infinito)
			{
				if (jogador == 1)
				{
					jogador1->setConcluiuFase(true);
				}
				else if (jogador == 2)
				{
					jogador2->setConcluiuFase(true);
				}
			}
		}
		void Portal::limparArquivo(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_portal.txt", std::ofstream::out);

			if (arquivo.is_open())
			{
				arquivo.close();
			}
		}
		void Portal::salvar(int save)
		{
			std::ofstream arquivo("Saves/save" + to_string(save) + "_portal.txt", std::ios::app);

			if (arquivo.is_open())
			{
				arquivo << "Posicao: " << corpo.getPosition().x << " " << corpo.getPosition().y << "\n";

				arquivo.close();
			}
		}
	}
}

