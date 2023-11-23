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

	Tela telaPausa;
	sf::Text tituloPausa;
	sf::Sprite* fundoPausa;
	Texture* imagemFundo;

	bool derrota;
	bool concluida;
	bool salvar;

public:
	Principal();
	~Principal();
	void executar();
	void executarFase1(int n_jogadores);
	void executarFase2(int n_jogadores);
	int exibirMenuPausa();
	void inicializaMenu();
	bool getConcluida();
};