#include "Chefao.hpp"

#include "iostream"
#define VIDA_MAX 600.0f
#define SIZE 3.5f


using namespace std;

namespace Entidades
{
	namespace Personagens
	{

		Chefao::Chefao(Vector2f pos, Vector2f tam) :
			Inimigo(pos, tam),
			gerenciador_grafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
			delayAtaque(2000),
			bravo(false),
			muitoBravo(false),
			enfurecido(false)

		{
			sprite.setPosition(pos);
			inicializaAnimacoes();
			voador = true;
			vel = Vector2f(0.25f, 0.4f);
			distanciaAlvo = 30.0f;
			ALCANCE_Y = 900.0f;
			corpo.setFillColor(sf::Color::Red);
			vida = VIDA_MAX;
			dano = 0.35f;
			distanciaAtaqueX = 900.0f;
			distanciaAtaqueY = 150.0f;

			healthBar.setScale(vida / 500.0f, 0.2f);

		}

		Chefao::~Chefao()
		{
		}

		void Chefao::atacar() {

			auto agora = std::chrono::steady_clock::now();
			auto diferenca = std::chrono::duration_cast<std::chrono::milliseconds>(agora - ultimoAtaque).count();

			if (!morto && concluida && diferenca >= delayAtaque) {
				Projetil* novoProjetil = new Projetil(Vector2f(corpo.getPosition().x, corpo.getPosition().y + 35.0f), Vector2f(40.0f, 65.0f), direita);
				projeteis.push_back(novoProjetil);

				ultimoAtaque = agora;

			}
			parado = false;

			animacao = 3;

		}

		void Chefao::atualizaProjeteis()
		{

			for (int i = 0; i < projeteis.size(); i++)
			{
				if (projeteis.at(i))
				{
					if (jogador1->getCorpo().getGlobalBounds().intersects(projeteis.at(i)->getCorpo().getGlobalBounds()))
					{
						projeteis.at(i)->setColidiu(true);
						jogador1->tomarDano(projeteis.at(i)->getDano());
					}

					projeteis.at(i)->atualizar();
					//gerenciador_grafico->desenhaHitbox(projeteis.at(i)->getCorpo());
					gerenciador_grafico->desenhaSprite(projeteis.at(i)->getSprite());

					if (projeteis.at(i)->getExplodiu())
						projeteis.at(i) = nullptr;
				}
			}

			atualizaVida();
			atualizarPortais();

		}

		void Chefao::atualizaVida() {
			if ((vida > VIDA_MAX * 3 / 4) && (!bravo && !muitoBravo && !enfurecido)) {
				bravo = true;
				delayAtaque = 1800.0f;

			}
			else if ((vida <= VIDA_MAX * 3 / 4 && vida > VIDA_MAX / 2) && (bravo && !muitoBravo && !enfurecido)) {
				muitoBravo = true;
				delayAtaque = 1500.0f;
				teleportar();

			}
			else if ((vida <= VIDA_MAX / 2 && vida > VIDA_MAX / 4) && (muitoBravo && !enfurecido)) {
				enfurecido = true;
				delayAtaque = 1200.0f;
				teleportar();

			}
			else if ((vida <= VIDA_MAX / 4) && enfurecido) {
				enfurecido = false;
				delayAtaque = 900.0f;
				teleportar();

			}
		}

		void Chefao::atualizarPortais()
		{
			for (int i = 0; i < portais.size(); i++)
			{
				if (portais.at(i))
				{
					gerenciador_grafico->desenhaSprite(portais.at(i)->atualizarAnimacao());

					if (portais.at(i)->getTerminou())
						portais.at(i) = nullptr;
				}
			}
		}

		void Chefao::atualizar()
		{
			Vector2f posJogador = jogador1->getCorpo().getPosition();
			Vector2f posInimigo = corpo.getPosition();

			if (!parado)
			{
				if (fabs(posJogador.x - posInimigo.x) <= ALCANCE_X && fabs(posJogador.y - posInimigo.y) <= ALCANCE_Y)
				{
					perseguirJogador(posJogador, posInimigo);
				}
				else
				{
					moveAleatorio();
				}
			}

			atualizaProjeteis();
			atualizaVida();
			atualizarPortais();

			atualizarAnimacao();

			if (vida <= 0.0f)
			{
				parado = true;
				animacao = 2;
				voador = false;
				//corpo.move(0.0f, 0.9f);
			}


		}

		void Chefao::teleportar()
		{
			teleportando = true;

			int lado1;
			int lado2;

			if (rand() % 2)
				lado1 = 1;
			else
				lado1 = -1;

			if (rand() % 2)
				lado2 = 1;
			else
				lado2 = -1;

			Portal* p1 = new Entidades::Portal(Vector2f(corpo.getPosition().x - 40.0f, corpo.getPosition().y - 20.0f), Vector2f(50.0f, 50.0f));
			portais.push_back(p1);

			corpo.move(lado1 * (300.0f + rand() % 300), lado2 * (rand() % 300));

			Portal* p2 = new Entidades::Portal(Vector2f(corpo.getPosition().x - 40.0f, corpo.getPosition().y - 20.0f), Vector2f(50.0f, 50.0f));
			portais.push_back(p2);
		}


		void Chefao::inicializaAnimacoes()
		{
			Animacao animacaoVoando;
			Animacao animacaoTomarDano;
			Animacao animacaoMorte;
			Animacao animacaoAtacar;
			Animacao animacaoParado;

			sf::Texture texture;
			int pedacoWidth = 64; //Largura
			int pedacoHeight = 64; //Altura

			sf::Vector2f spriteOrigin((corpo.getSize().x / 2.9f), (corpo.getSize().y / 4.0f) - 10.0f);
			sprite.setOrigin(spriteOrigin);

			//VOANDO 0 
			if (!texture.loadFromFile("Assets/Monsters/Chefao/Flight.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoVoando.addFrame(pedacoTexture);
			}

			//TOMAR DANO 1
			if (!texture.loadFromFile("Assets/Monsters/Chefao/TakeHit.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoTomarDano.addFrame(pedacoTexture);
			}

			animacaoTomarDano.setAnimationSpeed(25.0f);

			//MORTE 2
			if (!texture.loadFromFile("Assets/Monsters/Chefao/Death.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoMorte.addFrame(pedacoTexture);
			}

			animacaoMorte.setAnimationSpeed(90.0f);

			//ATACAR 3
			if (!texture.loadFromFile("Assets/Monsters/Chefao/Attack.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar.addFrame(pedacoTexture);
			}

			animacaoAtacar.setAnimationSpeed(50.0f);

			//PARADO 4 
			if (!texture.loadFromFile("Assets/Monsters/Chefao/Flight.png")) {
				exit(1);
			}

			for (int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoParado.addFrame(pedacoTexture);
			}

			animacoes.push_back(animacaoVoando);
			animacoes.push_back(animacaoTomarDano);
			animacoes.push_back(animacaoMorte);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoParado);

		}

		void Chefao::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}

		float Chefao::getVida()
		{
			return VIDA_MAX;
		}

		float Chefao::getSize()
		{
			return SIZE;
		}


	}
}