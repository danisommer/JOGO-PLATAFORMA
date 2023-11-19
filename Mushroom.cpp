#include "Mushroom.hpp"

#include "iostream"
#define VIDA_MAX 100.0f
#define SIZE 1.8f


using namespace std;

namespace Entidades
{
	namespace Personagens
	{

		Mushroom::Mushroom(Vector2f pos, Vector2f tam) :
			Inimigo(pos, tam),
			forcaVeneno(0.035f),
			tempoEnvenenamento(700)

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
		}

		Mushroom::~Mushroom()
		{
		}

		void Mushroom::atacar()
		{
			if (!morto)
				if (concluida)
				{
					jogador1->tomarDano(dano);
					jogador1->setEnvenenado(true, tempoEnvenenamento, forcaVeneno);

				}

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

			animacaoTomarDano.setAnimationSpeed(25.0f);

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

			animacaoMorte.setAnimationSpeed(90.0f);

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

			animacaoAtacar.setAnimationSpeed(20.0f);

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

		float Mushroom::getVida()
		{
			return VIDA_MAX;
		}

		float Mushroom::getSize()
		{
			return SIZE;
		}
		void Mushroom::atualizar()
		{
			Vector2f posJogador = jogador1->getCorpo().getPosition();
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