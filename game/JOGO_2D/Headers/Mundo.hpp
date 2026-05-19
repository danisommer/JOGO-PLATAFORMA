#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

namespace Entidades { namespace Personagens { class Jogador; } }

// Estado compartilhado de uma sessao de jogo. Substitui os membros
// 'static' que antes viviam espalhados em Inimigo, Obstaculo, Fase e
// Jogador (ponteiros para os jogadores, pontuacao, estado do chefao).
//
// Um Mundo e injetado em cada entidade pela Fase (Entidade::setMundo),
// eliminando o acoplamento por estado global: agora e possivel ter mais
// de uma sessao e o comportamento nao depende mais da ordem de criacao.
class Mundo
{
private:
	// Slots fixos: jogadores[i] vira nullptr quando o jogador i morre.
	std::vector<Entidades::Personagens::Jogador*> jogadores;

	int pontuacao[2];
	bool chefaoMorreu;
	sf::Vector2f posChefao;

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

	// Acrescenta as pontuacoes nao nulas ao arquivo de ranking.
	void gravarRanking(const std::string& nome1, const std::string& nome2) const;
};
