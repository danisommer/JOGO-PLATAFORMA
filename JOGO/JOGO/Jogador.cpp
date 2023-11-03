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
			anterior(0),
			iteracoes(0),
			ataque(0)
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
			else if (Keyboard::isKeyPressed(Keyboard::S))
			{
				animacao = 6;
			}
			else
			{
				animacao = 1;
			}

			// Pular
			if (Keyboard::isKeyPressed(Keyboard::W) && !isJumping)
			{
				velocity.y = jumpStrength;
				isJumping = true;
				animacao = 2;
			}

			if (Keyboard::isKeyPressed(Keyboard::E))
			{
				direcao.x = 0.0;

				if (ataque == 1)
					animacao = 5;
				else
					animacao = 4;
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

					if (ataque == 1)
						ataque = 0;
					else
						ataque = 1;
				}

				iteracoes = 0;
			}
			iteracoes++;

			sprite.setTexture(animacaoAtual->getFrame(count));
			sprite.setScale(lado * 2, 2);
			sprite.setPosition(corpo.getPosition().x + 15.0f, corpo.getPosition().y);

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

			sf::Vector2f spriteOrigin(corpo.getSize().x / 0.55, corpo.getSize().y / 2.0f);
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

			//ATAQUE PESADO 4
			if (!texture.loadFromFile("Assets/Jogador/_Attack.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar.addFrame(pedacoTexture);
			}

			//ATAQUE LEVE 5
			if (!texture.loadFromFile("Assets/Jogador/_Attack2.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar2.addFrame(pedacoTexture);
			}

			//ATACAR 6
			if (!texture.loadFromFile("Assets/Jogador/_Crouch.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAgachar.addFrame(pedacoTexture);
			}

			animacoes.push_back(animacaoAndar);
			animacoes.push_back(animacaoParado);
			animacoes.push_back(animacaoPulo);
			animacoes.push_back(animacaoVoltar);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoAtacar2);
			animacoes.push_back(animacaoAgachar);

		}

		void Jogador::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}
	}
}

