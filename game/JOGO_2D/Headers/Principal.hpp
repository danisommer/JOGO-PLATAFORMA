#pragma once

#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Colisoes.hpp"
#include "Gerenciador_Eventos.hpp"
#include "Mundo.hpp"
#include "Fase.hpp"
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

	// Modo roguelike: uma unica Fase reciclada a cada nova fase
	// procedural. As classes Floresta/Ruinas viraram apenas temas
	// visuais aplicados via Fase::setFase.
	Fases::Fase fase;

	// Estado de sessao compartilhado pelas fases (pontuacao
	// acumulada, jogadores, chefao, fase atual, kills, arvore).
	Mundo mundo;

	const sf::Font* fonte;

	sf::Text textoCarregamento;

	Tela telaPausa;
	Tela telaMundos;

public:
	Principal();
	~Principal();
	void recuperaFase(int save);

	// Gera (ou regenera) a fase atual a partir do numero armazenado
	// em Mundo. Usado para iniciar uma nova run e para avancar entre
	// fases dentro de uma mesma run.
	Fases::Fase* prepararFase(int numFase, int n_jogadores);
	void telaCarregamento();

	// Inicia a sessao do modo roguelike. Comeca da fase 1 (ou de uma
	// salva) e gera fases sucessivas enquanto o jogador sobrevive.
	void executarFase(int faseInicial, int n_jogadores);

	Tela& getTelaPausa();
	Tela& getTelaMundos();
	Mundo& getMundo();

	void inicializaMenu();
	void inicializaMundos();
};
