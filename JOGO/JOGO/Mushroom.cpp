#include "Mushroom.hpp"

#include "iostream"

using namespace std;

namespace Entidades
{
	namespace Personagens
	{

		Mushroom::Mushroom(Vector2f pos, Vector2f tam, Jogador* jogador) :
			Inimigo(pos, tam, jogador)

		{
			sprite.setPosition(pos);
			inicializaAnimacoes();
			voador = false;
			vel = Vector2f(0.1f, 0.1f);
			distanciaAlvo = 30.0f;
			//corpo.setFillColor(sf::Color::Red);

		}

		Mushroom::~Mushroom()
		{
		}

		void Mushroom::atualizarAnimacao()
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

			sprite.setTexture(animacaoAtual->getFrame(count));
			sprite.setScale(1.6 * lado, 1.6);
			sprite.setPosition(corpo.getPosition().x + 20.0f, corpo.getPosition().y);
		}

		void Mushroom::atacar()
		{
			animacaoAtual = &animacoes[3];
		}

		void Mushroom::inicializaAnimacoes()
		{
			Animacao animacaoAndando;
			Animacao animacaoTomarDano;
			Animacao animacaoMorte;
			Animacao animacaoAtacar;
			Animacao animacaoParado;

			sf::Texture texture;
			int pedacoWidth = 150; //Largura
			int pedacoHeight = 150; //Altura

			sf::Vector2f spriteOrigin(corpo.getSize().x / 0.5f, corpo.getSize().y / 1.6f);
			sprite.setOrigin(spriteOrigin);

			//WALK 0 
			if (!texture.loadFromFile("Assets/Monsters/Mushroom/Run.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAndando.addFrame(pedacoTexture);
			}

			//TOMAR DANO 1
			if (!texture.loadFromFile("Assets/Monsters/Mushroom/TakeHit.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoTomarDano.addFrame(pedacoTexture);
			}

			//MORTE 2
			if (!texture.loadFromFile("Assets/Monsters/Mushroom/Death.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoMorte.addFrame(pedacoTexture);
			}

			//ATACAR 3
			if (!texture.loadFromFile("Assets/Monsters/Mushroom/Attack.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar.addFrame(pedacoTexture);
			}

			//ATACAR 4
			if (!texture.loadFromFile("Assets/Monsters/Mushroom/Idle.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoParado.addFrame(pedacoTexture);
			}

			animacoes.push_back(animacaoAndando);
			animacoes.push_back(animacaoTomarDano);
			animacoes.push_back(animacaoMorte);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoParado);

		}
	}
}