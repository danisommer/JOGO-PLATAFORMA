#include "../Headers/Principal.hpp"

Principal::Principal() : fase1(), fase2()
{
	//fase2.criarMapa();
	//gerenciador_grafico->setFase(fase2.getFase());
}

Principal::~Principal()
{

}

void Principal::executar()
{
	fase2.executar();
}

