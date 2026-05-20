#pragma once

#include <array>
#include <string>

// Skill tree achatada (sem pre-requisitos). Cada habilidade tem custo
// fixo em pontos, uma descricao, e um estado bool de desbloqueio.
// Os efeitos sao aplicados em tempo de spawn pelo Jogador (consultando
// foiDesbloqueada).
class ArvoreHabilidades
{
public:
	enum Habilidade
	{
		VIDA_EXTRA = 0,
		DANO_EXTRA = 1,
		VELOCIDADE = 2,
		PULO_FORTE = 3,
		ALCANCE_EXTRA = 4,
		VAMPIRO = 5,
		PULO_DUPLO = 6,
		ARMADURA = 7,
		N_HABILIDADES = 8
	};

	struct Info
	{
		const char* nome;
		const char* descricao;
		int custo;
	};

	ArvoreHabilidades();

	// Pontos disponiveis para gastar na arvore. Crescem com kills e
	// fim de fase, consumidos ao desbloquear habilidades.
	int getPontos() const;
	void adicionarPontos(int p);
	void setPontos(int p);

	bool foiDesbloqueada(Habilidade h) const;
	void desbloquear(Habilidade h);
	void setDesbloqueada(Habilidade h, bool v);

	// Tenta gastar os pontos e marca a habilidade como desbloqueada.
	// Retorna true se a compra foi efetivada.
	bool comprar(Habilidade h);

	static const Info& getInfo(Habilidade h);

private:
	int pontos;
	std::array<bool, N_HABILIDADES> desbloqueadas;
};
