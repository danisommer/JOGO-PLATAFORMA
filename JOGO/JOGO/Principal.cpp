#include "Principal.hpp"

Principal::Principal() :
	gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
	gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador())
{
	instanciaEntidades();
}

Principal::~Principal()
{
	for (size_t i = 0; i < personagens.size(); i++)
	{
		delete personagens[i];
	}

	for (size_t i = 0; i < plataformas.size(); i++)
	{
		delete plataformas[i];
	}

	personagens.clear();
	plataformas.clear();
}

void Principal::Executar()
{
	sf::Event evento;

	while (gerenciador_grafico->getOpen())
	{
		while (gerenciador_grafico->getJanela()->pollEvent(evento))
		{
			if (evento.type == sf::Event::Closed)
			{
				gerenciador_grafico->fecharJanela();
			}
		}

		gerenciador_grafico->limpaTela();

		gerenciador_grafico->atualizaCamera();

		gerenciador_colisoes->Executar();

		AtualizarPersonagens();

		DesenharElementos();		
	}
}

void Principal::instanciaEntidades()
{
	jogador = new Entidades::Personagens::Jogador(Vector2f(200.0f, 200.0f), Vector2f(40.0f, 80.0f)); //pos tam
	Entidades::Personagens::Inimigo* inimigo = new Entidades::Personagens::Inimigo(Vector2f(550.0f, 500.0f), Vector2f(50.0f, 80.0f), jogador); //pos tam pJogador
	Entidades::Personagens::Inimigo* inimigo2 = new Entidades::Personagens::Inimigo(Vector2f(900.0f, 500.0f), Vector2f(50.0f, 80.0f), jogador); //pos tam pJogador
	Entidades::Obstaculos::Plataforma* platform = new Entidades::Obstaculos::Plataforma(Vector2f(-10000.0f, 950.0f), Vector2f(30000.0f, 120.0f));
	Entidades::Obstaculos::Plataforma* platform1 = new Entidades::Obstaculos::Plataforma(Vector2f(100.0f, 800.0f), Vector2f(300.0f, 30.0f));
	Entidades::Obstaculos::Plataforma* platform2 = new Entidades::Obstaculos::Plataforma(Vector2f(400.0f, 600.0f), Vector2f(300.0f, 30.0f));

	Entidades::Personagens::Personagem* p1 = static_cast<Entidades::Personagens::Personagem*>(jogador);
	Entidades::Personagens::Personagem* p2 = static_cast<Entidades::Personagens::Personagem*>(inimigo);
	Entidades::Personagens::Personagem* p3 = static_cast<Entidades::Personagens::Personagem*>(inimigo2);

	gerenciador_grafico->setJogador(jogador);

	gerenciador_colisoes->setJogador(jogador);
	gerenciador_colisoes->addInimigo(inimigo);
	gerenciador_colisoes->addInimigo(inimigo2);
	gerenciador_colisoes->addPlataforma(platform);
	gerenciador_colisoes->addPlataforma(platform1);
	gerenciador_colisoes->addPlataforma(platform2);

	plataformas.push_back(platform2);
	plataformas.push_back(platform1);
	plataformas.push_back(platform);

	personagens.push_back(p1);
	personagens.push_back(p2);
	personagens.push_back(p3);

}

void Principal::AtualizarPersonagens()
{
	for (int i = 0; i < personagens.size(); i++)
	{
		personagens.at(i)->atualizar();
		personagens.at(i)->cair();
	}
}

void Principal::DesenharElementos()
{
	gerenciador_grafico->desenhaSprite(jogador->getSprite());

	for (int i = 0; i < personagens.size(); i++)
	{
		gerenciador_grafico->desenhaElemento(personagens.at(i)->getCorpo());
	}
	for (int i = 0; i < plataformas.size(); i++)
	{
		gerenciador_grafico->desenhaElemento(plataformas.at(i)->getCorpo());
	}

	gerenciador_grafico->mostraElemento();
}
