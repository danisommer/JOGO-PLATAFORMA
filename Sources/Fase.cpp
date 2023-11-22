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
		texturaFundo()
	{
		id = 2;
	}

	Fase::~Fase()
	{

	}

	bool Fase::Aleatorizar(char character)
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

	void Fase::instanciaEntidades(char character, Vector2f pos)
	{

		switch (character)
		{
		case('j'):
		{
			criarJogador(Vector2f(pos.x * 50.0f, pos.y * 50.0f));
			cout << "Jogador Criado" << endl;
		}
		break;
		case('f'):
		{
			criarOlhoVoador(Vector2f(pos.x * 50.0f, pos.y * 50.0f));
			cout << "Olho Criado" << endl;
		}
		break;
		case('c'):
		{
			criarChefao(Vector2f(pos.x * 50.0f, pos.y * 50.0f));
		}
		break;
		case('m'):
		{
			criarCogumelo(Vector2f(pos.x * 50.0f, pos.y * 50.0f));
		}
		break;
		case('p'):
		{
			criarPlataformaPequena(Vector2f(pos.x * 50.0f, pos.y * 50.0f));
			cout << "pequena criada" << endl;
		}
		break;
		case('P'):
		{
			criarPlataformaGrande(Vector2f(pos.x * 50.0f, pos.y * 50.0f));
			cout << "grande criada" << endl;
		}
		break;
		case('w'):
		{
			criarParede(Vector2f(pos.x * 50.0f, pos.y * 50.0f));
		}
		break;
		case('s'):
		{
			criarSerra(Vector2f(pos.x * 50.0f, pos.y * 50.0f));
		}
		break;
		case('e'):
		{
			criarEspinho(Vector2f(pos.x * 50.0f, pos.y * 50.0f));
		}
		break;
		case('g'):
		{
			criarSlime(Vector2f(pos.x * 50.0f, pos.y * 50.0f));
		}
		}

	}

	void Fase::AtualizarPersonagens()
	{
		Entidades::Personagens::Inimigo* pAuxInim = nullptr;
		Entidades::Personagens::Personagem* pAuxPerso = nullptr;
		Entidades::Obstaculos::Obstaculo* pAuxObst = nullptr;


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
		sf::Sprite sprite(texturaFundo);
		sprite.setScale(0.9f, 0.9f);

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

		sprite.setPosition(pos);
		gerenciador_grafico->desenhaSprite(sprite);
		gerenciador_grafico->setCentro(cameraCenter);
	}

	int Fase::getFase()
	{
		return fase;
	}

	void Fases::Fase::criarOlhoVoador(Vector2f pos)
	{
		Entidades::Entidade* olho = new Entidades::Personagens::OlhoVoador(pos, Vector2f(40.0f, 50.0f));
		if (!olho) {
			std::cout << "Nao foi possivel criar o Olho Voador" << std::endl;
			exit(1);
		}
		if (Aleatorizar('f'))
		{
			if (dynamic_cast<Entidades::Personagens::Inimigo*>(olho)) {
				gerenciador_colisoes->addInimigo(dynamic_cast<Entidades::Personagens::Inimigo*>(olho));
				listaPersonagem.addEntidade(olho);
			}
			
		}

	}

	void Fases::Fase::criarChefao(Vector2f pos)
	{
		Entidades::Entidade* chefao = new Entidades::Personagens::Chefao(pos, Vector2f(40.0f, 50.0f));

		if (!chefao) {
			std::cout << "Nao foi possivel criar o Chefao" << std::endl;
			exit(1);
		}

		if (dynamic_cast<Entidades::Personagens::Inimigo*>(chefao)) {
			gerenciador_colisoes->addInimigo(dynamic_cast<Entidades::Personagens::Inimigo*>(chefao));
				listaPersonagem.addEntidade(chefao);
		}
		
	}

	void Fases::Fase::criarCogumelo(Vector2f pos)
	{
		Entidades::Entidade* cogumelo = new Entidades::Personagens::Cogumelo(pos, Vector2f(40.0f, 50.0f));

		if (!cogumelo) {
			std::cout << "Nao foi possivel criar o Cogumelo" << std::endl;
			exit(1);
		}
		if (Aleatorizar('m'))
		{
			if (dynamic_cast<Entidades::Personagens::Inimigo*>(cogumelo)) {
				gerenciador_colisoes->addInimigo(dynamic_cast<Entidades::Personagens::Inimigo*>(cogumelo));
				listaPersonagem.addEntidade(cogumelo);
			}
		}
			
	}

	void Fases::Fase::criarPlataformaPequena(const Vector2f pos)
	{
		Entidades::Obstaculos::Plataforma* plataformaPequena = new Entidades::Obstaculos::Plataforma(pos, Vector2f(300.0f, 50.0f), false);

		if (!plataformaPequena) {
			std::cout << "Nao foi possivel criar a plataforma pequena" << std::endl;
			exit(1);
		}
		listaObstaculo.addEntidade(static_cast<Entidades::Entidade*>(plataformaPequena));
	}

	void Fases::Fase::criarPlataformaGrande(const Vector2f pos)
	{
		Entidades::Entidade* plataformaGrande = new Entidades::Obstaculos::Plataforma(pos, Vector2f(1900.0f, 50.0f), true);

		if (!plataformaGrande) {
			std::cout << "Nao foi possivel criar a plataforma pequena" << std::endl;
			exit(1);
		}
		if (dynamic_cast<Entidades::Obstaculos::Plataforma*>(plataformaGrande)) {
			dynamic_cast<Entidades::Obstaculos::Plataforma*>(plataformaGrande)->setFase(fase);
		}
		
	}
	void Fases::Fase::criarParede(const Vector2f pos)
	{
		Entidades::Obstaculos::Parede* parede = new Entidades::Obstaculos::Parede(pos, Vector2f(50.0f, 1200.0f));

		if (!parede) {
			std::cout << "Nao foi possivel criar parede" << std::endl;
			exit(1);
		}
		listaObstaculo.addEntidade(static_cast<Entidades::Entidade*>(parede));
	}

	void Fases::Fase::criarSerra(const Vector2f pos)
	{
		Entidades::Obstaculos::Serra* serra = new Entidades::Obstaculos::Serra(pos, Vector2f(100.0f, 100.0f));

		if (Aleatorizar('s'))
		{
			if (!serra) {
				std::cout << "Nao foi possivel criar serra" << std::endl;
				exit(1);
			}
			listaObstaculo.addEntidade(static_cast<Entidades::Entidade*>(serra));
		}
	}

	void Fases::Fase::criarEspinho(const Vector2f pos)
	{
		Entidades::Obstaculos::Espinho* espinho = new Entidades::Obstaculos::Espinho(pos, Vector2f(100.0f, 50.0f));

		if (Aleatorizar('e'))
		{
			if (!espinho) {
				std::cout << "Nao foi possivel criar espinho" << std::endl;
				exit(1);
			}
			listaObstaculo.addEntidade(static_cast<Entidades::Entidade*>(espinho));
		}
	}

	void Fases::Fase::criarSlime(const Vector2f pos)
	{
		Entidades::Obstaculos::Slime* slime = new Entidades::Obstaculos::Slime(pos, Vector2f(300.0f, 50.0f));

		if (Aleatorizar('g'))
		{
			if (!slime) {
				std::cout << "Nao foi possivel criar slime" << std::endl;
				exit(1);
			}
			listaObstaculo.addEntidade(static_cast<Entidades::Entidade*>(slime));
		}
	}

	void Fases::Fase::criarJogador(const Vector2f pos)
	{
		Entidades::Personagens::Jogador* entity = new Entidades::Personagens::Jogador(pos, Vector2f(30.0f, 90.0f));

		if (!entity) {
			std::cout << "Nao foi possivel criar Jogador" << std::endl;
			exit(1);
		}
		if (!jogadorCriado)
		{
			jogador = dynamic_cast<Entidades::Personagens::Jogador*>(entity);
			listaPersonagem.addEntidade(entity);
			//gerenciador_grafico->setJogador(jogador);
			gerenciador_eventos->setJogador(jogador);
			gerenciador_colisoes->setJogador(jogador);

			jogadorCriado = true;
		}
		else
		{
			jogador2 = dynamic_cast<Entidades::Personagens::Jogador*>(entity);
			listaPersonagem.addEntidade(entity);
			//gerenciador_grafico->setJogador2(jogador2);
			gerenciador_eventos->setJogador2(jogador2);
			gerenciador_colisoes->setJogador2(jogador2);
		}
		numJogadores++;
	}
	
	void Fases::Fase::desenhaElementos()
	{
		listaPersonagem.desenharEntidades(gerenciador_grafico);
		listaObstaculo.desenharEntidades(gerenciador_grafico);
	}

	void Fases::Fase::executaLista()
	{
		listaPersonagem.executar(gerenciador_grafico->getJanela());
		listaObstaculo.executar(gerenciador_grafico->getJanela());
	}
}

