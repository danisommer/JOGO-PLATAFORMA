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
		static int pontuacaoP1;
		static int pontuacaoP2;

		static string nomeP1;
		static string nomeP2;
		Gerenciadores::Gerenciador_Grafico* gerenciador_grafico;
		Gerenciadores::Gerenciador_Eventos* gerenciador_eventos;
		Gerenciadores::Gerenciador_Colisoes* gerenciador_colisoes;

		Lista::ListaEntidade listaPersonagem;
		Lista::ListaEntidade listaObstaculo;
		Entidades::Personagens::Jogador* jogador;
		Entidades::Personagens::Jogador* jogador2;

		int numCogumelosCriados;
		int numVoadoresCriados;
		int numSerrasCriadas;
		int numEspinhosCriados;
		int numSlimesCriados;

		sf::Texture texturaFundo;

		bool jogadorCriado;
		int numJogadores;

		bool concluida;
		bool derrota;

		int fase;
		bool morreu;

	public:
		Fase();
		~Fase();
		void instanciaEntidades(const std::string& arquivoTxt);
		void desalocaEntidades();
		bool Aleatorizar(char character);
		void AtualizarPersonagens();
		void atualizaCamera();
		int getFase();
		bool getConcluida();
		bool getDerrota();
		void setDerrota(bool d);
		void verificaFase();

		void limparArquivo(int save);
		void salvarJogo(int save);
		void recuperarJogo(int save, int fase, int n_jogadores);

		static void gravarPontuacao(const std::string& nomeJogador, const std::string& nomeJogador2);
		static void ZeraPontuacao();
	};
}