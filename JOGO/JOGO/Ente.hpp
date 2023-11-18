#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <functional>
#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Colisoes.hpp"
#include "Gerenciador_Eventos.hpp"
#include "Jogador.hpp"
#include "Inimigo.hpp"
#include "Plataforma.hpp"
#include "Parede.hpp"
#include "Personagem.hpp"
#include "FlyingEye.hpp"
#include "Mushroom.hpp"
#include "Chefao.hpp"
#include "Serra.hpp"
#include <chrono>
#include <thread>


using namespace sf;
using namespace std;
using namespace Entidades::Personagens;
using namespace Entidades::Obstaculos;

namespace Gerenciadores
{
	class Gerenciador_Grafico;
	class Gerenciador_Colisoes;
	class Gerenciador_Eventos;
}

class Ente
{
private:
	Gerenciadores::Gerenciador_Grafico* gerenciador_grafico;
	Gerenciadores::Gerenciador_Colisoes* gerenciador_colisoes;
	Gerenciadores::Gerenciador_Eventos* gerenciador_eventos;

	Entidades::Personagens::Jogador* jogador;
	vector<Entidades::Personagens::Personagem*> personagens;
	vector<Entidades::Obstaculos::Plataforma*> plataformas;
	vector<Entidades::Obstaculos::Obstaculo*> obstaculos;
	vector<Entidades::Personagens::Inimigo*> inimigos;

public:
	Ente();
	~Ente();
	void Executar();
	void instanciaEntidades(const std::string& arquivoTxt);
	void AtualizarPersonagens();
	void DesenharElementos();
};

