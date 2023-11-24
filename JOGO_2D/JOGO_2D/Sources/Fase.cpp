#include "../Headers/Fase.hpp"
namespace Fases
{
	Fase::Fase() :
		Ente(),
		gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
		gerenciador_eventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
		gerenciador_colisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
		listaPersonagem(),
		listaObstaculo(),
		jogador(nullptr),
		jogador2(nullptr),
		jogadorCriado(false),
		numJogadores(0),
		fase(),
		texturaFundo(),
		concluida(false),
		derrota(false),
		numCogumelosCriados(0),
		numVoadoresCriados(0),
		numSerrasCriadas(0),
		numEspinhosCriados(0),
		numSlimesCriados(0),
		morreu(false)
	{
		id = 2;
	}

	Fase::~Fase()
	{

	}

	bool Fase::Aleatorizar(char character)
	{
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
				return !(std::rand() % 3 == 0);
			}
		}
		else if (character == 'm') {
			if (numCogumelosCriados < numMinimoCogumelos) {
				numCogumelosCriados++;
				return true;
			}
			else {
				return !(std::rand() % 3 == 0);
			}
		}
		else if (character == 's') {
			if (numSerrasCriadas < numMinimoSerras) {
				numSerrasCriadas++;
				return true;
			}
			else {
				return !(std::rand() % 3 == 0);
			}
		}
		else if (character == 'e') {
			if (numEspinhosCriados < numMinimoEspinhos) {
				numEspinhosCriados++;
				return true;
			}
			else {
				return !(std::rand() % 3 == 0);
			}
		}
		else if (character == 'g') {
			if (numSlimesCriados < numMinimoSlimes) {
				numSlimesCriados++;
				return true;
			}
			else {
				return !(std::rand() % 3 == 0);
			}
		}

		return true;
	}

	void Fase::instanciaEntidades(const std::string& arquivoTxt)
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
			return new Entidades::Personagens::OlhoVoador(Vector2f(posX, posY), Vector2f(40.0f, 50.0f));
			};
		entityCreators['c'] = [](float posX, float posY) -> Entidades::Entidade* {
			return new Entidades::Personagens::Chefao(Vector2f(posX, posY), Vector2f(100.0f, 120.0f));
			};
		entityCreators['m'] = [](float posX, float posY) -> Entidades::Entidade* {
			return new Entidades::Personagens::Cogumelo(Vector2f(posX, posY), Vector2f(40.0f, 80.0f));
			};
		entityCreators['p'] = [](float posX, float posY) -> Entidades::Entidade* {
			return new Entidades::Obstaculos::Plataforma(Vector2f(posX, posY), Vector2f(300.0f, 50.0f), false);
			};
		entityCreators['P'] = [](float posX, float posY) -> Entidades::Entidade* {
			return new Entidades::Obstaculos::Plataforma(Vector2f(posX, posY), Vector2f(1900.0f, 50.0f), true);
			};
		entityCreators['w'] = [](float posX, float posY) -> Entidades::Entidade* {
			return new Entidades::Obstaculos::Parede(Vector2f(posX, posY), Vector2f(50.0f, 3600.0f));
			};
		entityCreators['s'] = [](float posX, float posY) -> Entidades::Entidade* {
			return new Entidades::Obstaculos::Serra(Vector2f(posX, posY), Vector2f(100.0f, 100.0f));
			};		
		entityCreators['g'] = [](float posX, float posY) -> Entidades::Entidade* {
			return new Entidades::Obstaculos::Slime(Vector2f(posX, posY), Vector2f(300.0f, 50.0f));
			};
		entityCreators['e'] = [](float posX, float posY) -> Entidades::Entidade* {
			return new Entidades::Obstaculos::Espinho(Vector2f(posX, posY), Vector2f(100.0f, 50.0f));
			};
		entityCreators['t'] = [](float posX, float posY) -> Entidades::Entidade* {
			return new Entidades::Obstaculos::Portal(Vector2f(posX, posY), Vector2f(50.0f, 50.0f), true, true);
			};
		entityCreators['T'] = [](float posX, float posY) -> Entidades::Entidade* {
			return new Entidades::Obstaculos::Portal(Vector2f(posX, posY), Vector2f(50.0f, 50.0f), true, false);
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
						if (dynamic_cast<Entidades::Obstaculos::Plataforma*>(entity)) {
							dynamic_cast<Entidades::Obstaculos::Plataforma*>(entity)->setFase(fase);
						}
						if (dynamic_cast<Entidades::Personagens::Jogador*>(entity))
						{
							if (!jogadorCriado)
							{
								jogador = dynamic_cast<Entidades::Personagens::Jogador*>(entity);
								listaPersonagem.addEntidade(entity);
								gerenciador_eventos->setJogador(jogador);
								gerenciador_colisoes->setJogador(jogador);

								jogadorCriado = true;
							}
							else
							{
								jogador2 = dynamic_cast<Entidades::Personagens::Jogador*>(entity);
								listaPersonagem.addEntidade(entity);
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

	void Fase::desalocaEntidades()
	{
		gerenciador_eventos->setJogador(nullptr);
		gerenciador_colisoes->setJogador(nullptr);

		gerenciador_eventos->setJogador2(nullptr);
		gerenciador_colisoes->setJogador2(nullptr);

		gerenciador_colisoes->limparListas();

		Inimigo::setJogador(nullptr);
		Obstaculo::setJogador(nullptr);		
		
		Inimigo::setJogador2(nullptr);
		Obstaculo::setJogador2(nullptr);

		listaPersonagem.limpar();
		listaObstaculo.limpar();

		jogador = nullptr;
		jogador2 = nullptr;

		jogadorCriado = false;
		numJogadores = 0;

		concluida = false;
		derrota = false;

		Jogador::setJogadorCriado(false);

		int numCogumelosCriados = 0;
		int numVoadoresCriados = 0;
		int numSerrasCriadas = 0;
		int numEspinhosCriados = 0;
		int numSlimesCriados = 0;

		Inimigo::setChefaoMorreu(false);

	}

	void Fase::AtualizarPersonagens()
	{
		Entidades::Personagens::Inimigo* pAuxInim = nullptr;
		Entidades::Personagens::Personagem* pAuxPerso = nullptr;
		Entidades::Obstaculos::Obstaculo* pAuxObst = nullptr;


		if (Inimigo::getChefaoMorreu() && !morreu)
		{
			Obstaculos::Portal* p1 = new Entidades::Obstaculos::Portal(Inimigo::getPosChefao(), Vector2f(50.0f, 50.0f),true,true);
			gerenciador_colisoes->addObstaculo(p1);
			gerenciador_colisoes->addCorpo(p1);
			listaObstaculo.addEntidade(p1);

			morreu = true;
		}

		//Verificar se tomou dano
		if (jogador && jogador->getAtacando())
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
				gerenciador_eventos->setJogador(nullptr);
				gerenciador_colisoes->setJogador(nullptr);
				Inimigo::setJogador(nullptr);
				Obstaculo::setJogador(nullptr);
				jogador = nullptr;
			}
		}

		//Verificar se morreu
		if (jogador2)
		{
			if (jogador2->getMorte()) {
				gerenciador_eventos->setJogador2(nullptr);
				gerenciador_colisoes->setJogador2(nullptr);
				Inimigo::setJogador2(nullptr);
				Obstaculo::setJogador2(nullptr);
				jogador2 = nullptr;
			}
		}

		//atualizar obstaculos
		for (int i = 0; i < listaObstaculo.getTam(); i++)
		{
			pAuxObst = dynamic_cast<Entidades::Obstaculos::Obstaculo*>(listaObstaculo.operator[](i));
			if (pAuxObst)
			{
				pAuxObst->atualizar();
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
	}

	void Fase::atualizaCamera()
	{
		sf::Vector2f cameraCenter = gerenciador_grafico->getViewCenter();
		sf::Vector2f pos;

		if (jogador2 && jogador)
		{
			sf::Vector2f playerPosition = jogador->getPos();
			sf::Vector2f player2Position = jogador2->getPos();

			float middleX = (playerPosition.x + player2Position.x) / 2.0f;

			cameraCenter.x = middleX;

			pos.x = (middleX - (TELA_X / 2));
			pos.y = -30.0f;
		}
		else if (jogador2)
		{
			sf::Vector2f player2Position = jogador2->getPos();

			cameraCenter.x = player2Position.x;

			pos.x = player2Position.x - (TELA_X / 2);
			pos.y = -30.0f;

		}
		else if (jogador)
		{
			sf::Vector2f playerPosition = jogador->getPos();

			cameraCenter.x = playerPosition.x;

			pos.x = playerPosition.x - (TELA_X / 2);
			pos.y = -30.0f;
		}
		else
		{

		}

		sf::Sprite sprite(texturaFundo);
		sprite.setScale(0.9f, 0.9f);
		sprite.setPosition(pos);
		
		gerenciador_grafico->desenhaSprite(sprite);
		gerenciador_grafico->setCentro(cameraCenter);
	}

	int Fase::getFase()
	{
		return fase;
	}
	
	bool Fase::getConcluida()
	{
		return concluida;
	}

	bool Fase::getDerrota()
	{
		return derrota;
	}

	void Fase::setDerrota(bool d)
	{
		if(!derrota)
			derrota = d;
	}

	void Fase::verificaFase()
	{
		if ((jogador && jogador->getConcluiuFase()) || (jogador2 && jogador2->getConcluiuFase()))
		{
			concluida = true;
		}
		
		if(jogador == nullptr && jogador2 == nullptr)
		{
			derrota = true;
		}
	}
	void Fase::salvarJogo(int save)
	{
		if (save > 3 || save <= 0)
		{
			cout << "save inexistente" << endl;
			exit(1);
		}

		limparArquivo(save);

		Entidades::Personagens::Inimigo* pAuxInim = nullptr;
		Entidades::Personagens::Personagem* pAuxPerso = nullptr;
		Entidades::Obstaculos::Obstaculo* pAuxObst = nullptr;

		for (int i = 0; i < listaPersonagem.getTam(); i++)
		{
			if (listaPersonagem.operator[](i))
			{
				pAuxPerso = dynamic_cast<Entidades::Personagens::Personagem*>(listaPersonagem.operator[](i));
				if (pAuxPerso) 
				{
					pAuxPerso->salvar(save);
				}
			}
		}

		for (int i = 0; i < listaObstaculo.getTam(); i++)
		{
			if (listaObstaculo.operator[](i))
			{
				pAuxObst = dynamic_cast<Entidades::Obstaculos::Obstaculo*>(listaObstaculo.operator[](i));
				if (pAuxObst)
				{
					pAuxObst->salvar(save);
				}
			}
		}
	}

	void Fase::limparArquivo(int save)
	{
		Entidades::Personagens::Personagem* pAuxPerso = nullptr;
		Entidades::Obstaculos::Obstaculo* pAuxObst = nullptr;

		for (int i = 0; i < listaPersonagem.getTam(); i++)
		{
			if (listaPersonagem.operator[](i))
			{
				pAuxPerso = dynamic_cast<Entidades::Personagens::Personagem*>(listaPersonagem.operator[](i));
				if (pAuxPerso)
				{
					pAuxPerso->limparArquivo(save);
				}
			}
		}

		for (int i = 0; i < listaObstaculo.getTam(); i++)
		{
			if (listaObstaculo.operator[](i))
			{
				pAuxObst = dynamic_cast<Entidades::Obstaculos::Obstaculo*>(listaObstaculo.operator[](i));
				if (pAuxObst)
				{
					pAuxObst->limparArquivo(save);
				}
			}
		}
	}

	void Fase::recuperarJogo(int save)
	{

	}
}