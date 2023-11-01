#include "Ente.hpp"

Gerenciadores::Gerenciador_Grafico* Ente::pGG(Gerenciadores::Gerenciador_Grafico::getGerenciador());

Ente::Ente(sf::Vector2f tamanho) : id(0), corpo(tamanho)
{

}

Ente::~Ente()
{

}

const sf::RectangleShape* Ente::get_corpo() const
{
	return &corpo;
}

void Ente::desenhar()
{
	pGG->desenhaElemento(corpo);
}
