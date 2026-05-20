#include "Projetil.hpp"
#include "Gerenciador_Recursos.hpp"
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>

namespace Entidades
{
	Projetil::Projetil(const Vector2f pos, const Vector2f tam, bool direita) :
		Entidade(pos, tam),
		vel(6.0f),
		dano(18.0f),
		colidiu(false),
		explodiu(false),
		distanciaPercorrida(0.0f),
		concluida(false),
		anterior(0),
		n_frames(0),
		count(0),
		lado(0),
		iteracoes(0),
		guiado(false),
		posAlvo(0.0f, 0.0f),
		angulo(direita ? 0.0f : 3.14159265f),
		velAngular(0.0f),
		vidaUtil(0)
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

	void Projetil::setHoming(sf::Vector2f alvoInicial, float velAngularRad, int frames)
	{
		guiado = true;
		posAlvo = alvoInicial;
		velAngular = velAngularRad;
		vidaUtil = frames;
	}

	void Projetil::atualizarAlvo(sf::Vector2f pos)
	{
		posAlvo = pos;
	}

	bool Projetil::isGuiado() const
	{
		return guiado;
	}

	void Projetil::atualizar()
	{
		int animacao = 0;

		if (guiado && vidaUtil > 0 && !colidiu)
		{
			// Calcula o angulo desejado em direcao ao alvo
			const sf::Vector2f minha = corpo.getPosition();
			const sf::Vector2f delta = posAlvo - minha;
			const float anguloAlvo = std::atan2(delta.y, delta.x);

			// Rotacao lenta: diferenca de angulo limitada por velAngular
			float diff = anguloAlvo - angulo;
			// Normaliza para [-pi, pi]
			while (diff > 3.14159265f)  diff -= 2.0f * 3.14159265f;
			while (diff < -3.14159265f) diff += 2.0f * 3.14159265f;

			if (diff > velAngular)  diff = velAngular;
			if (diff < -velAngular) diff = -velAngular;
			angulo += diff;

			lado = (std::cos(angulo) >= 0.0f) ? 1 : -1;
			corpo.move(vel * std::cos(angulo), vel * std::sin(angulo));
			distanciaPercorrida += vel;
			vidaUtil--;

			if (vidaUtil <= 0)
			{
				animacao = 1;
				vel = 0.0f;
			}
		}
		else if (!guiado)
		{
			// Modo normal: linha reta
			corpo.move(vel * lado, 0.0f);
			distanciaPercorrida += vel;

			if (distanciaPercorrida >= 1200.0f || colidiu)
			{
				animacao = 1;
				vel = 0.0f;
			}
		}
		else
		{
			// Homing expirado ou colidiu
			animacao = 1;
			vel = 0.0f;
		}

		atualizarAnimacao(animacao);
	}

	void Projetil::inicializaAnimacoes()
	{
		auto* recursos = Gerenciadores::Gerenciador_Recursos::getGerenciador();

		sprite.setOrigin(Vector2f((corpo.getSize().x / 3.0f), (corpo.getSize().y / 2.0f) + 20.0f));

		Animacao animacaoVoar;
		Animacao animacaoExplodir;
		const int largura = 64;

		// A animacao de voo tem cada quadro em um arquivo separado.
		for (int i = 1; i < 60; i++)
			animacaoVoar.adicionarTextura(
				recursos->getTextura("Assets/Fireball/" + std::to_string(i) + ".png"));

		// A explosao e um spritesheet horizontal.
		animacaoExplodir.fatiarSpritesheet(
			recursos->getTextura("Assets/Fireball/explosao.png"), largura, largura);

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

		animacaoAtual->aplicar(sprite, count);
		if (guiado)
		{
			// Homing: rotaciona o sprite conforme o angulo atual
			sprite.setScale(2.3f, 2.3f);
			sprite.setRotation(angulo * 180.0f / 3.14159265f);
			sprite.setPosition(corpo.getPosition().x, corpo.getPosition().y);
		}
		else
		{
			sprite.setScale(2.3f * lado, 2.3f);
			sprite.setRotation(0.0f);
			sprite.setPosition(corpo.getPosition().x - 80.0f * lado, corpo.getPosition().y);
		}

		desenharSprite();
	}

	const bool Projetil::getExplodiu() const
	{
		return explodiu;
	}

	const bool Projetil::getColidiu() const
	{
		return colidiu;
	}

	void Projetil::desenharSprite()
	{
		gerenciador_grafico->desenhaSprite(sprite);
	}

	void Projetil::limparArquivo(int save)
	{
		std::ofstream arquivo("Saves/save" + std::to_string(save) + "_projetil.txt", std::ofstream::out);

		if (arquivo.is_open())
		{
			arquivo.close();
		}
	}
	void Projetil::salvar(int save)
	{
		std::ofstream arquivo("Saves/save" + to_string(save) + "_projetil.txt", std::ios::app);

		if (arquivo.is_open())
		{
			arquivo << "Posicao: " << corpo.getPosition().x << " " << corpo.getPosition().y << "\n";

			arquivo.close();
		}
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
