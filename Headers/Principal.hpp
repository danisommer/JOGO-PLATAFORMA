#pragma once

#include <iostream>
#include <map>
#include <fstream>
#include <functional>
#include <cstdlib>
#include <ctime>   
#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Colisoes.hpp"
#include "Gerenciador_Eventos.hpp"
#include "Jogador.hpp"
#include "Inimigo.hpp"
#include "Plataforma.hpp"
#include "Personagem.hpp"
#include "OlhoVoador.hpp"
#include "Cogumelo.hpp"
#include "Chefao.hpp"
#include "Parede.hpp"
#include "Serra.hpp"
#include "Espinho.hpp"
#include "Slime.hpp"
#include "ListaEntidade.hpp"
#include "Ruinas.hpp"
#include "Floresta.hpp"

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

	Fases::Floresta fase1;
	Fases::Ruinas fase2;

public:
	Principal();
	~Principal();
	void executar();
};