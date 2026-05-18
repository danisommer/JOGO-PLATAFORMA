#include "Cogumelo.hpp"
#include "Gerenciador_Recursos.hpp"

#include "iostream"
#define VIDA_MAX 60.0f
#define SIZE 1.8f


using namespace std;

namespace Entidades
{
	namespace Personagens
	{

		Cogumelo::Cogumelo(Vector2f pos, Vector2f tam) :
			Inimigo(pos, tam),
			forcaVeneno(0.035f),
			tempoEnvenenamento(700),
			venenoEspecial(false)

		{
			sprite.setPosition(pos);
			inicializaAnimacoes();
			voador = false;
			vel = Vector2f(0.35f, 0.1f);
			distanciaAlvo = 30.0f;
			corpo.setFillColor(sf::Color::Red);
			healthBar.setScale(vida / 500.0f, 0.2f);
			dano = 0.2f;
			vida = VIDA_MAX;

			if (rand() % 3 == 0)
			{
				venenoEspecial = true;
				sprite.setColor(Color{ 235, 255, 0 });
				vida += 20.0f;
			}
		}

		Cogumelo::~Cogumelo()
		{
		}

		void Cogumelo::atacar(int jogador)
		{
			if (!morto)
				if (concluida)
				{
					if (jogador == 1)
					{
						jogador1->tomarDano(dano);
						envenenar(1);
					}
					else if (jogador == 2)
					{
						jogador2->tomarDano(dano);
						envenenar(2);
					}
				}

			animacao = 3;
		}

		void Cogumelo::envenenar(int jogador)
		{
			if (venenoEspecial)
			{
				forcaVeneno = 0.04f;
				corJogador = Color::Green;
			}
			else
			{
				forcaVeneno = 0.035f;
				corJogador = Color{ 100, 255, 100 };

			}
			if (jogador == 1)
			{
				jogador1->setEnvenenado(true, tempoEnvenenamento, forcaVeneno, corJogador);
			}
			else if (jogador == 2)
			{
				jogador2->setEnvenenado(true, tempoEnvenenamento, forcaVeneno, corJogador);
			}
		}

		void Cogumelo::salvar(int save)
		{
			std::ofstream arquivo("Saves/save" + to_string(save) + "_cogumelo.txt", std::ios::app);

			if (arquivo.is_open())
			{
				arquivo << "Vida: " << vida << "\n";
				arquivo << "Posicao: " << corpo.getPosition().x << " " << corpo.getPosition().y << "\n";

				arquivo.close();
			}
		}

		void Cogumelo::limparArquivo(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_cogumelo.txt", std::ofstream::out);

			if (arquivo.is_open())
			{
				arquivo.close();
			}
		}

		void Cogumelo::inicializaAnimacoes()
		{
			auto* recursos = Gerenciadores::Gerenciador_Recursos::getGerenciador();
			const std::string base = "Assets/Monsters/Cogumelo/";

			const int pedacoWidth = 150;  //Largura
			const int pedacoHeight = 150; //Altura

			sf::Vector2f spriteOrigin(corpo.getSize().x / 0.5f, corpo.getSize().y / 1.6f);
			sprite.setOrigin(spriteOrigin);

			Animacao animacaoAndando;
			Animacao animacaoTomarDano;
			Animacao animacaoMorte;
			Animacao animacaoAtacar;
			Animacao animacaoParado;

			//WALK 0
			animacaoAndando.fatiarSpritesheet(recursos->getTextura(base + "Run.png"), pedacoWidth, pedacoHeight);

			//TOMAR DANO 1
			animacaoTomarDano.fatiarSpritesheet(recursos->getTextura(base + "TakeHit.png"), pedacoWidth, pedacoHeight);
			animacaoTomarDano.setAnimationSpeed(20.0f);

			//MORTE 2
			animacaoMorte.fatiarSpritesheet(recursos->getTextura(base + "Death.png"), pedacoWidth, pedacoHeight);
			animacaoMorte.setAnimationSpeed(90.0f);

			//ATACAR 3
			animacaoAtacar.fatiarSpritesheet(recursos->getTextura(base + "Attack.png"), pedacoWidth, pedacoHeight);
			animacaoAtacar.setAnimationSpeed(12.0f);

			//PARADO 4
			animacaoParado.fatiarSpritesheet(recursos->getTextura(base + "Idle.png"), pedacoWidth, pedacoHeight);

			animacoes.push_back(animacaoAndando);
			animacoes.push_back(animacaoTomarDano);
			animacoes.push_back(animacaoMorte);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoParado);

		}
		void Cogumelo::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}

		float Cogumelo::getVida()
		{
			if (venenoEspecial)
				return VIDA_MAX + 20.0f;

			return VIDA_MAX;
		}

		float Cogumelo::getSize()
		{
			return SIZE;
		}
		void Cogumelo::atualizar()
		{
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
				//corpo.move(0.0f, 0.9f);
			}

			desenharSprite();

		}
	}

}