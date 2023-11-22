#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <functional>
#include <cstdlib>
#include <ctime>   
#include "Ente.hpp"
#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Colisoes.hpp"
#include "Gerenciador_Eventos.hpp"
#include "Jogador.hpp"
#include "Inimigo.hpp"
#include "Plataforma.hpp"
#include "Personagem.hpp"
#include "OlhoVoador.hpp"
#include "Cogumelo.hpp"
#include "Chefao.hpp"
#include "Parede.hpp"
#include "Serra.hpp"
#include "Espinho.hpp"
#include "Slime.hpp"
#include "ListaEntidade.hpp"


using namespace sf;
using namespace std;
using namespace Entidades::Personagens;
using namespace Entidades::Obstaculos;

namespace Fases
{
	class Fase : public Ente
	{
	protected:
		Gerenciadores::Gerenciador_Grafico* gerenciador_grafico;
		Gerenciadores::Gerenciador_Eventos* gerenciador_eventos;
		Gerenciadores::Gerenciador_Colisoes* gerenciador_colisoes;

		Lista::ListaEntidade listaPersonagem;
		Lista::ListaEntidade listaObstaculo;
		Entidades::Personagens::Jogador* jogador;
		Entidades::Personagens::Jogador* jogador2;

		sf::Texture texturaFundo;

		bool jogadorCriado;
		int numJogadores;

		int fase;

	public:
		Fase();
		~Fase();
		void instanciaEntidades(char character, Vector2f pos);
		//void desalocaEntidades();
		bool Aleatorizar(char character);
		void AtualizarPersonagens();
		void atualizaCamera();
		int getFase();
		void criarJogador(const Vector2f pos);
		void criarOlhoVoador(const Vector2f pos);
		void criarChefao(const Vector2f pos);
		void criarCogumelo(const Vector2f pos);
		void criarPlataformaPequena(const Vector2f pos);
		void criarPlataformaGrande(const Vector2f pos);
		void criarParede(const Vector2f pos);
		void criarSerra(const Vector2f pos);
		void criarEspinho(const Vector2f pos);
		void criarSlime(const Vector2f pos);
		virtual void criarMapa() = 0;
		void desenhaElementos();
		void executaLista();
	};
}