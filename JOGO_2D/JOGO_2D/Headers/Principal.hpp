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
#include "FlyingEye.hpp"
#include "Mushroom.hpp"
#include "Chefao.hpp"
#include "Parede.hpp"
#include "Serra.hpp"
#include "Espinho.hpp"
#include "Slime.hpp"
#include "ListaEntidade.hpp"

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

	Entidades::Personagens::Jogador* jogador;
	Lista::ListaEntidade listaPersonagem;
	Lista::ListaEntidade listaObstaculo;

public:
	Principal();
	~Principal();
	void executar();
	void instanciaEntidades(const std::string& arquivoTxt);
	void AtualizarPersonagens();
	void DesenharElementos();
	bool Aleatorizar(char character);

};