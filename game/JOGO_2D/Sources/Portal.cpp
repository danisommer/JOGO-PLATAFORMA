#include "Portal.hpp"
#include "Gerenciador_Recursos.hpp"
#include "Mundo.hpp"

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
			auto* recursos = Gerenciadores::Gerenciador_Recursos::getGerenciador();

			// Cada quadro do portal esta em um arquivo separado.
			for (int i = 1; i < 41; i++)
				animacaoTeleporte.adicionarTextura(
					recursos->getTextura("Assets/Portal/Portal (" + std::to_string(i) + ").png"));

			animacaoTeleporte.setAnimationSpeed(10.0f);
		}

		void Portal::atualizarAnimacao()
		{
			if (iteracoes > animacaoTeleporte.getAnimationSpeed())
			{
				if (cont < 40)
				{
					animacaoTeleporte.aplicar(sprite, cont);
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
				Entidades::Personagens::Jogador* alvo =
					mundo ? mundo->getJogador(jogador - 1) : nullptr;
				if (alvo)
					alvo->setConcluiuFase(true);
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

