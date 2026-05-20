#include "Chefao.hpp"
#include "OlhoVoador.hpp"
#include "Gerenciador_Recursos.hpp"
#include "Mundo.hpp"
#include "Camera.hpp"
#include "Jogador.hpp"
#include "iostream"
#include <cmath>

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
			estagio(Estagio::E1),
			morrendo(false),
			realizandoSlam(false),
			posYOriginal(0.0f)
		{
			sprite.setPosition(pos);
			inicializaAnimacoes();
			voador = true;
			vel = Vector2f(1.5f, 1.9f);
			distanciaAlvo = 30.0f;
			ALCANCE_Y = 1200.0f;
			ALCANCE_X = 1200.0f;
			corpo.setFillColor(sf::Color::Red);
			vida = VIDA_MAX;
			vidaMaxima = vida;
			dano = 0.35f;
			distanciaAtaqueX = 900.0f;
			distanciaAtaqueY = 150.0f;

			healthBar.setScale(vida / 500.0f, 0.2f);

			ultimoAtaque = std::chrono::steady_clock::now();
			ultimoSlam   = std::chrono::steady_clock::now();
			ultimoSpawn  = std::chrono::steady_clock::now();
		}

		Chefao::~Chefao()
		{
		}

		// Encontra o jogador mais proximo vivo.
		static Jogador* jogadorMaisProximo(Mundo* mundo, const sf::Vector2f& pos)
		{
			Jogador* melhor = nullptr;
			float distMin = 1e9f;
			for (int i = 0; i < 2; ++i)
			{
				Jogador* j = mundo ? mundo->getJogador(i) : nullptr;
				if (!j) continue;
				const sf::Vector2f d = j->getCorpo()->getPosition() - pos;
				const float dist = std::sqrt(d.x * d.x + d.y * d.y);
				if (dist < distMin) { distMin = dist; melhor = j; }
			}
			return melhor;
		}

		void Chefao::dispararProjetil(bool homingAtivado)
		{
			Projetil* p = new Projetil(
				Vector2f(corpo.getPosition().x, corpo.getPosition().y + 35.0f),
				Vector2f(40.0f, 65.0f),
				direita);

			if (homingAtivado && mundo)
			{
				Jogador* alvo = jogadorMaisProximo(mundo, corpo.getPosition());
				if (alvo)
				{
					// giro lento: 0.025 rad/frame (~4s para dar volta completa)
					// O jogador consegue desviar dando uma grande volta
					p->setHoming(alvo->getCorpo()->getPosition(), 0.025f, 420);
				}
			}

			projeteis.push_back(p);
		}

		void Chefao::atacar(int jogador)
		{
			auto agora = std::chrono::steady_clock::now();
			auto dif = std::chrono::duration_cast<std::chrono::milliseconds>(agora - ultimoAtaque).count();

			if (!morto && concluida && dif >= delayAtaque)
			{
				// Estagio 3+: projetil guiado. Estagio 1-2: projetil reto.
				const bool guiado = (estagio >= Estagio::E3);
				dispararProjetil(guiado);
				ultimoAtaque = agora;

				if (mundo && mundo->getCamera())
					mundo->getCamera()->dispararTremor(15, 7.0f);
			}
			parado = false;
			animacao = 3;
		}

		void Chefao::realizarSlam()
		{
			// O slam dura ~40 frames: desce rapido, volta e atordoa jogadores proximos.
			if (!realizandoSlam)
			{
				realizandoSlam = true;
				posYOriginal = corpo.getPosition().y;
			}

			// Desce 6px por frame ate 120px abaixo
			const float alvo = posYOriginal + 120.0f;
			if (corpo.getPosition().y < alvo)
			{
				corpo.move(0.0f, 10.0f);
			}
			else
			{
				// Chegou ao fundo: tremor forte + atordoar jogadores
				corpo.setPosition(corpo.getPosition().x, posYOriginal);
				realizandoSlam = false;
				ultimoSlam = std::chrono::steady_clock::now();

				if (mundo && mundo->getCamera())
					mundo->getCamera()->dispararTremor(35, 18.0f);

				for (int i = 0; i < 2; ++i)
				{
					Jogador* j = mundo ? mundo->getJogador(i) : nullptr;
					if (!j) continue;
					const sf::Vector2f d = j->getCorpo()->getPosition() - corpo.getPosition();
					if (std::fabs(d.x) < 400.0f && std::fabs(d.y) < 250.0f)
						j->setAtordoado(true, 180); // 3 segundos
				}
			}
		}

		void Chefao::spawnVoadores()
		{
			if (!mundo) return;

			// Abre um portal e spawna 1-2 OlhoVoador proximos ao chefao
			const int qtd = (estagio >= Estagio::E4) ? 2 : 1;
			for (int i = 0; i < qtd; ++i)
			{
				const float ox = corpo.getPosition().x + (i == 0 ? -120.0f : 120.0f);
				const float oy = corpo.getPosition().y - 60.0f;

				// Portal visual
				auto* portal = new Obstaculos::Portal(
					Vector2f(ox - 25.0f, oy - 25.0f), Vector2f(50.0f, 50.0f), false, false);
				portais.push_back(portal);

				// Inimigo enfileirado no Mundo
				auto* voador = new OlhoVoador(
					Vector2f(ox, oy), Vector2f(40.0f, 50.0f), false);
				mundo->enfileirarInimigo(voador);
			}

			ultimoSpawn = std::chrono::steady_clock::now();
		}

		void Chefao::atualizaProjeteis()
		{
			Jogador* j1 = mundo ? mundo->getJogador(0) : nullptr;
			Jogador* j2 = mundo ? mundo->getJogador(1) : nullptr;

			// Pega posicao atual do alvo mais proximo para projetos guiados
			Jogador* alvoPrincipal = jogadorMaisProximo(mundo, corpo.getPosition());

			for (int i = 0; i < (int)projeteis.size(); i++)
			{
				Projetil* p = projeteis.at(i);
				if (!p || p->getColidiu()) continue;

				// Atualiza posicao do alvo (safe: alvoPrincipal pode morrer)
				if (p->isGuiado() && alvoPrincipal)
					p->atualizarAlvo(alvoPrincipal->getCorpo()->getPosition());

				auto verificar = [&](Jogador* j)
				{
					if (!j) return;
					if (j->getCorpo()->getGlobalBounds().intersects(p->getCorpo()->getGlobalBounds()))
					{
						p->setColidiu(true);
						j->tomarDano(p->getDano(),
							(j->getPos().x >= p->getPos().x) ? 1 : -1);
					}
				};
				verificar(j1);
				verificar(j2);

				p->atualizar();

				if (p->getExplodiu())
					projeteis.at(i) = nullptr;
			}
		}

		void Chefao::atualizaVida()
		{
			// Usa vidaMaxima (inclui escalonamento da fase) em vez de VIDA_MAX fixo
			const float vm = (vidaMaxima > 0.0f) ? vidaMaxima : VIDA_MAX;

			auto agora = std::chrono::steady_clock::now();
			const long long msSlamDec = std::chrono::duration_cast<std::chrono::milliseconds>(
				agora - ultimoSlam).count();
			const long long msSpawnDec = std::chrono::duration_cast<std::chrono::milliseconds>(
				agora - ultimoSpawn).count();

			if (vida > vm * 3.0f / 4.0f)
			{
				// Estagio 1: comportamento padrao
				if (estagio != Estagio::E1)
				{
					estagio = Estagio::E1;
					delayAtaque = 2000;
				}
			}
			else if (vida > vm / 2.0f)
			{
				// Estagio 2: mais rapido, spawna voadores a cada 8s
				if (estagio != Estagio::E2)
				{
					estagio = Estagio::E2;
					delayAtaque = 1500;
					teleportar();
				}
				if (msSpawnDec >= 8000)
					spawnVoadores();
			}
			else if (vida > vm / 4.0f)
			{
				// Estagio 3: projetil guiado, slam a cada 6s, spawn a cada 6s
				if (estagio != Estagio::E3)
				{
					estagio = Estagio::E3;
					delayAtaque = 1100;
					teleportar();
				}
				if (msSlamDec >= 6000 && !realizandoSlam)
					realizarSlam();
				if (msSpawnDec >= 6000)
					spawnVoadores();
			}
			else
			{
				// Estagio 4: frenético - slam frequente, spawn frequente, projetil guiado
				if (estagio != Estagio::E4)
				{
					estagio = Estagio::E4;
					delayAtaque = 750;
					teleportar();
					// Tint vermelho intenso para sinalizar furia maxima
					sprite.setColor(sf::Color(255, 80, 80));
				}
				if (msSlamDec >= 4000 && !realizandoSlam)
					realizarSlam();
				if (msSpawnDec >= 4500)
					spawnVoadores();
			}
		}

		void Chefao::atualizarPortais()
		{
			for (int i = 0; i < (int)portais.size(); i++)
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
			Jogador* j1 = mundo ? mundo->getJogador(0) : nullptr;
			Jogador* j2 = mundo ? mundo->getJogador(1) : nullptr;

			if (realizandoSlam)
			{
				realizarSlam();
				atualizaProjeteis();
				atualizarPortais();
				atualizarAnimacao();
				desenharSprite();
				return;
			}

			if (!parado)
			{
				Vector2f posJogador;
				Vector2f posInimigo = corpo.getPosition();
				bool temAlvo = false;

				if (j1) { posJogador = j1->getCorpo()->getPosition(); temAlvo = true; }
				else if (j2) { posJogador = j2->getCorpo()->getPosition(); temAlvo = true; }

				if (!temAlvo)
				{
					moveAleatorio();
				}
				else
				{
					float dist1 = std::sqrt(std::pow(posJogador.x - posInimigo.x, 2)
						+ std::pow(posJogador.y - posInimigo.y, 2));

					if (j2)
					{
						Vector2f pos2 = j2->getCorpo()->getPosition();
						float dist2 = std::sqrt(std::pow(pos2.x - posInimigo.x, 2)
							+ std::pow(pos2.y - posInimigo.y, 2));

						if (dist1 <= ALCANCE_X && dist1 <= ALCANCE_Y &&
							(dist1 < dist2 || dist2 > ALCANCE_X || dist2 > ALCANCE_Y))
							perseguirJogador(posJogador, posInimigo);
						else if (dist2 <= ALCANCE_X && dist2 <= ALCANCE_Y)
							perseguirJogador(pos2, posInimigo);
						else
							moveAleatorio();
					}
					else
					{
						if (dist1 <= ALCANCE_X && dist1 <= ALCANCE_Y)
							perseguirJogador(posJogador, posInimigo);
						else
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
				if (mundo)
					mundo->setChefaoMorreu(true, corpo.getPosition());
			}
		}

		void Chefao::limparArquivo(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_chefao.txt", std::ofstream::out);
			if (arquivo.is_open()) arquivo.close();
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

			if (mundo && mundo->getCamera())
				mundo->getCamera()->dispararTremor(25, 12.0f);

			float lado1 = (rand() % 2) ? 1.0f : -1.0f;

			auto* p1 = new Obstaculos::Portal(
				Vector2f(corpo.getPosition().x - 40.0f, corpo.getPosition().y - 20.0f),
				Vector2f(50.0f, 50.0f), false, false);
			portais.push_back(p1);

			corpo.move(lado1 * (float)(300 + rand() % 400), -1.0f * (float)(rand() % 300));

			auto* p2 = new Obstaculos::Portal(
				Vector2f(corpo.getPosition().x - 40.0f, corpo.getPosition().y - 20.0f),
				Vector2f(50.0f, 50.0f), false, false);
			portais.push_back(p2);
		}

		void Chefao::inicializaAnimacoes()
		{
			auto* recursos = Gerenciadores::Gerenciador_Recursos::getGerenciador();
			const std::string base = "Assets/Monsters/Chefao/";

			const int pedacoWidth = 64;
			const int pedacoHeight = 64;

			sf::Vector2f spriteOrigin((corpo.getSize().x / 2.9f), (corpo.getSize().y / 4.0f) - 10.0f);
			sprite.setOrigin(spriteOrigin);

			Animacao animacaoVoando, animacaoTomarDano, animacaoMorte, animacaoAtacar, animacaoParado;

			animacaoVoando.fatiarSpritesheet(recursos->getTextura(base + "Flight.png"), pedacoWidth, pedacoHeight);
			animacaoTomarDano.fatiarSpritesheet(recursos->getTextura(base + "TakeHit.png"), pedacoWidth, pedacoHeight);
			animacaoTomarDano.setAnimationSpeed(20.0f);
			animacaoMorte.fatiarSpritesheet(recursos->getTextura(base + "Death.png"), pedacoWidth, pedacoHeight);
			animacaoMorte.setAnimationSpeed(90.0f);
			animacaoAtacar.fatiarSpritesheet(recursos->getTextura(base + "Attack.png"), pedacoWidth, pedacoHeight);
			animacaoAtacar.setAnimationSpeed(50.0f);
			animacaoParado.fatiarSpritesheet(recursos->getTextura(base + "Flight.png"), pedacoWidth, pedacoHeight);

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

		float Chefao::getVida() { return VIDA_MAX; }
		float Chefao::getSize() { return SIZE; }
	}
}
