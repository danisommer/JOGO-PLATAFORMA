#pragma once

#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Colisoes.hpp"
#include "Gerenciador_Eventos.hpp"
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

	sf::Font* fonte;

	sf::Text textoCarregamento;

	Tela telaPausa;
	Tela telaMundos;

	bool derrota;
	bool concluida;
	bool salvar;
	bool carregar;

public:
	Principal();
	~Principal();
	void alocaFase1(int n_jogadores);
	void alocaFase2(int n_jogadores);
	void recuperaFase(int save);

	void executarFase(int fase, int n_jogadores);

	int exibirMenuPausa();
	int exibirMenuMundos();
	void inicializaMenu();
	void inicializaMundos();
	bool getConcluida();
	void telaCarregamento();
};