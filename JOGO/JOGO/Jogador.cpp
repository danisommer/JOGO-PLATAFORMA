#include "Jogador.hpp"
#include <iostream>

namespace Entidades
{
	namespace Personagens
	{
		Jogador::Jogador(const Vector2f pos, const Vector2f tam) :
			aceleracao(1200.0f),
			desaceleracao(1500.0f),
			velocidadeMaxima(900.0f),
			jumpStrength(-0.175f)
		{
			corpo = RectangleShape(tam);
			corpo.setPosition(pos);
			corpo.setFillColor(sf::Color::Green);
			vel = Vector2f(0.80f, 0.80f);
		}

		Jogador::~Jogador() = default;

		void Jogador::atualizar()
		{
			dt = relogio.getElapsedTime().asSeconds();

			relogio.restart();

			sf::Vector2f direcao(0.0f, 0.0f);

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				direcao.x -= aceleracao * dt;
			}
			else if (Keyboard::isKeyPressed(Keyboard::D))
			{
				direcao.x += aceleracao * dt;
			}
			else
			{
				if (vel.x > 0)
				{
					direcao.x = -desaceleracao * dt;
				}
				else if (vel.x < 0)
				{
					direcao.x = desaceleracao * dt;
				}
			}

			vel.x += direcao.x;

			if (vel.x > velocidadeMaxima)
			{
				vel.x = velocidadeMaxima;
			}
			else if (vel.x < -velocidadeMaxima)
			{
				vel.x = -velocidadeMaxima;
			}

			// Pular
			if ((Keyboard::isKeyPressed(Keyboard::Space)) && !isJumping)
			{
				velocity.y = jumpStrength;
				isJumping = true;
			}

			ds = vel.x * dt;
			corpo.move(ds, direcao.y);

		}
	}


}

