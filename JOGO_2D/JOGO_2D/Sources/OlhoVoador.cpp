#include "OlhoVoador.hpp"
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
			ALCANCE_Y = 900.0f;
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
			if (!morto)
				if (concluida)
				{
					if (jogador == 1)
					{
						jogador1->tomarDano(dano);
						jogador1->setLento(true, tempoLentidao, forcaLentidao, forcaPulo);
					}
					else if (jogador == 2)
					{
						jogador2->tomarDano(dano);
						jogador2->setLento(true, tempoLentidao, forcaLentidao, forcaPulo);
					}

				}

			animacao = 3;
		}

		void OlhoVoador::inicializaAnimacoes()
		{
			Animacao animacaoVoando;
			Animacao animacaoTomarDano;
			Animacao animacaoMorte;
			Animacao animacaoAtacar;
			Animacao animacaoParado;

			sf::Texture texture;
			int pedacoWidth = 150; //Largura
			int pedacoHeight = 150; //Altura

			sf::Vector2f spriteOrigin(corpo.getSize().x / 0.5f, corpo.getSize().y / 0.85f);
			sprite.setOrigin(spriteOrigin);

			//VOANDO 0 
			if (!texture.loadFromFile("Assets/Monsters/OlhoVoador/Flight.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoVoando.addFrame(pedacoTexture);
			}

			//TOMAR DANO 1
			if (!texture.loadFromFile("Assets/Monsters/OlhoVoador/TakeHit.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoTomarDano.addFrame(pedacoTexture);
			}

			animacaoTomarDano.setAnimationSpeed(20.0f);

			//MORTE 2
			if (!texture.loadFromFile("Assets/Monsters/OlhoVoador/Death.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoMorte.addFrame(pedacoTexture);
			}

			animacaoMorte.setAnimationSpeed(90.0f);

			//ATACAR 3
			if (!texture.loadFromFile("Assets/Monsters/OlhoVoador/Attack.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar.addFrame(pedacoTexture);
			}

			animacaoAtacar.setAnimationSpeed(15.0f);

			//PARADO 4 
			if (!texture.loadFromFile("Assets/Monsters/OlhoVoador/Flight.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
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


	}
}