#include "FlyingEye.hpp"
#include "iostream"

using namespace std;

namespace Entidades
{
	namespace Personagens
	{

		FlyingEye::FlyingEye(Vector2f pos, Vector2f tam, Jogador* jogador):
			Inimigo(pos, tam, jogador)

		{
			sprite.setPosition(pos);
			inicializaAnimacoes();
			voador = true;
			vel = Vector2f(0.15f, 0.3f);
			distanciaAlvo = 30.0f;
			ALCANCE_Y = 300.0f;
			corpo.setFillColor(sf::Color::Red);

		}

		FlyingEye::~FlyingEye()
		{
		}

		void FlyingEye::atacar()
		{
			if (concluida)
				jogador->tomarDano(0.2f);

			animacao = 3;
		}

		void FlyingEye::inicializaAnimacoes()
		{
			Animacao animacaoVoando;
			Animacao animacaoTomarDano;
			Animacao animacaoMorte;
			Animacao animacaoAtacar;
			Animacao animacaoParado;

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

			animacaoMorte.setAnimationSpeed(80.0f);

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

			animacaoAtacar.setAnimationSpeed(25.0f);

			//PARADO 4 
			if (!texture.loadFromFile("Assets/Monsters/FlyingEye/Flight.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoParado.addFrame(pedacoTexture);
			}

			animacoes.push_back(animacaoVoando);
			animacoes.push_back(animacaoTomarDano);
			animacoes.push_back(animacaoMorte);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoParado);

		}

		void FlyingEye::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}
	}
}