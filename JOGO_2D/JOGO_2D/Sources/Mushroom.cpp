#include "Mushroom.hpp"

#include "iostream"
#define VIDA_MAX 100.0f
#define SIZE 1.8f


using namespace std;

namespace Entidades
{
	namespace Personagens
	{

		Mushroom::Mushroom(Vector2f pos, Vector2f tam) :
			Inimigo(pos, tam),
			forcaVeneno(0.035f),
			tempoEnvenenamento(700),
			venenoEspecial(false)

		{
			sprite.setPosition(pos);
			inicializaAnimacoes();
			voador = false;
			vel = Vector2f(0.35f, 0.1f);
			distanciaAlvo = 30.0f;
			corpo.setFillColor(sf::Color::Red);
			healthBar.setScale(vida / 500.0f, 0.2f);
			dano = 0.2f;
			vida = VIDA_MAX;
		
			if (rand() % 3 == 0)
			{
				venenoEspecial = true;
				sprite.setColor(Color{235, 255, 0});
			}
		}

		Mushroom::~Mushroom()
		{
		}

		void Mushroom::atacar(int jogador)
		{
			if (!morto)
				if (concluida)
				{
					if (jogador == 1)
					{
						jogador1->tomarDano(dano);
						envenenar(1);
					}
					else if (jogador == 2)
					{
						jogador2->tomarDano(dano);
						envenenar(2);
					}
				}

			animacao = 3;
		}

		void Mushroom::envenenar(int jogador)
		{
			if (venenoEspecial)
			{
				forcaVeneno = 0.04f;
				corJogador = Color::Green;
			}
			else
			{
				forcaVeneno = 0.035f;
				corJogador = Color{100, 255, 100};

			}
			if (jogador == 1) 
			{
				jogador1->setEnvenenado(true, tempoEnvenenamento, forcaVeneno, corJogador);
			}
			else if (jogador == 2)
			{
				jogador2->setEnvenenado(true, tempoEnvenenamento, forcaVeneno, corJogador);
			}
		}

		void Mushroom::inicializaAnimacoes()
		{
			Animacao animacaoAndando;
			Animacao animacaoTomarDano;
			Animacao animacaoMorte;
			Animacao animacaoAtacar;
			Animacao animacaoParado;

			sf::Texture texture;
			int pedacoWidth = 150; //Largura
			int pedacoHeight = 150; //Altura

			sf::Vector2f spriteOrigin(corpo.getSize().x / 0.5f, corpo.getSize().y / 1.6f);
			sprite.setOrigin(spriteOrigin);

			//WALK 0 
			if (!texture.loadFromFile("Assets/Monsters/Mushroom/Run.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAndando.addFrame(pedacoTexture);
			}

			//TOMAR DANO 1
			if (!texture.loadFromFile("Assets/Monsters/Mushroom/TakeHit.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoTomarDano.addFrame(pedacoTexture);
			}

			animacaoTomarDano.setAnimationSpeed(25.0f);

			//MORTE 2
			if (!texture.loadFromFile("Assets/Monsters/Mushroom/Death.png")) {
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
			if (!texture.loadFromFile("Assets/Monsters/Mushroom/Attack.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoAtacar.addFrame(pedacoTexture);
			}

			animacaoAtacar.setAnimationSpeed(12.0f);

			//PARADO 4
			if (!texture.loadFromFile("Assets/Monsters/Mushroom/Idle.png")) {
				exit(1);
			}

			for (unsigned int x = 0; x < texture.getSize().x; x += pedacoWidth) {
				sf::IntRect pedacoRect(x, 0, pedacoWidth, pedacoHeight);
				sf::Texture pedacoTexture;
				pedacoTexture.loadFromImage(texture.copyToImage(), pedacoRect);
				animacaoParado.addFrame(pedacoTexture);
			}

			animacoes.push_back(animacaoAndando);
			animacoes.push_back(animacaoTomarDano);
			animacoes.push_back(animacaoMorte);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoParado);

		}
		void Mushroom::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}

		float Mushroom::getVida()
		{
			return VIDA_MAX;
		}
	
		float Mushroom::getSize()
		{
			return SIZE;
		}
		void Mushroom::atualizar()
		{
			Vector2f posJogador = jogador1->getCorpo()->getPosition();
			Vector2f posJogador2 = jogador2->getCorpo()->getPosition();
			Vector2f posInimigo = corpo.getPosition();

			if (!parado)
			{
				float distanciaJogador1 = sqrt(pow(posJogador.x - posInimigo.x, 2) + pow(posJogador.y - posInimigo.y, 2));
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

			atualizarAnimacao();

			if (vida <= 0.0f)
			{
				parado = true;
				animacao = 2;
				voador = false;
				//corpo.move(0.0f, 0.9f);
			}


		}
}

}