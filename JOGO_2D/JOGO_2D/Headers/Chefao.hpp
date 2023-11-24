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
		private:
			void inicializaAnimacoes();
			std::vector<Entidades::Projetil*> projeteis;
			std::vector<Entidades::Obstaculos::Portal*> portais;
			std::chrono::steady_clock::time_point ultimoAtaque;
			int delayAtaque;
			bool bravo;
			bool muitoBravo;
			bool enfurecido;
			bool morrendo;
			Animacao animacaoTeleporte;

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
