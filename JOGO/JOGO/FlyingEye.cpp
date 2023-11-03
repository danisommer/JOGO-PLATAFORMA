#include "FlyingEye.hpp"
#include "iostream"

using namespace std;

namespace Entidades
{
	namespace Personagens
	{

		FlyingEye::FlyingEye(Vector2f pos, Vector2f tam, Jogador* jogador):
			Inimigo(pos, tam, jogador),
			animacaoAtual(nullptr),
			n_frames(0),
			count(0),
			lado(0),
			animacao(0),
			anterior(0),
			iteracoes(0)
		{
			sprite.setPosition(pos);
			inicializaAnimacoes();
			voador = true;
			vel = Vector2f(0.15f, 0.1f);
			distanciaAlvo = 30.0f;
			//corpo.setFillColor(sf::Color::Red);

		}

		FlyingEye::~FlyingEye()
		{
		}

		void FlyingEye::atualizarAnimacao()
		{
			if (animacao != anterior)
			{
				count = 0;
				iteracoes = 0;
			}

			anterior = animacao;

			animacaoAtual = &animacoes[animacao];

			n_frames = animacaoAtual->getNumFrames();

			static sf::Clock clock;
			sf::Time elapsed = clock.getElapsedTime();

			if (iteracoes > animacaoAtual->getAnimationSpeed())
			{
				if (count < n_frames - 1)
				{
					count++;
				}
				else
				{
					count = 0;
				}

				iteracoes = 0;
			}
			iteracoes++;


			int lado;

			if (direita)
				lado = 1;
			else
				lado = -1;

			animacaoAtual = &animacoes[0];
			sprite.setTexture(animacaoAtual->getFrame(count));
			sprite.setScale(2 * lado, 2);
			sprite.setPosition(corpo.getPosition().x + 20.0f, corpo.getPosition().y);
		}

		void FlyingEye::inicializaAnimacoes()
		{
			Animacao animacaoVoando;
			Animacao animacaoTomarDano;
			Animacao animacaoMorte;
			Animacao animacaoAtacar;

			sf::Texture texture;
			int pedacoWidth = 150; //Largura
			int pedacoHeight = 150; //Altura

			sf::Vector2f spriteOrigin(corpo.getSize().x / 0.5f, corpo.getSize().y / 1.25);
			sprite.setOrigin(spriteOrigin);

			//VOANDO 0 
			if (!texture.loadFromFile("Assets/Monsters/FlyingEye/Flight.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoVoando.addFrame(pedacoTexture);
			}

			//TOMAR DANO 1
			if (!texture.loadFromFile("Assets/Monsters/FlyingEye/TakeHit.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoTomarDano.addFrame(pedacoTexture);
			}

			//MORTE 2
			if (!texture.loadFromFile("Assets/Monsters/FlyingEye/Death.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoMorte.addFrame(pedacoTexture);
			}

			//ATACAR 3
			if (!texture.loadFromFile("Assets/Monsters/FlyingEye/Attack.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar.addFrame(pedacoTexture);
			}

			animacoes.push_back(animacaoVoando);
			animacoes.push_back(animacaoTomarDano);
			animacoes.push_back(animacaoMorte);
			animacoes.push_back(animacaoAtacar);

		}
	}
}