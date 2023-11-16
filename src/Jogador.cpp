#include "Jogador.hpp"
#include <iostream>

namespace Entidades
{
	namespace Personagens
	{
		Jogador::Jogador(const Vector2f pos, const Vector2f tam) :
			aceleracao(900.0f),
			desaceleracao(900.0f),
			velocidadeMaxima(450.0f),
			jumpStrength(-0.13f),
			Personagem(),
			n_frames(0),
			count(0),
			lado(1),
			anterior(0),
			iteracoes(0),
			ataque(0),
			concluida(false),
			regiaoAtaque(),
			dano(0.15f),
			atacando(false)
		{

			sprite.setPosition(pos);
			corpo.setSize(tam);
			corpo.setPosition(pos);
			corpo.setFillColor(sf::Color::Red);
			vel = Vector2f(0.8f, 0.8f);

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
				animacao = 5;
			}

			// Pular
			if (Keyboard::isKeyPressed(Keyboard::W) && !isJumping)
			{
				velocity.y = jumpStrength;
				isJumping = true;
				animacao = 6;
			}
			
			if (Keyboard::isKeyPressed(Keyboard::E))
			{
				atacar(lado);

				direcao.x = 0.0;

				if (ataque == 1)
					animacao = 4;
				else
					animacao = 3;
			}
			else
			{
				regiaoAtaque = Vector2f();
				atacando = false;
			}

			if (isJumping)
			{
				animacao = 6;
			}

			if (vida <= 0.0f)
			{
				vel.x = 0.0f;
				vel.y = 0.0f;
				jumpStrength = 0.0f;

				animacao = 2;
				if (concluida)
					morrer();
			}


			if (animacao != anterior)
			{
				count = 0;
				iteracoes = 0;
			}

			anterior = animacao;

			animacaoAtual = &animacoes[animacao];

			n_frames = animacaoAtual->getNumFrames();

			if (iteracoes > animacaoAtual->getAnimationSpeed())
			{
				if (count < n_frames - 1)
				{
					count++;
					concluida = false;
				}
				else
				{
					count = 0;
					concluida = true;

					if (ataque == 1)
						ataque = 0;
					else
						ataque = 1;
				}

				iteracoes = 0;
			}
			iteracoes++;

			sprite.setTexture(animacaoAtual->getFrame(count));
			sprite.setScale(lado * 2.5, 2.5);
			sprite.setPosition(corpo.getPosition().x + 15.0f, corpo.getPosition().y);

			corpo.move(direcao.x, 0.0f);
		}

		void Jogador::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}

		Vector2f Jogador::getRegiaoAtaque()
		{
			return regiaoAtaque;
		}

		float Jogador::getDano()
		{
			return dano;
		}

		bool Jogador::getAtacando()
		{
			return atacando;
		}

		void Jogador::atacar(int lado)
		{
			regiaoAtaque = Vector2f(corpo.getPosition().x + (65.0f * lado), corpo.getPosition().y);
			atacando = true;
		}

		void Jogador::inicializaAnimacoes()
		{
			Animacao animacaoParado;
			Animacao animacaoPulo;
			Animacao animacaoAndar;
			Animacao animacaoAtacar;
			Animacao animacaoTomarDano;
			Animacao animacaoAtacar2;
			Animacao animacaoMorte;
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

			animacaoAndar.setAnimationSpeed(45.0f);

			//TOMAR DANO 1
			if (!texture.loadFromFile("Assets/Jogador/_Hit.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoTomarDano.addFrame(pedacoTexture);
			}

			//MORTE 2
			if (!texture.loadFromFile("Assets/Jogador/_Death.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoMorte.addFrame(pedacoTexture);
			}


			//ATAQUE PESADO 3
			if (!texture.loadFromFile("Assets/Jogador/_Attack.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar.addFrame(pedacoTexture);
			}

			animacaoAtacar.setAnimationSpeed(20.0f);
			animacaoAtacar2.setAnimationSpeed(20.0f);

			//ATAQUE LEVE 4
			if (!texture.loadFromFile("Assets/Jogador/_Attack2.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar2.addFrame(pedacoTexture);
			}


			//PARADO 5
			if (!texture.loadFromFile("Assets/Jogador/_Idle.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoParado.addFrame(pedacoTexture);
			}

			//PULO 6
			if (!texture.loadFromFile("Assets/Jogador/_Jump.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoPulo.addFrame(pedacoTexture);
			}

			animacoes.push_back(animacaoAndar);
			animacoes.push_back(animacaoTomarDano);
			animacoes.push_back(animacaoMorte);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoAtacar2);
			animacoes.push_back(animacaoParado);
			animacoes.push_back(animacaoPulo);

		}

	}
}