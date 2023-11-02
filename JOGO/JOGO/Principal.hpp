#pragma once

#include <iostream>
#include <vector>
#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Colisoes.hpp"
#include "Jogador.hpp"
#include "Inimigo.hpp"
#include "Plataforma.hpp"
#include "Personagem.hpp"

using namespace sf;
using namespace std;
using namespace Entidades::Personagens;
using namespace Entidades::Obstaculos;

namespace Gerenciadores
{
	class Gerenciador_Grafico;
	class Gerenciador_Colisoes;
}

class Principal
{
private:
	Gerenciadores::Gerenciador_Grafico* gerenciador_grafico;
	Gerenciadores::Gerenciador_Colisoes* gerenciador_colisoes;

	Entidades::Personagens::Jogador* jogador;
	vector<Entidades::Personagens::Personagem*> personagens;
	vector<Entidades::Obstaculos::Plataforma*> plataformas;

public:
	Principal();
	~Principal();
	void Executar();
	void instanciaEntidades();
	void AtualizarPersonagens();
	void DesenharElementos();

};

