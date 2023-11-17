#include "Ente.hpp"

Ente::Ente() :
	gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
	gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
	gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador())
{
	instanciaEntidades("fase1.txt");
}

Ente::~Ente()
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
void Ente::Executar()
{
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
void Ente::instanciaEntidades(const std::string& arquivoTxt)
{

	std::ifstream arquivo(arquivoTxt);
	if (!arquivo.is_open()) {
		cout << "Nao foi possivel abrir o arquivo da fase" << endl;
		exit(1);
	}

	std::vector<std::vector<char>> matriz;

	std::string linha;
	while (std::getline(arquivo, linha)) {
		std::vector<char> linhaChars;
		for (char c : linha) {
			linhaChars.push_back(c);
		}
		matriz.push_back(linhaChars);
	}
	arquivo.close();

	std::map<char, std::function<Entidades::Entidade* (float, float)>> entityCreators;
	entityCreators['j'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Personagens::Jogador(Vector2f(posX, posY), Vector2f(30.0f, 90.0f));
		};
	entityCreators['f'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Personagens::FlyingEye(Vector2f(posX, posY), Vector2f(40.0f, 50.0f));
		};
	entityCreators['c'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Personagens::Chefao(Vector2f(posX, posY), Vector2f(100.0f, 120.0f));
		};
	entityCreators['m'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Personagens::Mushroom(Vector2f(posX, posY), Vector2f(40.0f, 80.0f));
		};
	entityCreators['p'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Obstaculos::Plataforma(Vector2f(posX, posY), Vector2f(300.0f, 50.0f), false);
		};
	entityCreators['P'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Obstaculos::Plataforma(Vector2f(posX, posY), Vector2f(1900.0f, 50.0f), true);
		};
	entityCreators['w'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Obstaculos::Parede(Vector2f(posX, posY), Vector2f(50.0f, 1200.0f));
		};	

	for (int x = 0; x < matriz.size(); x++) {
		for (int y = 0; y < matriz[x].size(); y++) {
			float posX = x * 50.0f;
			float posY = y * 50.0f;

			char character = matriz[x][y];

			if (entityCreators.find(character) != entityCreators.end()) {
				Entidades::Entidade* entity = entityCreators[character](posX, posY);
				if (dynamic_cast<Entidades::Personagens::Personagem*>(entity)) {
					personagens.push_back(dynamic_cast<Entidades::Personagens::Personagem*>(entity));
				}
				if (dynamic_cast<Entidades::Obstaculos::Plataforma*>(entity)) {
					plataformas.push_back(dynamic_cast<Entidades::Obstaculos::Plataforma*>(entity));
					gerenciador_colisoes->addPlataforma(dynamic_cast<Entidades::Obstaculos::Plataforma*>(entity));
				}
				if (dynamic_cast<Entidades::Obstaculos::Parede*>(entity)) {
					gerenciador_colisoes->addParede(dynamic_cast<Entidades::Obstaculos::Parede*>(entity));
				}
				if (dynamic_cast<Entidades::Personagens::Inimigo*>(entity)) {
					gerenciador_colisoes->addInimigo(dynamic_cast<Entidades::Personagens::Inimigo*>(entity));
					inimigos.push_back(dynamic_cast<Entidades::Personagens::Inimigo*>(entity));
				}
				if (dynamic_cast<Entidades::Personagens::Jogador*>(entity)) {
					jogador = dynamic_cast<Entidades::Personagens::Jogador*>(entity);
					gerenciador_grafico->setJogador(jogador);
					gerenciador_eventos->setJogador(jogador);
					gerenciador_colisoes->setJogador(jogador);
				}
			}
		}
	}
	for (int i = 0; i < inimigos.size(); i++)
	{
		inimigos.at(i)->setJogador(jogador);
	}
}

void Ente::AtualizarPersonagens()
{
	if (jogador->getAtacando())
	{
		for (int i = 0; i < inimigos.size(); i++)
		{
			if (inimigos.at(i))
			{
				if (fabs(inimigos.at(i)->getPos().x - jogador->getRegiaoAtaque().x) < 80.0f &&
					fabs(inimigos.at(i)->getPos().y - jogador->getRegiaoAtaque().y) < 80.0f)
					inimigos.at(i)->tomarDano(jogador->getDano());
			}
		}
	}

	if (jogador)
	{
		if (jogador->getMorte())
		{
			exit(1);
			/*
			gerenciador_colisoes->removeJogador();
			jogador = nullptr;
			gerenciador_grafico->setJogador(nullptr);
			gerenciador_eventos->setJogador(nullptr);
			gerenciador_colisoes->setJogador(nullptr);
			*/

		}
	}

	for (int i = 1; i < personagens.size(); i++)
	{
		if (personagens.at(i))
		{
			//cout << i << " - " << personagens.at(i)->getMorte() << endl;

			if (personagens.at(i)->getMorte())
			{

				gerenciador_colisoes->removeInimigo(i - 1);
				personagens[i] = nullptr;
				inimigos[i - 1] = nullptr;
			}
		}
	}

	for (int i = 0; i < personagens.size(); i++)
	{
		if (personagens.at(i))
		{
			personagens.at(i)->atualizar();
			personagens.at(i)->cair();
		}

	}
}

void Ente::DesenharElementos()
{

	for (int i = 0; i < plataformas.size(); i++)
	{
		//gerenciador_grafico->desenhaHitbox(plataformas.at(i)->getCorpo());
		gerenciador_grafico->desenhaSprite(plataformas.at(i)->getSprite());
	}

	for (int i = 0; i < personagens.size(); i++)
	{
		if (personagens.at(i))
		{
			//gerenciador_grafico->desenhaHitbox(personagens.at(i)->getCorpo());
			gerenciador_grafico->desenhaSprite(personagens.at(i)->getSprite());
			personagens.at(i)->atualizarBarraVida();
			gerenciador_grafico->desenhaSprite(personagens.at(i)->getBorder());
			gerenciador_grafico->desenhaSprite(personagens.at(i)->getHealthBar());
		}
	}

	gerenciador_grafico->mostraElemento();
}