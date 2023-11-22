#include "../Headers/Principal.hpp"

Principal::Principal() : gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
//fase1(),
fase2()
{
	
}

Principal::~Principal()
{

}

void Principal::executar()
{
	fase2.executaFase();
}
	
