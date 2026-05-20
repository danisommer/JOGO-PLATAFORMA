#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>
#include "ArvoreHabilidades.hpp"

namespace Entidades { namespace Personagens { class Jogador; } }

// Estado compartilhado de uma sessao de jogo. Substitui os membros
// 'static' que antes viviam espalhados em Inimigo, Obstaculo, Fase e
// Jogador (ponteiros para os jogadores, pontuacao, estado do chefao).
//
// Um Mundo e injetado em cada entidade pela Fase (Entidade::setMundo),
// eliminando o acoplamento por estado global: agora e possivel ter mais
// de uma sessao e o comportamento nao depende mais da ordem de criacao.
//
// Com a conversao para roguelike, o Mundo passou a guardar o numero
// da fase atual, a contagem de kills e a arvore de habilidades, que
// persistem entre fases dentro de uma run.
class Mundo
{
private:
	// Slots fixos: jogadores[i] vira nullptr quando o jogador i morre.
	std::vector<Entidades::Personagens::Jogador*> jogadores;

	int pontuacao[2];
	bool chefaoMorreu;
	sf::Vector2f posChefao;

	int faseAtual;
	int kills;
	ArvoreHabilidades arvore;

public:
	Mundo();

	// --- Jogadores -------------------------------------------------------
	void definirJogadores(const std::vector<Entidades::Personagens::Jogador*>& js);
	void limparJogadores();
	void removerJogador(Entidades::Personagens::Jogador* j);

	int getNumSlots() const;
	// Indice 0-based; devolve nullptr se fora do range ou se o jogador morreu.
	Entidades::Personagens::Jogador* getJogador(int indice) const;

	// --- Pontuacao -------------------------------------------------------
	void zerarPontuacao();
	void adicionarPontos(int indiceJogador, int pontos);
	int getPontuacao(int indiceJogador) const;

	// --- Chefao ----------------------------------------------------------
	void setChefaoMorreu(bool morreu, sf::Vector2f pos);
	bool getChefaoMorreu() const;
	sf::Vector2f getPosChefao() const;

	// --- Roguelike: progresso da run ------------------------------------
	int getFaseAtual() const;
	void setFaseAtual(int f);
	void avancarFase();

	int getKills() const;
	void setKills(int k);
	// Chamado quando um inimigo e abatido: incrementa kills e da
	// pontos para a arvore. Boss conta como kill especial (extra).
	void registrarKill(bool ehChefao);

	ArvoreHabilidades& getArvore();
	const ArvoreHabilidades& getArvore() const;

	// Reinicia o estado da run (fase, kills, pontuacao); a arvore de
	// habilidades e seus pontos NAO sao zerados aqui - sobrevivem
	// entre runs como meta-progressao.
	void reiniciarRun();

	// Acrescenta as pontuacoes nao nulas ao arquivo de ranking.
	void gravarRanking(const std::string& nome1, const std::string& nome2) const;
};
