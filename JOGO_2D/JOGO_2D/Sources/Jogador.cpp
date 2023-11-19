#include "Jogador.hpp"
#include <iostream>
#define VIDA_MAX 100.0f

namespace Entidades
{
	namespace Personagens
	{
		Jogador::Jogador(const Vector2f pos, const Vector2f tam) :
			aceleracao(900.0f),
			desaceleracao(900.0f),
			velocidadeMaxima(450.0f),
			jumpStrength(-0.16f),
			Personagem(),
			n_frames(0),
			count(0),
			lado(1),
			anterior(0),
			iteracoes(0),
			ataque(0),
			regiaoAtaque(),
			atacando(false),
			vidaAnterior(),
			tempoDecorridoLentidao(0),
			tempoDecorridoVeneno(0),
			tempoVeneno(0),
			tempoLentidao(0)
		{
			dano = 0.15f;
			vida = VIDA_MAX;
			sprite.setPosition(pos);
			corpo.setSize(tam);
			corpo.setPosition(pos);
			corpo.setFillColor(sf::Color::Red);
			vel = Vector2f(1.1f, 1.1f);
			healthBar.setScale(vida / 500.0f, 0.2f);
			inicializaAnimacoes();
		}

		Jogador::~Jogador() = default;

		void Jogador::atualizar()
		{
			if (animacao != 2)
			{
				if (envenenado)
				{
					if (tempoDecorridoVeneno < tempoVeneno)
					{
						sprite.setColor(corEnvenenado);
						vida -= forcaVeneno;
						tempoDecorridoVeneno++;
					}
					else
					{
						sprite.setColor(Color{255, 255, 255});
						tempoDecorridoVeneno = 0;
						envenenado = false;
					}
				}

				if (lento)
				{
					if (tempoDecorridoLentidao < tempoLentidao)
					{
						vel.x = forcaLentidao;
						jumpStrength = forcaPulo;
						tempoDecorridoLentidao++;
						animacoes.at(0).setAnimationSpeed(80.0f);
					}
					else
					{
						jumpStrength = -0.16f;
						vel.x = 1.1f;
						tempoDecorridoLentidao = 0;
						lento = false;
						animacoes.at(0).setAnimationSpeed(25.0f);

					}
				}
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


			atualizarAnimacao(animacao);
		}

		void Jogador::atualizarAnimacao(int animacao)
		{
			if (animacao != anterior)
			{
				count = 0;
				iteracoes = 0;
				concluida = false;
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
			sprite.setScale(lado * 2.5f, 2.5f);
			sprite.setPosition(corpo.getPosition().x + 15.0f, corpo.getPosition().y);
		}

		void Jogador::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}

		const Vector2f Jogador::getRegiaoAtaque() const 
		{
			return regiaoAtaque;
		}

		const float Jogador::getDano() const
		{
			return dano;
		}

		const bool Jogador::getAtacando() const
		{
			return atacando;
		}

		float Jogador::getVida()
		{
			return VIDA_MAX;
		}

		void Jogador::setEnvenenado(bool veneno, int tempo, float intensidade, sf::Color cor)
		{
			envenenado = veneno;
			tempoVeneno = tempo;
			forcaVeneno = intensidade;
			corEnvenenado = cor;
		}

		void Jogador::setLento(bool lentidao, int tempo, float fL, float fP)
		{
			lento = lentidao;
			tempoLentidao = tempo;
			forcaLentidao = fL;
			forcaPulo = fP;
		}


		void Jogador::mover(bool direita, bool esquerda)
		{

			if (direita)
			{
				direcao.x = vel.x;
				animacao = 0;
				lado = 1;
			}
			else if (esquerda)
			{
				direcao.x = -vel.x;
				animacao = 0;
				lado = -1;
			}
			
			if(!direita && !esquerda)
			{
				animacao = 5;
				direcao.x = 0.0f;
			}

			corpo.move(direcao.x, 0.0f);
		}

		void Jogador::bater(bool batendo)
		{
			if (batendo && !isJumping)
			{
				regiaoAtaque = Vector2f(corpo.getPosition().x + (65.0f * lado), corpo.getPosition().y);
				atacando = true;

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

		}

		void Jogador::pular(bool pulando)
		{
			if (pulando)
			{
				if (!isJumping)
				{
					velocity.y = jumpStrength;
					isJumping = true;
					animacao = 6;
				}

			}
			else
			{
				velocity.y = jumpStrength / 1.13f;
			}
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
			Animacao animacaoCair;
			sf::Texture texture;
			int pedacoWidth = 120; //Largura
			int pedacoHeight = 80; //Altura

			sf::Vector2f spriteOrigin(corpo.getSize().x / 0.55f, corpo.getSize().y / 2.0f);
			sprite.setOrigin(spriteOrigin);

			//ANDAR 0 
			if (!texture.loadFromFile("Assets/Jogador/_Run.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAndar.addFrame(pedacoTexture);
			}

			animacaoAndar.setAnimationSpeed(25.0f);

			//TOMAR DANO 1
			if (!texture.loadFromFile("Assets/Jogador/_Hit.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoTomarDano.addFrame(pedacoTexture);
			}

			animacaoTomarDano.setAnimationSpeed(30.0f);


			//MORTE 2
			if (!texture.loadFromFile("Assets/Jogador/_Death.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoMorte.addFrame(pedacoTexture);
			}


			//ATAQUE PESADO 3
			if (!texture.loadFromFile("Assets/Jogador/_Attack.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
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

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar2.addFrame(pedacoTexture);
			}


			//PARADO 5
			if (!texture.loadFromFile("Assets/Jogador/_Idle.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoParado.addFrame(pedacoTexture);
			}

			animacaoParado.setAnimationSpeed(40.0f);

			//PULO 6
			if (!texture.loadFromFile("Assets/Jogador/_Jump.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoPulo.addFrame(pedacoTexture);
			}

			//PULO 6
			if (!texture.loadFromFile("Assets/Jogador/_Fall.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoCair.addFrame(pedacoTexture);
			}



			animacoes.push_back(animacaoAndar);
			animacoes.push_back(animacaoTomarDano);
			animacoes.push_back(animacaoMorte);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoAtacar2);
			animacoes.push_back(animacaoParado);
			animacoes.push_back(animacaoPulo);
			animacoes.push_back(animacaoCair);

		}

	}
}