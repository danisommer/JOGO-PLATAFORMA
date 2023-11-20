#include "Projetil.hpp"
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <windows.h>

namespace Entidades
{
	Projetil::Projetil(const Vector2f pos, const Vector2f tam, bool direita) :
		Entidade(pos, tam),
		vel(1.6f),
		dano(0.15f),
		colidiu(false),
		explodiu(false),
		distanciaPercorrida(0.0f),
		concluida(false),
		anterior(0),
		n_frames(0),
		count(0),
		lado(0),
		iteracoes(0)
	{
		if (direita)
			lado = 1;
		else
			lado = -1;

		inicializaAnimacoes();
		corpo.setFillColor(Color::Red);
		corpo.setPosition(pos);
		corpo.setSize(tam);
	}

	Projetil::~Projetil()
	{
	}

	void Projetil::atualizar()
	{
		int animacao = 0;

		corpo.move(vel * lado, 0.0f);
		distanciaPercorrida += vel;

		if (distanciaPercorrida >= 1200.0f || colidiu)
		{
			animacao = 1;
			vel = 0.0f;
		}

		atualizarAnimacao(animacao);
	}

	void Projetil::inicializaAnimacoes()
	{
		sprite.setOrigin(Vector2f((corpo.getSize().x / 3.0f), (corpo.getSize().y / 2.0f) + 20.0f));

		Animacao animacaoVoar;
		Animacao animacaoExplodir;
		sf::Texture texture;
		int largura = 64;

		for (int i = 1; i < 60; i++) {
			if (!texture.loadFromFile("Assets/Fireball/" + std::to_string(i) + ".png"))
			{
				exit(1);
			}

			animacaoVoar.addFrame(texture);
		}

		if (!texture.loadFromFile("Assets/Fireball/explosao.png")) {
			exit(1);
		}

		for (unsigned int i = 0; i < texture.getSize().x; i += largura) {
			for (unsigned int j = 0; j < texture.getSize().y; j += largura) {
				sf::IntRect pedacoRect(i, 0, largura, largura);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoExplodir.addFrame(pedacoTexture);
			}
		}
		animacaoVoar.setAnimationSpeed(10.0f);
		animacaoExplodir.setAnimationSpeed(7.0f);


		animacoes.push_back(animacaoVoar);
		animacoes.push_back(animacaoExplodir);

	}


	void Projetil::atualizarAnimacao(int animacao)
	{
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
				concluida = false;
				count++;
			}
			else
			{
				concluida = true;
				count = 0;
			}

			iteracoes = 0;
		}
		iteracoes++;

		if (animacao == 1 && concluida)
			explodiu = true;

		sprite.setTexture(animacaoAtual->getFrame(count));
		sprite.setScale(2.3f * lado, 2.3f);
		sprite.setPosition(corpo.getPosition().x - 80.0f * lado, corpo.getPosition().y);
	}

	const bool Projetil::getExplodiu() const
	{
		return explodiu;
	}

	void Projetil::setColidiu(bool c)
	{
		colidiu = c;
	}

	const float Projetil::getDano() const
	{
		return dano;
	}




}
