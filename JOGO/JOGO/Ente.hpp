#pragma once

#include <iostream>
#include <vector>
#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Colisoes.hpp"
#include "Gerenciador_Eventos.hpp"
#include "Jogador.hpp"
#include "Inimigo.hpp"
#include "Plataforma.hpp"
#include "Personagem.hpp"
#include "FlyingEye.hpp"
#include "Skeleton.hpp"
#include "Goblin.hpp"
#include "Mushroom.hpp"

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

public:
	Ente();
	~Ente();
	void Executar();
	void instanciaEntidades();
	void AtualizarPersonagens();
	void DesenharElementos();
	//std::vector<std::string> lerArquivoMapa(const std::string& nomeArquivo);
	//void criarEntidades(const std::vector<std::string>& mapa);
	//void instanciaEntidadesAPartirdoMapa(const std::string& nomeArquivo);
};

