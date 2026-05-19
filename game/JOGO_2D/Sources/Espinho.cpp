#include "Espinho.hpp"
#include "Gerenciador_Recursos.hpp"
#include "Mundo.hpp"

namespace Entidades
{
	namespace Obstaculos
	{
		Espinho::Espinho(const Vector2f pos, const Vector2f tam) :
			Obstaculo(pos, tam),
			cont(0),
			iteracoes(0),
			danoInstantaneo(9.0f),
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
			auto* recursos = Gerenciadores::Gerenciador_Recursos::getGerenciador();
			const int largura = 32;

			animacao.fatiarSpritesheet(
				recursos->getTextura("Assets/Cenario/Trap/Espinho.png"), largura, largura);
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
						animacao.aplicar(sprite, cont);
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
				animacao.aplicar(sprite, 0);
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
				Entidades::Personagens::Jogador* alvo =
					mundo ? mundo->getJogador(jogador - 1) : nullptr;
				if (alvo)
				{
					alvo->tomarDano(danoInstantaneo);
					alvo->setEnvenenado(true, tempoEnvenenamento, veneno, Color{ 100, 255, 100 });
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