#include "../Headers/Ente.hpp"

Gerenciadores::Gerenciador_Grafico* Ente::gerenciador_grafico = Gerenciadores::Gerenciador_Grafico::getGerenciador();

Ente::Ente():
	id()
{

}

Ente::~Ente()
{

}

int Ente::getId()
{
	return id;
}