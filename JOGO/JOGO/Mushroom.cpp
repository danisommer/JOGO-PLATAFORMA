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
			corpo.setFillColor(sf::Color::Red);

		}

		Mushroom::~Mushroom()
		{
		}

		void Mushroom::atacar()
		{
			if (concluida)
				jogador->tomarDano(0.4f);

			animacao = 3;
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

			animacaoTomarDano.setAnimationSpeed(30.0f);

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

			animacaoMorte.setAnimationSpeed(80.0f);

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

			animacaoAtacar.setAnimationSpeed(25.0f);

			//PARADO 4
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
		void Mushroom::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}
	}
}