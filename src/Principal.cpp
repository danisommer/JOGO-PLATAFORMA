#include "Principal.hpp"

Principal::Principal():
	gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
	gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
	gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
	listaPersonagem(), listaObstaculo()
{
	instanciaEntidades("fase1.txt");
}
Principal::~Principal()
{
	listaPersonagem.limpar();
	listaObstaculo.limpar();
}
void Principal::instanciaEntidades(const std::string& arquivoTxt)
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
	entityCreators['s'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Personagens::Skeleton(Vector2f(posX, posY), Vector2f(40.0f, 80.0f));
		};
	entityCreators['g'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Personagens::Goblin(Vector2f(posX, posY), Vector2f(40.0f, 80.0f));
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

	for (int x = 0; x < matriz.size(); x++) {
		for (int y = 0; y < matriz[x].size(); y++) {
			float posX = x * 50.0f;
			float posY = y * 50.0f;

			char character = matriz[x][y];

			if (entityCreators.find(character) != entityCreators.end()) {
				Entidades::Entidade* entity = entityCreators[character](posX, posY);
				if (dynamic_cast<Entidades::Personagens::Personagem*>(entity)) {
					//personagens.push_back(dynamic_cast<Entidades::Personagens::Personagem*>(entity));
					listaPersonagem.addEntidade(entity);
				}
				if (dynamic_cast<Entidades::Obstaculos::Plataforma*>(entity)) {
					//plataformas.push_back(dynamic_cast<Entidades::Obstaculos::Plataforma*>(entity));

					gerenciador_colisoes->addPlataforma(dynamic_cast<Entidades::Obstaculos::Plataforma*>(entity));
					listaObstaculo.addEntidade(entity);
				}
				if (dynamic_cast<Entidades::Personagens::Inimigo*>(entity)) {
					gerenciador_colisoes->addInimigo(dynamic_cast<Entidades::Personagens::Inimigo*>(entity));
					//inimigos.push_back(dynamic_cast<Entidades::Personagens::Inimigo*>(entity));
					listaPersonagem.addEntidade(entity);
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
	Inimigo::setJogador(jogador);
}
void Principal::executar()
{
	while (gerenciador_grafico->getOpen())
	{
		sf::Event evento;

		while (gerenciador_grafico->getJanela()->pollEvent(evento))
		{
			if (evento.type == sf::Event::Closed)
			{
				gerenciador_grafico->fecharJanela();
			}
		}

		gerenciador_eventos->Executar();
		gerenciador_grafico->limpaTela();
		gerenciador_grafico->atualizaCamera();
		listaPersonagem.executar(gerenciador_grafico->getJanela());
		listaObstaculo.executar(gerenciador_grafico->getJanela());
		gerenciador_colisoes->Executar();
		AtualizarPersonagens();
		listaPersonagem.desenharEntidades(gerenciador_grafico);
		listaObstaculo.desenharEntidades(gerenciador_grafico);
		gerenciador_grafico->mostraElemento();
		//std::cout << "Entrando" << std::endl;
	}
}
void Principal::AtualizarPersonagens()
{
	Entidades::Personagens::Inimigo* aux = nullptr;
	Entidades::Personagens::Personagem* pAuxPerso = nullptr;
	if (jogador->getAtacando())
	{
		for (int i = 1; i < listaPersonagem.getTam(); i++)
		{
			if (listaPersonagem.operator[](i))
			{
				aux = dynamic_cast<Entidades::Personagens::Inimigo*>(listaPersonagem.operator[](i));
				if (aux) {
					if (fabs(aux->getPos().x - jogador->getRegiaoAtaque().x) < 80.0f &&
						fabs(aux->getPos().y - jogador->getRegiaoAtaque().y) < 45.0f)
						aux->tomarDano(jogador->getDano());
				}

			}
		}
	}

	if (jogador)
	{
		if (jogador->getMorte()) {
			gerenciador_colisoes->removeJogador();
			jogador = nullptr;
			gerenciador_grafico->setJogador(nullptr);
			gerenciador_eventos->setJogador(nullptr);
			gerenciador_colisoes->setJogador(nullptr);
		}
	}

	for (int i = 1; i < listaPersonagem.getTam(); i++)
	{
		pAuxPerso = dynamic_cast<Entidades::Personagens::Personagem*>(listaPersonagem.operator[](i));
		if (pAuxPerso && pAuxPerso->getMorte())
		{
			/*gerenciador_colisoes->removeInimigo(i - 1);
			personagens[i] = nullptr;
			inimigos[i - 1] = nullptr;*/
			std::cout << "Morreu" << std::endl;
		}

	}

	for (int i = 0; i < listaPersonagem.getTam(); i++)
	{
		pAuxPerso = dynamic_cast<Entidades::Personagens::Personagem*>(listaPersonagem.operator[](i));
		if (pAuxPerso)
		{
			//personagens.at(i)->atualizar();
			pAuxPerso->cair();
		}

	}


}
