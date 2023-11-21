#include "../Headers/Principal.hpp"

Principal::Principal() : gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()), 
fase1()
{
}

Principal::~Principal()
{
	
}

void Principal::executar()
{
	while (gerenciador_grafico->getOpen())
	{
		gerenciador_eventos->Executar();
		gerenciador_grafico->limpaTela();
		gerenciador_grafico->atualizaCamera();
		fase1.AtualizarPersonagens();
		gerenciador_colisoes->Executar();
		fase1.DesenharElementos();
	}
}

