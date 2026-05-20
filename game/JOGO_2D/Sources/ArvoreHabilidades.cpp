#include "ArvoreHabilidades.hpp"

namespace
{
	const ArvoreHabilidades::Info TABELA_HABILIDADES[ArvoreHabilidades::N_HABILIDADES] = {
		{ "Vida +25",       "Aumenta a vida maxima em 25",          1 },
		{ "Dano +5",        "Aumenta o dano dos ataques em 5",       1 },
		{ "Velocidade +2",  "Move-se mais rapido pelo cenario",      2 },
		{ "Pulo +25%",      "Pula com mais forca",                   2 },
		{ "Alcance +30%",   "Aumenta o alcance dos ataques",         2 },
		{ "Vampiro",        "Recupera 5 de vida ao matar inimigo",   3 },
		{ "Pulo duplo",     "Permite pular uma vez no ar",           3 },
		{ "Armadura",       "Recebe 25% menos dano",                 3 },
	};
}

ArvoreHabilidades::ArvoreHabilidades() :
	pontos(0)
{
	desbloqueadas.fill(false);
}

int ArvoreHabilidades::getPontos() const
{
	return pontos;
}

void ArvoreHabilidades::adicionarPontos(int p)
{
	pontos += p;
}

void ArvoreHabilidades::setPontos(int p)
{
	pontos = p;
}

bool ArvoreHabilidades::foiDesbloqueada(Habilidade h) const
{
	if (h < 0 || h >= N_HABILIDADES)
		return false;
	return desbloqueadas[h];
}

void ArvoreHabilidades::desbloquear(Habilidade h)
{
	if (h >= 0 && h < N_HABILIDADES)
		desbloqueadas[h] = true;
}

void ArvoreHabilidades::setDesbloqueada(Habilidade h, bool v)
{
	if (h >= 0 && h < N_HABILIDADES)
		desbloqueadas[h] = v;
}

bool ArvoreHabilidades::comprar(Habilidade h)
{
	if (h < 0 || h >= N_HABILIDADES)
		return false;
	if (desbloqueadas[h])
		return false;

	const int custo = TABELA_HABILIDADES[h].custo;
	if (pontos < custo)
		return false;

	pontos -= custo;
	desbloqueadas[h] = true;
	return true;
}

const ArvoreHabilidades::Info& ArvoreHabilidades::getInfo(Habilidade h)
{
	if (h < 0 || h >= N_HABILIDADES)
		return TABELA_HABILIDADES[0];
	return TABELA_HABILIDADES[h];
}
