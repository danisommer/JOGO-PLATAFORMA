#include "Principal.hpp"

Principal::Principal() :
	gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
	gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
	gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador())
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
		gerenciador_eventos->Executar();

		gerenciador_grafico->limpaTela();

		gerenciador_grafico->atualizaCamera();

		gerenciador_colisoes->Executar();

		AtualizarPersonagens();

		DesenharElementos();		
	}
}

void Principal::instanciaEntidades()
{
	jogador = new Entidades::Personagens::Jogador(Vector2f(200.0f, 200.0f), Vector2f(30.0f, 80.0f)); //pos tam
	
	Entidades::Obstaculos::Plataforma* platform = new Entidades::Obstaculos::Plataforma(Vector2f(-10000.0f, 950.0f), Vector2f(30000.0f, 120.0f));
	Entidades::Obstaculos::Plataforma* platform1 = new Entidades::Obstaculos::Plataforma(Vector2f(100.0f, 800.0f), Vector2f(300.0f, 30.0f));
	Entidades::Obstaculos::Plataforma* platform2 = new Entidades::Obstaculos::Plataforma(Vector2f(400.0f, 600.0f), Vector2f(300.0f, 30.0f));
	Entidades::Personagens::FlyingEye* olhoVoador = new Entidades::Personagens::FlyingEye(Vector2f(550.0f, 500.0f), Vector2f(40.0f, 80.0f), jogador);
	Entidades::Personagens::Skeleton* esqueleto = new Entidades::Personagens::Skeleton(Vector2f(400.0f, 500.0f), Vector2f(40.0f, 80.0f), jogador);
	Entidades::Personagens::Goblin* goblin = new Entidades::Personagens::Goblin(Vector2f(300.0f, 500.0f), Vector2f(40.0f, 80.0f), jogador);
	Entidades::Personagens::Mushroom* cogumelo = new Entidades::Personagens::Mushroom(Vector2f(200.0f, 500.0f), Vector2f(40.0f, 80.0f), jogador);

	Entidades::Personagens::Personagem* p1 = static_cast<Entidades::Personagens::Personagem*>(jogador);
	//Entidades::Personagens::Inimigo* inimigo = static_cast<Entidades::Personagens::Inimigo*>(olhoVoador);
	//Entidades::Personagens::Inimigo* inimigo2 = static_cast<Entidades::Personagens::Inimigo*>(esqueleto);
	Entidades::Personagens::Personagem* p2 = static_cast<Entidades::Personagens::Personagem*>(olhoVoador);
	Entidades::Personagens::Personagem* p3 = static_cast<Entidades::Personagens::Personagem*>(esqueleto);
	Entidades::Personagens::Personagem* p4 = static_cast<Entidades::Personagens::Personagem*>(goblin);
	Entidades::Personagens::Personagem* p5 = static_cast<Entidades::Personagens::Personagem*>(cogumelo);

	gerenciador_grafico->setJogador(jogador);
	gerenciador_eventos->setJogador(jogador);
	gerenciador_colisoes->setJogador(jogador);

	gerenciador_colisoes->addPlataforma(platform);
	gerenciador_colisoes->addPlataforma(platform1);
	gerenciador_colisoes->addPlataforma(platform2);
	gerenciador_colisoes->addInimigo(olhoVoador);
	gerenciador_colisoes->addInimigo(esqueleto);
	gerenciador_colisoes->addInimigo(goblin);
	gerenciador_colisoes->addInimigo(cogumelo);

	plataformas.push_back(platform2);
	plataformas.push_back(platform1);
	plataformas.push_back(platform);

	personagens.push_back(p1);
	personagens.push_back(p2);
	personagens.push_back(p3);
	personagens.push_back(p4);
	personagens.push_back(p5);


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
	for (int i = 0; i < personagens.size(); i++)
	{
		gerenciador_grafico->desenhaElemento(personagens.at(i)->getCorpo());
		gerenciador_grafico->desenhaSprite(personagens.at(i)->getSprite());

	}
	for (int i = 0; i < plataformas.size(); i++)
	{
		gerenciador_grafico->desenhaElemento(plataformas.at(i)->getCorpo());
	}

	gerenciador_grafico->mostraElemento();
}
