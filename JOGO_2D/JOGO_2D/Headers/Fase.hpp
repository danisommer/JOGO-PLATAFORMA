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

		bool concluida;
		bool derrota;

		int fase;

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
	};
}