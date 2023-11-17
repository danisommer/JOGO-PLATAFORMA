#include "Skeleton.hpp"
#include "iostream"
#define VIDA_MAX 100.0f
#define SIZE 1.8f


using namespace std;

namespace Entidades
{
	namespace Personagens
	{

		Skeleton::Skeleton(Vector2f pos, Vector2f tam) :
			Inimigo(pos, tam)

		{
			sprite.setPosition(pos);
			inicializaAnimacoes();
			voador = false;
			vel = Vector2f(0.40f, 0.1f);
			distanciaAlvo = 30.0f;
			corpo.setFillColor(sf::Color::Red);
			dano = 0.4f;
			vida = VIDA_MAX;
		}

		Skeleton::~Skeleton()
		{
		}

		void Skeleton::atacar()
		{
			if(!morto)	
				if (concluida)
					jogador->tomarDano(dano);

			animacao = 3;
		}

		void Skeleton::inicializaAnimacoes()
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
			if (!texture.loadFromFile("Assets/Monsters/Skeleton/Walk.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAndando.addFrame(pedacoTexture);
			}

			//TOMAR DANO 1
			if (!texture.loadFromFile("Assets/Monsters/Skeleton/TakeHit.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoTomarDano.addFrame(pedacoTexture);
			}

			animacaoTomarDano.setAnimationSpeed(25.0f);

			//MORTE 2
			if (!texture.loadFromFile("Assets/Monsters/Skeleton/Death.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoMorte.addFrame(pedacoTexture);
			}

			animacaoMorte.setAnimationSpeed(90.0f);


			//ATACAR 3
			if (!texture.loadFromFile("Assets/Monsters/Skeleton/Attack.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar.addFrame(pedacoTexture);
			}

			animacaoAtacar.setAnimationSpeed(15.0f);

			//PARADO 4
			if (!texture.loadFromFile("Assets/Monsters/Skeleton/Idle.png")) {
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
		void Skeleton::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}

		float Skeleton::getVida()
		{
			return VIDA_MAX;
		}

		float Skeleton::getSize()
		{
			return SIZE;
		}
		void Skeleton::atualizar()
		{
			Vector2f posJogador = jogador->getCorpo().getPosition();
			Vector2f posInimigo = corpo.getPosition();

			if (!parado)
			{
				if (fabs(posJogador.x - posInimigo.x) <= ALCANCE_X && fabs(posJogador.y - posInimigo.y) <= ALCANCE_Y)
				{
					perseguirJogador(posJogador, posInimigo);
				}
				else
				{
					moveAleatorio();
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


		}
	}
}