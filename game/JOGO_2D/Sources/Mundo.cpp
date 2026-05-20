#include "Mundo.hpp"
#include <algorithm>
#include <fstream>

Mundo::Mundo() :
	pontuacao{ 0, 0 },
	chefaoMorreu(false),
	posChefao(0.0f, 0.0f),
	faseAtual(1),
	kills(0),
	vidaPersistida{ -1.0f, -1.0f },
	camera(nullptr)
{
}

void Mundo::setCamera(Sistemas::Camera* c)
{
	camera = c;
}

Sistemas::Camera* Mundo::getCamera() const
{
	return camera;
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

int Mundo::getFaseAtual() const
{
	return faseAtual;
}

void Mundo::setFaseAtual(int f)
{
	faseAtual = f;
}

void Mundo::avancarFase()
{
	faseAtual++;
	// Bonus de pontuacao por terminar uma fase.
	adicionarPontos(0, 30);
	arvore.adicionarPontos(1);
}

int Mundo::getKills() const
{
	return kills;
}

void Mundo::setKills(int k)
{
	kills = k;
}

void Mundo::registrarKill(bool ehChefao)
{
	kills++;
	if (ehChefao)
	{
		adicionarPontos(0, 150);
		arvore.adicionarPontos(4);
	}
	else
	{
		adicionarPontos(0, 8);
		// 1 ponto de arvore a cada 3 kills (progressao mais gradual)
		if (kills % 3 == 0)
			arvore.adicionarPontos(1);
	}
}

ArvoreHabilidades& Mundo::getArvore()
{
	return arvore;
}

const ArvoreHabilidades& Mundo::getArvore() const
{
	return arvore;
}

void Mundo::reiniciarRun()
{
	faseAtual = 1;
	kills = 0;
	zerarPontuacao();
	chefaoMorreu = false;
	posChefao = sf::Vector2f();
	limparVidaPersistida();
}

void Mundo::setVidaPersistida(int indiceJogador, float vida)
{
	if (indiceJogador >= 0 && indiceJogador < 2)
		vidaPersistida[indiceJogador] = vida;
}

float Mundo::getVidaPersistida(int indiceJogador) const
{
	if (indiceJogador < 0 || indiceJogador >= 2)
		return -1.0f;
	return vidaPersistida[indiceJogador];
}

void Mundo::limparVidaPersistida()
{
	vidaPersistida[0] = -1.0f;
	vidaPersistida[1] = -1.0f;
}

void Mundo::enfileirarInimigo(Entidades::Personagens::Inimigo* inim)
{
	if (inim)
		filaInimigos.push_back(inim);
}

std::vector<Entidades::Personagens::Inimigo*>& Mundo::getFilaInimigos()
{
	return filaInimigos;
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
