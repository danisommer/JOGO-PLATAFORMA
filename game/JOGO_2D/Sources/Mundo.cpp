#include "Mundo.hpp"
#include <algorithm>
#include <fstream>

Mundo::Mundo() :
	pontuacao{ 0, 0 },
	chefaoMorreu(false),
	posChefao(0.0f, 0.0f)
{
}

void Mundo::definirJogadores(const std::vector<Entidades::Personagens::Jogador*>& js)
{
	jogadores = js;
}

void Mundo::limparJogadores()
{
	jogadores.clear();
}

void Mundo::removerJogador(Entidades::Personagens::Jogador* j)
{
	// Mantem o slot (indice estavel), apenas marca como ausente.
	for (auto& slot : jogadores)
		if (slot == j)
			slot = nullptr;
}

int Mundo::getNumSlots() const
{
	return static_cast<int>(jogadores.size());
}

Entidades::Personagens::Jogador* Mundo::getJogador(int indice) const
{
	if (indice < 0 || indice >= static_cast<int>(jogadores.size()))
		return nullptr;

	return jogadores[indice];
}

void Mundo::zerarPontuacao()
{
	pontuacao[0] = 0;
	pontuacao[1] = 0;
}

void Mundo::adicionarPontos(int indiceJogador, int pontos)
{
	if (indiceJogador >= 0 && indiceJogador < 2)
		pontuacao[indiceJogador] += pontos;
}

int Mundo::getPontuacao(int indiceJogador) const
{
	if (indiceJogador >= 0 && indiceJogador < 2)
		return pontuacao[indiceJogador];

	return 0;
}

void Mundo::setChefaoMorreu(bool morreu, sf::Vector2f pos)
{
	chefaoMorreu = morreu;
	posChefao = pos;
}

bool Mundo::getChefaoMorreu() const
{
	return chefaoMorreu;
}

sf::Vector2f Mundo::getPosChefao() const
{
	return posChefao;
}

void Mundo::gravarRanking(const std::string& nome1, const std::string& nome2) const
{
	std::ofstream arquivo("Saves/ranking.txt", std::ios::app);
	if (!arquivo.is_open())
		return;

	if (pontuacao[0] != 0)
		arquivo << pontuacao[0] << "-" << nome1 << "\n";
	if (pontuacao[1] != 0)
		arquivo << pontuacao[1] << "-" << nome2 << "\n";
}
