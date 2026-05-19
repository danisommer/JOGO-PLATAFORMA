#pragma once

#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Colisoes.hpp"
#include "Gerenciador_Eventos.hpp"
#include "Mundo.hpp"
#include "Ruinas.hpp"
#include "Floresta.hpp"
#include "Tela.hpp"

using namespace sf;
using namespace std;
using namespace Entidades::Personagens;
using namespace Entidades::Obstaculos;

class Principal
{
private:
	Gerenciadores::Gerenciador_Grafico* gerenciador_grafico;
	Gerenciadores::Gerenciador_Eventos* gerenciador_eventos;
	Gerenciadores::Gerenciador_Colisoes* gerenciador_colisoes;

	Fases::Floresta fase1;
	Fases::Ruinas fase2;

	// Estado de sessao compartilhado por ambas as fases (pontuacao
	// acumulada, jogadores, chefao). Injetado nas fases no construtor.
	Mundo mundo;

	const sf::Font* fonte;

	sf::Text textoCarregamento;

	Tela telaPausa;
	Tela telaMundos;

public:
	Principal();
	~Principal();
	void alocaFase1(int n_jogadores);
	void alocaFase2(int n_jogadores);
	void recuperaFase(int save);

	// Roda uma fase atraves da maquina de estados (EstadoJogo/EstadoPausa).
	void executarFase(int fase, int n_jogadores);

	// Usados pelos estados de jogo/pausa.
	Fases::Fase* prepararFase(int numFase, int n_jogadores);
	void telaCarregamento();
	Tela& getTelaPausa();
	Tela& getTelaMundos();
	Mundo& getMundo();

	void inicializaMenu();
	void inicializaMundos();
};