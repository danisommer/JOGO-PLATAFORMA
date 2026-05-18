#include "Serra.hpp"
#include "Gerenciador_Recursos.hpp"


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
			auto* recursos = Gerenciadores::Gerenciador_Recursos::getGerenciador();
			const int largura = 32;

			animacao.fatiarSpritesheet(
				recursos->getTextura("Assets/Cenario/Trap/Serra.png"), largura, largura);
			animacao.setAnimationSpeed(8.0f);
		}

		void Serra::atualizaAnimacao()
		{
			if (iteracoes > animacao.getAnimationSpeed())
			{
				if (cont < animacao.getNumFrames())
				{
					animacao.aplicar(sprite, cont);
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