#pragma once

#include <array>
#include <string>

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
		int custoBase;   // custo do nivel 1
		int custoNivel;  // acrescimo por nivel adicional
		int maxNivel;    // nivel maximo (1 = so desbloquear)
	};

	ArvoreHabilidades();

	int getPontos() const;
	void adicionarPontos(int p);
	void setPontos(int p);

	// Retorna o nivel atual da habilidade (0 = nao desbloqueada).
	int getNivel(Habilidade h) const;
	// Alias legivel: true se nivel >= 1.
	bool foiDesbloqueada(Habilidade h) const;
	void setNivel(Habilidade h, int v);

	// Custo para adquirir o proximo nivel. Retorna -1 se ja no maximo.
	int custoProximoNivel(Habilidade h) const;

	// Tenta comprar o proximo nivel. Retorna true se efetivado.
	bool comprar(Habilidade h);

	static const Info& getInfo(Habilidade h);

private:
	int pontos;
	std::array<int, N_HABILIDADES> niveis;
};
