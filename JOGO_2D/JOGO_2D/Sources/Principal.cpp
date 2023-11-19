#include "Principal.hpp"

Principal::Principal() :
	gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
	gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
	gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
	listaPersonagem(), listaObstaculo()
{
	instanciaEntidades("Fases/fase1.txt");
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
	entityCreators['s'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Obstaculos::Serra(Vector2f(posX, posY), Vector2f(100.0f, 100.0f));
		};
	for (int x = 0; x < matriz.size(); x++) {
		for (int y = 0; y < matriz[x].size(); y++) {
			float posX = x * 50.0f;
			float posY = y * 50.0f;

			char character = matriz[x][y];

			if (entityCreators.find(character) != entityCreators.end()) {
				Entidades::Entidade* entity = entityCreators[character](posX, posY);

				if (dynamic_cast<Entidades::Obstaculos::Plataforma*>(entity)) {
					gerenciador_colisoes->addPlataforma(dynamic_cast<Entidades::Obstaculos::Plataforma*>(entity));
				}
				if (dynamic_cast<Entidades::Obstaculos::Parede*>(entity)) {
					gerenciador_colisoes->addParede(dynamic_cast<Entidades::Obstaculos::Parede*>(entity));
				}
				if (dynamic_cast<Entidades::Obstaculos::Obstaculo*>(entity)) {
					gerenciador_colisoes->addObstaculo(dynamic_cast<Entidades::Obstaculos::Obstaculo*>(entity));
					listaObstaculo.addEntidade(entity);
				}
				if (dynamic_cast<Entidades::Personagens::Inimigo*>(entity)) {
					gerenciador_colisoes->addInimigo(dynamic_cast<Entidades::Personagens::Inimigo*>(entity));
					listaPersonagem.addEntidade(entity);
				}
				if (dynamic_cast<Entidades::Personagens::Jogador*>(entity)) {
					jogador = dynamic_cast<Entidades::Personagens::Jogador*>(entity);
					listaPersonagem.addEntidade(entity);
					gerenciador_grafico->setJogador(jogador);
					gerenciador_eventos->setJogador(jogador);
					gerenciador_colisoes->setJogador(jogador);
				}

			}

		}
	}
	Inimigo::setJogador(jogador);

	cout << listaObstaculo.getTam() << endl;
}

void Principal::executar()
{
	while (gerenciador_grafico->getOpen())
	{
		gerenciador_eventos->Executar();
		gerenciador_grafico->limpaTela();
		gerenciador_grafico->atualizaCamera();
		AtualizarPersonagens();
		gerenciador_colisoes->Executar();

		DesenharElementos();
	}
}

void Principal::AtualizarPersonagens()
{
	//listaPersonagem.executar(gerenciador_grafico->getJanela());
	//listaObstaculo.executar(gerenciador_grafico->getJanela());

	Entidades::Personagens::Inimigo* aux = nullptr;
	Entidades::Personagens::Personagem* pAuxPerso = nullptr;


	//Verificar se tomou dano
	if (jogador->getAtacando())
	{
		for (int i = 1; i < listaPersonagem.getTam(); i++)
		{
			if (listaPersonagem.operator[](i))
			{
				aux = dynamic_cast<Entidades::Personagens::Inimigo*>(listaPersonagem.operator[](i));
				if (aux) {
					if (fabs(aux->getPos().x - jogador->getRegiaoAtaque().x) < 80.0f &&
						fabs(aux->getPos().y - jogador->getRegiaoAtaque().y) < 80.0f)
						aux->tomarDano(jogador->getDano());
				}

			}
		}
	}

	//Verificar se morreu
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

	//Verificar se morreu
	for (int i = 1; i < listaPersonagem.getTam(); i++)
	{
		pAuxPerso = dynamic_cast<Entidades::Personagens::Personagem*>(listaPersonagem.operator[](i));
		if (pAuxPerso && pAuxPerso->getMorte())
		{
			listaPersonagem.removerEntidade(pAuxPerso);
		}

	}

	//Atualizar, cair e barra de vida
	for (int i = 0; i < listaPersonagem.getTam(); i++)
	{
		pAuxPerso = dynamic_cast<Entidades::Personagens::Personagem*>(listaPersonagem.operator[](i));
		if (pAuxPerso)
		{

			pAuxPerso->atualizar();
			pAuxPerso->cair();

		}
	}
}

void Principal::DesenharElementos()
{
	Entidades::Personagens::Personagem* pAuxPerso = nullptr;

	listaObstaculo.desenharEntidades(gerenciador_grafico);
	listaPersonagem.desenharEntidades(gerenciador_grafico);

	for (int i = 0; i < listaPersonagem.getTam(); i++)
	{
		pAuxPerso = dynamic_cast<Entidades::Personagens::Personagem*>(listaPersonagem.operator[](i));
		if (pAuxPerso)
		{
			pAuxPerso->atualizarBarraVida();
			gerenciador_grafico->desenhaSprite(pAuxPerso->getBorder());
			gerenciador_grafico->desenhaSprite(pAuxPerso->getHealthBar());

		}
	}

	gerenciador_grafico->mostraElemento();

}
