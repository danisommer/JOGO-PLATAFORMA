#include "Jogador.hpp"
#include <iostream>

namespace Entidades
{
	namespace Personagens
	{
		Jogador::Jogador(const Vector2f pos, const Vector2f tam) :
			aceleracao(900.0f),
			desaceleracao(900.0f),
			velocidadeMaxima(600.0f),
			jumpStrength(-0.155f),
			Personagem(),
			n_frames(0),
			count(0),
			lado(1),
			animacao(0),
			velAnimacao(0.0f),
			anterior(0),
			iteracoes(0)
		{
			sprite.setPosition(pos);
			corpo.setSize(tam);
			corpo.setPosition(pos);
			corpo.setFillColor(sf::Color::Transparent);
			vel = Vector2f(0.80f, 0.80f);

			inicializaAnimacoes();
		}

		Jogador::~Jogador() = default;

		void Jogador::atualizar()
		{
			

			sf::Vector2f direcao(0.0f, 0.0f);

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				direcao.x = -vel.x;
				animacao = 0;
				lado = -1;
			}
			else if (Keyboard::isKeyPressed(Keyboard::D))
			{
				direcao.x = vel.x;
				animacao = 0;
				lado = 1;
			}
			else
			{
				animacao = 1;
			}

			// Pular
			if (Keyboard::isKeyPressed(Keyboard::Space) && !isJumping)
			{
				velocity.y = jumpStrength;
				isJumping = true;
				animacao = 2;
			}

			if (Keyboard::isKeyPressed(Keyboard::C))
			{
				direcao.x = 0.0;
				animacao = 0 + 5;
			}


			if (isJumping)
			{
				animacao = 2;
			}

			if (animacao != anterior)
			{
				count = 0;
				iteracoes = 0;
			}

			anterior = animacao;


			animacaoAtual = &animacoes[animacao];

			n_frames = animacaoAtual->getNumFrames();
			velAnimacao = animacaoAtual->getAnimationSpeed();

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

			sprite.setTexture(animacaoAtual->getFrame(count));
			sprite.setScale(lado * 2, 2);
			sprite.setPosition(corpo.getPosition().x, corpo.getPosition().y);

			corpo.move(direcao.x, 0.0f);

			clock.restart();
		}

		void Jogador::inicializaAnimacoes()
		{
			Animacao animacaoParado;
			Animacao animacaoPulo;
			Animacao animacaoAndar;
			Animacao animacaoAtacar;
			Animacao animacaoAtacar2;
			Animacao animacaoVoltar;
			Animacao animacaoAgachar;
			sf::Texture texture;
			int pedacoWidth = 120; //Largura
			int pedacoHeight = 80; //Altura

			//corpo.setOrigin(corpo.getSize().x / 0.93, corpo.getSize().y / 2.0f);
			sf::Vector2f spriteOrigin(corpo.getSize().x / 0.7, corpo.getSize().y / 2.0f);
			sprite.setOrigin(spriteOrigin);


			//ANDAR 0 
			if (!texture.loadFromFile("Assets/Jogador/_Run.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAndar.addFrame(pedacoTexture);
			}

			//PARADO 1
			if (!texture.loadFromFile("Assets/Jogador/_Idle.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoParado.addFrame(pedacoTexture);
			}

			//PULO 2
			if (!texture.loadFromFile("Assets/Jogador/_Jump.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoPulo.addFrame(pedacoTexture);
			}

			//VOLTAR 3
			if (!texture.loadFromFile("Assets/Jogador/_TurnAround.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoVoltar.addFrame(pedacoTexture);
			}

			//ATACAR 4
			if (!texture.loadFromFile("Assets/Jogador/_Attack.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar.addFrame(pedacoTexture);
			}


			//ATACAR 4
			if (!texture.loadFromFile("Assets/Jogador/_Attack2.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar2.addFrame(pedacoTexture);
			}

			animacoes.push_back(animacaoAndar);
			animacoes.push_back(animacaoParado);
			animacoes.push_back(animacaoPulo);
			animacoes.push_back(animacaoVoltar);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoAtacar2);

		}

		void Jogador::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}
	}
}

