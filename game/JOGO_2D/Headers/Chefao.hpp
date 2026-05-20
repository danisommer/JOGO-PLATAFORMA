#pragma once
#include "Inimigo.hpp"
#include "vector"
#include "Projetil.hpp"
#include "Portal.hpp"
#include <chrono>
#include <thread>
#include "Gerenciador_Grafico.hpp"

using namespace Entidades;

namespace Entidades
{
	namespace Personagens
	{
		class Chefao : public Inimigo
		{
		public:
			enum class Estagio { E1 = 1, E2, E3, E4 };

		private:
			void inicializaAnimacoes();
			std::vector<Entidades::Projetil*> projeteis;
			std::vector<Entidades::Obstaculos::Portal*> portais;
			std::chrono::steady_clock::time_point ultimoAtaque;
			std::chrono::steady_clock::time_point ultimoSlam;
			std::chrono::steady_clock::time_point ultimoSpawn;
			int delayAtaque;
			Estagio estagio;
			bool morrendo;

			// Slam: bater no chao para atordoar o jogador
			bool realizandoSlam;
			float posYOriginal;

			void dispararProjetil(bool guiado);
			void realizarSlam();
			void spawnVoadores();

		public:
			Chefao(Vector2f pos, Vector2f tam);
			~Chefao();
			void atacar(int jogador);
			void setAnimacao(int anim);
			float getVida();
			float getSize();
			void atualizaProjeteis();
			void atualizarPortais();
			void atualizaVida();
			void teleportar();
			void atualizar();

			void salvar(int save);
			void limparArquivo(int save);
		};

	}
}
