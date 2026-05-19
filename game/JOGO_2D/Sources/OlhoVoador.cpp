#include "OlhoVoador.hpp"
#include "Gerenciador_Recursos.hpp"
#include "Mundo.hpp"
#include "iostream"
#define VIDA_MAX 45.0f
#define SIZE 1.8f

using namespace std;

namespace Entidades
{
	namespace Personagens
	{

		OlhoVoador::OlhoVoador(Vector2f pos, Vector2f tam):
			Inimigo(pos, tam),
			tempoLentidao(700),
			forcaLentidao(0.5f),
			forcaPulo(-0.14f)


		{
			sprite.setPosition(pos);
			inicializaAnimacoes();
			voador = true;
			vel = Vector2f(0.4f, 0.45f);
			distanciaAlvo = 30.0f;
			ALCANCE_X = 900.0f;
			ALCANCE_Y = 900.0f;
			corpo.setFillColor(sf::Color::Red);
			vida = VIDA_MAX;
			dano = 0.35f;

			healthBar.setScale(vida / 500.0f, 0.2f);

		}

		OlhoVoador::~OlhoVoador()
		{
		}

		void OlhoVoador::atacar(int jogador)
		{
			if (!morto && concluida)
			{
				Jogador* alvo = mundo ? mundo->getJogador(jogador - 1) : nullptr;
				if (alvo)
				{
					alvo->tomarDano(dano);
					alvo->setLento(true, tempoLentidao, forcaLentidao, forcaPulo);
				}
			}

			animacao = 3;
		}

		void OlhoVoador::inicializaAnimacoes()
		{
			auto* recursos = Gerenciadores::Gerenciador_Recursos::getGerenciador();
			const std::string base = "Assets/Monsters/OlhoVoador/";

			const int pedacoWidth = 150;  //Largura
			const int pedacoHeight = 150; //Altura

			sf::Vector2f spriteOrigin(corpo.getSize().x / 0.5f, corpo.getSize().y / 0.85f);
			sprite.setOrigin(spriteOrigin);

			Animacao animacaoVoando;
			Animacao animacaoTomarDano;
			Animacao animacaoMorte;
			Animacao animacaoAtacar;
			Animacao animacaoParado;

			//VOANDO 0
			animacaoVoando.fatiarSpritesheet(recursos->getTextura(base + "Flight.png"), pedacoWidth, pedacoHeight);

			//TOMAR DANO 1
			animacaoTomarDano.fatiarSpritesheet(recursos->getTextura(base + "TakeHit.png"), pedacoWidth, pedacoHeight);
			animacaoTomarDano.setAnimationSpeed(20.0f);

			//MORTE 2
			animacaoMorte.fatiarSpritesheet(recursos->getTextura(base + "Death.png"), pedacoWidth, pedacoHeight);
			animacaoMorte.setAnimationSpeed(90.0f);

			//ATACAR 3
			animacaoAtacar.fatiarSpritesheet(recursos->getTextura(base + "Attack.png"), pedacoWidth, pedacoHeight);
			animacaoAtacar.setAnimationSpeed(15.0f);

			//PARADO 4
			animacaoParado.fatiarSpritesheet(recursos->getTextura(base + "Flight.png"), pedacoWidth, pedacoHeight);

			animacoes.push_back(animacaoVoando);
			animacoes.push_back(animacaoTomarDano);
			animacoes.push_back(animacaoMorte);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoParado);

		}

		void OlhoVoador::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}

		float OlhoVoador::getVida()
		{
			return VIDA_MAX;
		}

		float OlhoVoador::getSize()
		{
			return SIZE;
		}
		void OlhoVoador::atualizar()
		{
			Jogador* jogador1 = mundo ? mundo->getJogador(0) : nullptr;
			Jogador* jogador2 = mundo ? mundo->getJogador(1) : nullptr;

			if (!parado)
			{
				Vector2f posJogador;
				Vector2f posInimigo = corpo.getPosition();

				if (jogador1)
				{
					posJogador = jogador1->getCorpo()->getPosition();
				}
				else if (jogador2)
				{
					posJogador = jogador2->getCorpo()->getPosition();
				}
				else
				{
					moveAleatorio();
					return; 
				}

				float distanciaJogador1 = sqrt(pow(posJogador.x - posInimigo.x, 2) + pow(posJogador.y - posInimigo.y, 2));

				if (jogador2)
				{
					Vector2f posJogador2 = jogador2->getCorpo()->getPosition();
					float distanciaJogador2 = sqrt(pow(posJogador2.x - posInimigo.x, 2) + pow(posJogador2.y - posInimigo.y, 2));

					if (distanciaJogador1 <= ALCANCE_X && distanciaJogador1 <= ALCANCE_Y &&
						(distanciaJogador1 < distanciaJogador2 || distanciaJogador2 > ALCANCE_X || distanciaJogador2 > ALCANCE_Y))
					{
						perseguirJogador(posJogador, posInimigo);
					}
					else if (distanciaJogador2 <= ALCANCE_X && distanciaJogador2 <= ALCANCE_Y)
					{
						perseguirJogador(posJogador2, posInimigo);
					}
					else
					{
						moveAleatorio();
					}
				}
				else
				{
					if (distanciaJogador1 <= ALCANCE_X && distanciaJogador1 <= ALCANCE_Y)
					{
						perseguirJogador(posJogador, posInimigo);
					}
					else
					{
						moveAleatorio();
					}
				}
			}

			atualizarAnimacao();

			if (vida <= 0.0f)
			{
				parado = true;
				animacao = 2;
				voador = false;
				// corpo.move(0.0f, 0.9f);
			}

			desenharSprite();

		}

		void OlhoVoador::limparArquivo(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_olhoVoador.txt",  std::ofstream::out);

			if (arquivo.is_open())
			{
				arquivo.close();
			}
		}

		void OlhoVoador::salvar(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_olhoVoador.txt", std::ios::app);

			if (arquivo.is_open())
			{
				arquivo << "Vida: " << vida << "\n";
				arquivo << "Posicao: " << corpo.getPosition().x << " " << corpo.getPosition().y << "\n";

				arquivo.close();
			}
		}
	}
}