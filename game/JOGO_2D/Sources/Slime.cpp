#include "Slime.hpp"
#include "Gerenciador_Recursos.hpp"
#include "Mundo.hpp"

#include <iostream>

namespace Entidades
{
	namespace Obstaculos
	{
		Slime::Slime(const sf::Vector2f pos, const sf::Vector2f tam) :
			Obstaculo(pos, tam),
			pegajosidadePulo(-10.5f),
			pegajosidadeMovimento(2.0f)
		{
			danoso = true;
			corpo = sf::RectangleShape(tam);
			corpo.setPosition(pos);
			corpo.setFillColor(Color::Red);


			sprite.setTexture(Gerenciadores::Gerenciador_Recursos::getGerenciador()
				->getTextura("Assets/Cenario/Trap/Slime.png"));
			sprite.setPosition(Vector2f(pos.x - 102.0f, pos.y - 75.0f));
			sprite.setScale(0.5f, 0.31f);


		}
		Slime::~Slime()
		{
		}

		void Slime::atualizar()
		{
			desenharSprite();
		}

		void Slime::danar(int jogador)
		{
			// O efeito so dura enquanto o jogador continua em contato:
			// usamos uma duracao curtissima (3 passos) que e renovada
			// a cada frame de colisao. Ao sair, o efeito expira em ~3
			// passos, sem o antigo arrasto de varios segundos.
			Entidades::Personagens::Jogador* alvo =
				mundo ? mundo->getJogador(jogador - 1) : nullptr;
			if (alvo)
				alvo->setLento(true, 3, pegajosidadeMovimento, pegajosidadePulo);
		}
		void Slime::limparArquivo(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_slime.txt", std::ofstream::out);

			if (arquivo.is_open())
			{
				arquivo.close();
			}
		}
		void Slime::salvar(int save)
		{
			std::ofstream arquivo("Saves/save" + to_string(save) + "_slime.txt", std::ios::app);

			if (arquivo.is_open())
			{
				arquivo << "Posicao: " << corpo.getPosition().x << " " << corpo.getPosition().y << "\n";

				arquivo.close();
			}
		}
	}
}