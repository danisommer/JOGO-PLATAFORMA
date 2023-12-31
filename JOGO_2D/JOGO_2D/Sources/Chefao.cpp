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
			delayAtaque(2000),
			bravo(false),
			muitoBravo(false),
			enfurecido(false),
			morrendo(false)

		{
			sprite.setPosition(pos);
			inicializaAnimacoes();
			voador = true;
			vel = Vector2f(0.25f, 0.4f);
			distanciaAlvo = 30.0f;
			ALCANCE_Y = 1200.0f;
			ALCANCE_X = 1200.0f;
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

		void Chefao::atacar(int jogador) {

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
				if (projeteis.at(i) && !projeteis.at(i)->getColidiu())
				{
					if (jogador1)
					{
						if (jogador1->getCorpo()->getGlobalBounds().intersects(projeteis.at(i)->getCorpo()->getGlobalBounds()))
						{
							projeteis.at(i)->setColidiu(true);
							jogador1->tomarDano(projeteis.at(i)->getDano());
						}
					}

					if (jogador2)
					{
						if (jogador2->getCorpo()->getGlobalBounds().intersects(projeteis.at(i)->getCorpo()->getGlobalBounds()))
						{
							projeteis.at(i)->setColidiu(true);
							jogador2->tomarDano(projeteis.at(i)->getDano());
						}
					}

					projeteis.at(i)->atualizar();

					if (projeteis.at(i)->getExplodiu())
						projeteis.at(i) = nullptr;
				}
			}
		}

		void Chefao::atualizaVida() {
			if ((vida > VIDA_MAX * 3 / 4) && (!bravo && !muitoBravo && !enfurecido)) {
				bravo = true;
				delayAtaque = 1800;

			}
			else if ((vida <= VIDA_MAX * 3 / 4 && vida > VIDA_MAX / 2) && (bravo && !muitoBravo && !enfurecido)) {
				muitoBravo = true;
				delayAtaque = 1500;
				teleportar();

			}
			else if ((vida <= VIDA_MAX / 2 && vida > VIDA_MAX / 4) && (muitoBravo && !enfurecido)) {
				enfurecido = true;
				delayAtaque = 1200;
				teleportar();

			}
			else if ((vida <= VIDA_MAX / 4) && enfurecido) {
				enfurecido = false;
				delayAtaque = 900;
				teleportar();

			}
		}

		void Chefao::atualizarPortais()
		{
			for (int i = 0; i < portais.size(); i++)
			{
				if (portais.at(i))
				{
					portais.at(i)->atualizar();

					if (portais.at(i)->getTerminou())
						portais.at(i) = nullptr;
				}
			}
		}

		void Chefao::atualizar()
		{
			if (!parado)
			{
				Vector2f posJogador;
				Vector2f posInimigo = corpo.getPosition();

				if (jogador1)
				{
					posJogador = jogador1->getCorpo()->getPosition();
				}
				else if (jogador2)
				{
					posJogador = jogador2->getCorpo()->getPosition();
				}
				else
				{
					moveAleatorio();
					return;
				}

				float distanciaJogador1 = sqrt(pow(posJogador.x - posInimigo.x, 2) + pow(posJogador.y - posInimigo.y, 2));

				if (jogador2)
				{
					Vector2f posJogador2 = jogador2->getCorpo()->getPosition();
					float distanciaJogador2 = sqrt(pow(posJogador2.x - posInimigo.x, 2) + pow(posJogador2.y - posInimigo.y, 2));

					if (distanciaJogador1 <= ALCANCE_X && distanciaJogador1 <= ALCANCE_Y &&
						(distanciaJogador1 < distanciaJogador2 || distanciaJogador2 > ALCANCE_X || distanciaJogador2 > ALCANCE_Y))
					{
						perseguirJogador(posJogador, posInimigo);
					}
					else if (distanciaJogador2 <= ALCANCE_X && distanciaJogador2 <= ALCANCE_Y)
					{
						perseguirJogador(posJogador2, posInimigo);
					}
					else
					{
						moveAleatorio();
					}
				}
				else
				{
					if (distanciaJogador1 <= ALCANCE_X && distanciaJogador1 <= ALCANCE_Y)
					{
						perseguirJogador(posJogador, posInimigo);
					}
					else
					{
						moveAleatorio();
					}
				}
			}

			atualizaProjeteis();
			atualizaVida();
			atualizarPortais();

			atualizarAnimacao();

			desenharSprite();

			if (vida <= 0.0f)
			{
				parado = true;
				animacao = 2;
				voador = false;

				chefaoMorreu = true;
				posChefao = corpo.getPosition();
			}
		}

		void Chefao::limparArquivo(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_chefao.txt", std::ofstream::out);

			if (arquivo.is_open())
			{
				arquivo.close();
			}
		}

		void Chefao::salvar(int save)
		{
			std::ofstream arquivo("Saves/save" + to_string(save) + "_chefao.txt", std::ios::app);

			if (arquivo.is_open())
			{
				arquivo << "Vida: " << vida << "\n";
				arquivo << "Posicao: " << corpo.getPosition().x << " " << corpo.getPosition().y << "\n";

				arquivo.close();
			}
		}

		void Chefao::teleportar()
		{
			teleportando = true;

			float lado1;
			float lado2 = -1.0f;

			if (rand() % 2)
				lado1 = 1.0;
			else
				lado1 = -1.0;

			Obstaculos::Portal* p1 = new Entidades::Obstaculos::Portal(Vector2f(corpo.getPosition().x - 40.0f, corpo.getPosition().y - 20.0f), Vector2f(50.0f, 50.0f), false, false);
			portais.push_back(p1);

			corpo.move(lado1 * (float)(300.0f + rand() % 400), lado2 * (float)(rand() % 300));

			Obstaculos::Portal* p2 = new Entidades::Obstaculos::Portal(Vector2f(corpo.getPosition().x - 40.0f, corpo.getPosition().y - 20.0f), Vector2f(50.0f, 50.0f), false, false);
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

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoVoando.addFrame(pedacoTexture);
			}

			//TOMAR DANO 1
			if (!texture.loadFromFile("Assets/Monsters/Chefao/TakeHit.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoTomarDano.addFrame(pedacoTexture);
			}

			animacaoTomarDano.setAnimationSpeed(20.0f);

			//MORTE 2
			if (!texture.loadFromFile("Assets/Monsters/Chefao/Death.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
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

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
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

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
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