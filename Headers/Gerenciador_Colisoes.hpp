#pragma once
#include "Inimigo.hpp"
#include "Jogador.hpp"
#include "Plataforma.hpp"
#include <vector>

namespace Gerenciadores
{
	class Gerenciador_Colisoes
	{
	private:
		static Gerenciador_Colisoes* pGerenciador;
		Gerenciador_Colisoes();
		Entidades::Personagens::Jogador* pJogador;
		std::vector<Entidades::Personagens::Inimigo*> inimigos;
		std::vector<Entidades::Obstaculos::Plataforma*> plataformas;

	public:
		~Gerenciador_Colisoes();
		static Gerenciador_Colisoes* getGerenciador();
		void Executar();
		void setJogador(Entidades::Personagens::Jogador* jogador);
		void removeJogador();
		void addInimigo(Entidades::Personagens::Inimigo* inimigo);
		void removeInimigo(int index);
		void addPlataforma(Entidades::Obstaculos::Plataforma* plataforma);
		void verificaColisaoJogador(Entidades::Personagens::Personagem* corpo, Entidades::Obstaculos::Plataforma* plataforma);
		void verificaColisaoInimigo(Entidades::Personagens::Personagem* corpo, Entidades::Obstaculos::Plataforma* plataforma);

	};
}



