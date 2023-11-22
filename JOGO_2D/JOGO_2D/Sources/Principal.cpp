#include "../Headers/Principal.hpp"

Principal::Principal() : gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()), 
fase1(),
fase2()
{
	fase2.instanciaEntidades("Fases/fase2-1p.txt");
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
		fase2.atualizaCamera();
		fase2.AtualizarPersonagens();
		gerenciador_colisoes->Executar();
		gerenciador_grafico->mostraElemento();

	}
}

