#include "../Headers/Fase.hpp"

Fases::Fase::Fase():gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
listaPersonagem(),
listaObstaculo(),
jogador(nullptr),
jogador2(nullptr),
jogadorCriado(false),
numJogadores(0)
{
	
}

Fases::Fase::~Fase()
{

}

bool Fases::Fase::Aleatorizar(char character)
{
	static int numCogumelosCriados = 0;
	static int numVoadoresCriados = 0;
	static int numSerrasCriadas = 0;
	static int numEspinhosCriados = 0;
	static int numSlimesCriados = 0;

	const int numMinimoCogumelos = 3;
	const int numMinimoVoadores = 3;
	const int numMinimoSerras = 3;
	const int numMinimoEspinhos = 3;
	const int numMinimoSlimes = 3;

	static bool seedInicializada = false;
	if (!seedInicializada) {
		std::srand(std::time(0));
		seedInicializada = true;
	}

	if (character == 'f') {
		if (numVoadoresCriados < numMinimoVoadores) {
			numVoadoresCriados++;
			return true;
		}
		else {
			return std::rand() % 2 == 0;
		}
	}
	else if (character == 'm') {
		if (numCogumelosCriados < numMinimoCogumelos) {
			numCogumelosCriados++;
			return true;
		}
		else {
			return std::rand() % 2 == 0;
		}
	}
	else if (character == 's') {
		if (numSerrasCriadas < numMinimoSerras) {
			numSerrasCriadas++;
			return true;
		}
		else {
			return std::rand() % 2 == 0;
		}
	}
	else if (character == 'e') {
		if (numEspinhosCriados < numMinimoEspinhos) {
			numEspinhosCriados++;
			return true;
		}
		else {
			return std::rand() % 2 == 0;
		}
	}
	else if (character == 'g') {
		if (numSlimesCriados < numMinimoSlimes) {
			numSlimesCriados++;
			return true;
		}
		else {
			return std::rand() % 2 == 0;
		}
	}

	return true;
}

void Fases::Fase::instanciaEntidades(const std::string& arquivoTxt)
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
	entityCreators['e'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Obstaculos::Espinho(Vector2f(posX, posY), Vector2f(100.0f, 50.0f));
		};
	entityCreators['g'] = [](float posX, float posY) -> Entidades::Entidade* {
		return new Entidades::Obstaculos::Slime(Vector2f(posX, posY), Vector2f(300.0f, 50.0f));
		};
	for (int x = 0; x < matriz.size(); x++) {
		for (int y = 0; y < matriz[x].size(); y++) {
			float posX = x * 50.0f;
			float posY = y * 50.0f;

			char character = matriz[x][y];

			if (Aleatorizar(character))
			{
				if (entityCreators.find(character) != entityCreators.end()) {
					Entidades::Entidade* entity = entityCreators[character](posX, posY);
					if (dynamic_cast<Entidades::Obstaculos::Obstaculo*>(entity)) {
						gerenciador_colisoes->addObstaculo(dynamic_cast<Entidades::Obstaculos::Obstaculo*>(entity));
						gerenciador_colisoes->addCorpo(dynamic_cast<Entidades::Obstaculos::Obstaculo*>(entity));
						listaObstaculo.addEntidade(entity);
					}
					if (dynamic_cast<Entidades::Personagens::Inimigo*>(entity)) {
						gerenciador_colisoes->addInimigo(dynamic_cast<Entidades::Personagens::Inimigo*>(entity));
						listaPersonagem.addEntidade(entity);
					}
					if (dynamic_cast<Entidades::Personagens::Jogador*>(entity))
					{
						if (!jogadorCriado)
						{
							jogador = dynamic_cast<Entidades::Personagens::Jogador*>(entity);
							listaPersonagem.addEntidade(entity);
							gerenciador_grafico->setJogador(jogador);
							gerenciador_eventos->setJogador(jogador);
							gerenciador_colisoes->setJogador(jogador);

							jogadorCriado = true;
						}
						else
						{
							jogador2 = dynamic_cast<Entidades::Personagens::Jogador*>(entity);
							listaPersonagem.addEntidade(entity);
							gerenciador_grafico->setJogador2(jogador2);
							gerenciador_eventos->setJogador2(jogador2);
							gerenciador_colisoes->setJogador2(jogador2);
						}
						numJogadores++;
					}
				}
			}
		}
	}

	if (numJogadores == 1)
	{
		Inimigo::setJogador(jogador);
		Obstaculo::setJogador(jogador);
	}
	else if (numJogadores == 2)
	{
		Inimigo::setJogador(jogador);
		Obstaculo::setJogador(jogador);

		Inimigo::setJogador2(jogador2);
		Obstaculo::setJogador2(jogador2);
	}
	else
	{
		cout << "numero incomum de jogadores" << endl;
		exit(1);
	}

}

void Fases::Fase::AtualizarPersonagens()
{
	//listaPersonagem.executar(gerenciador_grafico->getJanela());
	//listaObstaculo.executar(gerenciador_grafico->getJanela());

	Entidades::Personagens::Inimigo* pAuxInim = nullptr;
	Entidades::Personagens::Personagem* pAuxPerso = nullptr;
	Entidades::Obstaculos::Obstaculo* pAuxObst = nullptr;


	//Verificar se tomou dano
	if (jogador->getAtacando())
	{
		for (int i = 1; i < listaPersonagem.getTam(); i++)
		{
			if (listaPersonagem.operator[](i))
			{
				pAuxInim = dynamic_cast<Entidades::Personagens::Inimigo*>(listaPersonagem.operator[](i));
				if (pAuxInim) {
					if (fabs(pAuxInim->getPos().x - jogador->getRegiaoAtaque().x) < 80.0f &&
						fabs(pAuxInim->getPos().y - jogador->getRegiaoAtaque().y) < 80.0f)
						pAuxInim->tomarDano(jogador->getDano());
				}

			}
		}
	}

	if (jogador2 && jogador2->getAtacando())
	{
		for (int i = 1; i < listaPersonagem.getTam(); i++)
		{
			if (listaPersonagem.operator[](i))
			{
				pAuxInim = dynamic_cast<Entidades::Personagens::Inimigo*>(listaPersonagem.operator[](i));
				if (pAuxInim) {
					if (fabs(pAuxInim->getPos().x - jogador2->getRegiaoAtaque().x) < 80.0f &&
						fabs(pAuxInim->getPos().y - jogador2->getRegiaoAtaque().y) < 80.0f)
						pAuxInim->tomarDano(jogador2->getDano());
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
	if (jogador2)
	{
		if (jogador2->getMorte()) {
			gerenciador_colisoes->removeJogador();
			jogador2 = nullptr;
			gerenciador_grafico->setJogador2(nullptr);
			gerenciador_eventos->setJogador2(nullptr);
			gerenciador_colisoes->setJogador2(nullptr);
		}
	}

	//Verificar se morreu
	for (int i = 0; i < listaPersonagem.getTam(); i++)
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

	//atualizar obstaculos
	for (int i = 0; i < listaObstaculo.getTam(); i++)
	{
		pAuxObst = dynamic_cast<Entidades::Obstaculos::Obstaculo*>(listaObstaculo.operator[](i));
		if (pAuxObst)
		{
			pAuxObst->atualizar();
			gerenciador_grafico->desenhaSprite(pAuxObst->getSprite());
			//gerenciador_grafico->desenhaHitbox(*pAuxObst->getCorpo());

		}
	}
}

void Fases::Fase::DesenharElementos()
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