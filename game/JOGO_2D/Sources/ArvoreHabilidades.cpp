#include "ArvoreHabilidades.hpp"

namespace
{
	// custoBase = custo do nivel 1
	// custoNivel = acrescimo por nivel adicional (nivel 2 custa base + 1*extra, nivel 3 = base + 2*extra...)
	// maxNivel = teto de upgrades
	const ArvoreHabilidades::Info TABELA[ArvoreHabilidades::N_HABILIDADES] = {
		{ "Vida +25",      "Aumenta a vida maxima em 25 por nivel",        1, 2, 3 },
		{ "Dano +5",       "Aumenta o dano dos ataques em 5 por nivel",     1, 2, 3 },
		{ "Velocidade",    "Move-se mais rapido (2 px/nivel)",              2, 2, 3 },
		{ "Pulo Forte",    "Pula com mais forca (25% por nivel)",           2, 3, 2 },
		{ "Alcance",       "Alcance dos ataques +15% por nivel",            2, 2, 3 },
		{ "Vampiro",       "Recupera 5 HP por kill (acumula por nivel)",    3, 3, 3 },
		{ "Pulo Duplo",    "Permite pular uma vez no ar",                   3, 0, 1 },
		{ "Armadura",      "Recebe 12.5% menos dano por nivel",             3, 3, 3 },
	};
}

ArvoreHabilidades::ArvoreHabilidades() :
	pontos(0)
{
	niveis.fill(0);
}

int ArvoreHabilidades::getPontos() const { return pontos; }
void ArvoreHabilidades::adicionarPontos(int p) { pontos += p; }
void ArvoreHabilidades::setPontos(int p) { pontos = p; }

int ArvoreHabilidades::getNivel(Habilidade h) const
{
	if (h < 0 || h >= N_HABILIDADES) return 0;
	return niveis[h];
}

bool ArvoreHabilidades::foiDesbloqueada(Habilidade h) const
{
	return getNivel(h) >= 1;
}

void ArvoreHabilidades::setNivel(Habilidade h, int v)
{
	if (h >= 0 && h < N_HABILIDADES)
		niveis[h] = v;
}

int ArvoreHabilidades::custoProximoNivel(Habilidade h) const
{
	if (h < 0 || h >= N_HABILIDADES) return -1;
	const int atual = niveis[h];
	if (atual >= TABELA[h].maxNivel) return -1;
	return TABELA[h].custoBase + atual * TABELA[h].custoNivel;
}

bool ArvoreHabilidades::comprar(Habilidade h)
{
	const int custo = custoProximoNivel(h);
	if (custo < 0 || pontos < custo)
		return false;

	pontos -= custo;
	niveis[h]++;
	return true;
}

const ArvoreHabilidades::Info& ArvoreHabilidades::getInfo(Habilidade h)
{
	if (h < 0 || h >= N_HABILIDADES)
		return TABELA[0];
	return TABELA[h];
}
