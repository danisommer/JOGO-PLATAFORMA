#pragma once
#include "Inimigo.hpp"
#include "Jogador.hpp"
#include "Plataforma.hpp"
#include "Parede.hpp"
#include "Obstaculo.hpp"
#include <vector>
#include <iostream>

namespace Gerenciadores
{
	class Gerenciador_Colisoes
	{
	private:
		static Gerenciador_Colisoes* pGerenciador;
		Gerenciador_Colisoes();
		Entidades::Personagens::Jogador* pJogador;
		std::vector<Entidades::Personagens::Inimigo*> inimigos;
		std::vector<sf::RectangleShape*> corpos;

		std::vector<Entidades::Obstaculos::Obstaculo*> obstaculos;

	public:
		~Gerenciador_Colisoes();
		static Gerenciador_Colisoes* getGerenciador();
		void Executar();
		void setJogador(Entidades::Personagens::Jogador* jogador);
		void removeJogador();
		void addInimigo(Entidades::Personagens::Inimigo* inimigo);
		void removeInimigo(int index);
		void addPlataforma(Entidades::Obstaculos::Plataforma* plataforma);
		void addParede(Entidades::Obstaculos::Parede* parede);

		void addCorpo(Entidades::Obstaculos::Obstaculo* obstaculo);

		void addObstaculo(Entidades::Obstaculos::Obstaculo* obstaculo);
		void verificaColisaoJogador(Entidades::Personagens::Personagem* corpo, RectangleShape* plataforma);
		void verificaColisaoInimigo(Entidades::Personagens::Personagem* corpo, RectangleShape* plataforma);
	};
}



