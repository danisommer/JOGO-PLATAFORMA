#include "Jogador.hpp"
#include "Gerenciador_Recursos.hpp"
#include "Configuracao.hpp"
#include "Mundo.hpp"
#include "ArvoreHabilidades.hpp"
#include "Camera.hpp"
#include <iostream>

namespace Entidades
{
	namespace Personagens
	{
		namespace
		{
			// Constantes do jogador, em unidades por passo de 1/60 s.
			constexpr float VIDA_MAX = 100.0f;
			constexpr float VEL_JOGADOR = 7.0f;    // velocidade horizontal
			constexpr float FORCA_PULO = -20.0f;   // impulso inicial do pulo
			constexpr float DANO_JOGADOR = 18.0f;  // dano por golpe
		}

		Jogador::Jogador(const Vector2f pos, const Vector2f tam, int indice) :
			Personagem(),
			indiceJogador(indice),
			jumpStrength(FORCA_PULO),
			n_frames(0),
			count(0),
			lado(1),
			anterior(0),
			iteracoes(0),
			ataque(0),
			regiaoAtaque(),
			atacando(false),
			vidaAnterior(),
			tempoDecorridoLentidao(0),
			tempoDecorridoVeneno(0),
			tempoVeneno(0),
			tempoLentidao(0),
			atordoado(false),
			tempoAtordoado(0),
			tempoDecorridoAtordoado(0),
			atacandoAtivo(false),
			ataquePressionadoAnterior(false),
			tamanhoCorpo(tam),
			concluiuFase(false),
			puloDuploDisponivel(false),
			puloDuploLiberado(false),
			puloPressionadoAnterior(false),
			fatorArmadura(1.0f),
			vampiro(false),
			curaVampiro(5.0f),
			bonusAlcance(0.0f)
		{
			dano = DANO_JOGADOR;
			vida = VIDA_MAX;
			vidaMaxima = VIDA_MAX;
			sprite.setPosition(pos);
			corpo.setSize(tam);
			corpo.setPosition(pos);
			corpo.setFillColor(sf::Color::Red);
			vel = Vector2f(VEL_JOGADOR, VEL_JOGADOR);
			healthBar.setScale(vida / 500.0f, 0.2f);
			inicializaAnimacoes();
			inicializaTeclas();
		}

		Jogador::~Jogador() = default;

		void Jogador::atualizar()
		{
			if (!isJumping)
				puloDuploDisponivel = puloDuploLiberado;

			if (animacao != 2)
			{
				if (envenenado)
				{
					if (tempoDecorridoVeneno < tempoVeneno)
					{
						sprite.setColor(corEnvenenado);
						vida -= forcaVeneno;
						tempoDecorridoVeneno++;

						if (mundo && mundo->getCamera())
							mundo->getCamera()->setEnvenenado(true);
					}
					else
					{
						sprite.setColor(Color{255, 255, 255});
						tempoDecorridoVeneno = 0;
						envenenado = false;
						if (mundo && mundo->getCamera())
							mundo->getCamera()->setEnvenenado(false);
					}
				}

				if (lento)
				{
					if (tempoDecorridoLentidao < tempoLentidao)
					{
						vel.x = forcaLentidao;
						jumpStrength = forcaPulo;
						tempoDecorridoLentidao++;
						animacoes.at(0).setAnimationSpeed(80.0f);
					}
					else
					{
						jumpStrength = FORCA_PULO;
						vel.x = VEL_JOGADOR;
						tempoDecorridoLentidao = 0;
						lento = false;
						animacoes.at(0).setAnimationSpeed(25.0f);
					}
				}

				if (atordoado)
				{
					if (tempoDecorridoAtordoado < tempoAtordoado)
					{
						// Efeito visual: piscar amarelo durante o stun
						const int ciclo = tempoDecorridoAtordoado % 12;
						sprite.setColor(ciclo < 6
							? sf::Color(255, 255, 80)
							: sf::Color(255, 255, 255));
						vel.x = 0.0f;
						tempoDecorridoAtordoado++;
					}
					else
					{
						sprite.setColor(sf::Color(255, 255, 255));
						tempoDecorridoAtordoado = 0;
						atordoado = false;
					}
				}
			}

			if (isJumping)
				animacao = 6;

			if (vida <= 0.0f)
			{
				vel.x = 0.0f;
				vel.y = 0.0f;
				jumpStrength = 0.0f;

				animacao = 2;
				if (concluida)
					morrer();
			}

			atualizarAnimacao(animacao);
		}

		void Jogador::atualizarAnimacao(int animacao)
		{
			if (animacao != anterior)
			{
				count = 0;
				iteracoes = 0;
				concluida = false;
			}

			anterior = animacao;

			animacaoAtual = &animacoes[animacao];

			n_frames = animacaoAtual->getNumFrames();

			if (iteracoes > animacaoAtual->getAnimationSpeed())
			{
				if (count < n_frames - 1)
				{
					count++;
					concluida = false;
				}
				else
				{
					count = 0;
					concluida = true;

					if (ataque == 1)
						ataque = 0;
					else
						ataque = 1;
				}

				iteracoes = 0;
			}
			iteracoes++;

			animacaoAtual->aplicar(sprite, count);
			sprite.setScale(lado * 2.5f, 2.5f);
			sprite.setPosition(corpo.getPosition().x + 15.0f, corpo.getPosition().y);

			desenharSprite();
			desenharEfeitosAtivos();
		}

		void Jogador::desenharEfeitosAtivos()
		{
			const sf::Font& fonte = Gerenciadores::Gerenciador_Recursos::getGerenciador()
				->getFonte("Menu/antiquity-print.ttf");

			// Converte frames restantes em segundos (60 fps).
			float offsetY = -55.0f;
			const float linhaH = 18.0f;

			auto desenharContador = [&](const std::string& label, sf::Color cor)
			{
				sf::Text t;
				t.setFont(fonte);
				t.setString(label);
				t.setCharacterSize(14);
				t.setFillColor(cor);
				t.setOutlineColor(sf::Color::Black);
				t.setOutlineThickness(1.0f);
				t.setPosition(corpo.getPosition().x - 10.0f, corpo.getPosition().y + offsetY);
				Gerenciadores::Gerenciador_Grafico::getGerenciador()->desenhaTexto(t);
				offsetY -= linhaH;
			};

			if (envenenado && tempoVeneno > 0)
			{
				const float segs = static_cast<float>(tempoVeneno - tempoDecorridoVeneno) / 60.0f;
				char buf[32];
				std::snprintf(buf, sizeof(buf), "Veneno %.1fs", segs);
				desenharContador(buf, sf::Color(180, 80, 220));
			}

			if (lento && tempoLentidao > 0)
			{
				const float segs = static_cast<float>(tempoLentidao - tempoDecorridoLentidao) / 60.0f;
				char buf[32];
				std::snprintf(buf, sizeof(buf), "Lento %.1fs", segs);
				desenharContador(buf, sf::Color(80, 160, 255));
			}

			if (atordoado && tempoAtordoado > 0)
			{
				const float segs = static_cast<float>(tempoAtordoado - tempoDecorridoAtordoado) / 60.0f;
				char buf[32];
				std::snprintf(buf, sizeof(buf), "Atordoado %.1fs", segs);
				desenharContador(buf, sf::Color(255, 220, 40));
			}
		}

		void Jogador::setAnimacao(int anim)
		{
			animacaoAtual = &animacoes[anim];
		}

		const Vector2f Jogador::getRegiaoAtaque() const 
		{
			return regiaoAtaque;
		}

		const float Jogador::getDano() const
		{
			return dano;
		}

		const bool Jogador::getAtacando() const
		{
			return atacando;
		}

		float Jogador::getVida()
		{
			return VIDA_MAX;
		}

		void Jogador::setEnvenenado(bool veneno, int tempo, float intensidade, sf::Color cor)
		{
			envenenado = veneno;
			tempoVeneno = tempo;
			forcaVeneno = intensidade;
			corEnvenenado = cor;
		}

		void Jogador::setLento(bool lentidao, int tempo, float fL, float fP)
		{
			lento = lentidao;
			tempoLentidao = tempo;
			tempoDecorridoLentidao = 0;
			forcaLentidao = fL;
			forcaPulo = fP;
		}

		void Jogador::setAtordoado(bool atorz, int tempo)
		{
			atordoado = atorz;
			tempoAtordoado = tempo;
			tempoDecorridoAtordoado = 0;
		}

		bool Jogador::getAtordoado() const
		{
			return atordoado;
		}


		void Jogador::mover(bool direita, bool esquerda)
		{
			if (direita)
			{
				velocity.x = vel.x;
				animacao = 0;
				lado = 1;
			}
			else if (esquerda)
			{
				velocity.x = -vel.x;
				animacao = 0;
				lado = -1;
			}

			if (!direita && !esquerda)
			{
				animacao = 5;
				velocity.x = 0.0f;
			}

			corpo.move(velocity.x, 0.0f);
		}

		void Jogador::bater(bool batendo)
		{
			if (batendo && !isJumping)
			{
				if (Keyboard::isKeyPressed(teclas.at(0)))
					lado = -1;
				if (Keyboard::isKeyPressed(teclas.at(1)))
					lado = 1;

				const float alcance = 65.0f * (1.0f + bonusAlcance);
				regiaoAtaque = Vector2f(corpo.getPosition().x + (alcance * lado), corpo.getPosition().y);
				atacando = true;

				velocity.x = 0.0f;

				if (ataque == 1)
					animacao = 4;
				else
					animacao = 3;
			}
			else
			{
				regiaoAtaque = Vector2f();
				atacando = false;
			}

		}

		void Jogador::pular(bool pulando)
		{
			// Pulo de altura fixa: so impulsiona quando o jogador esta no
			// chao (isJumping == false). A colisao zera isJumping ao pousar.
			// Com a habilidade "Pulo duplo" ativa, um segundo pulo no ar
			// e disparado quando a tecla e RE-pressionada (edge "subiu") e
			// o pulo duplo ainda nao foi consumido.
			if (pulando && !isJumping)
			{
				velocity.y = jumpStrength;
				isJumping = true;
				animacao = 6;
			}
			else if (pulando && !puloPressionadoAnterior
				&& puloDuploLiberado && puloDuploDisponivel)
			{
				velocity.y = jumpStrength;
				puloDuploDisponivel = false;
				animacao = 6;
			}

			puloPressionadoAnterior = pulando;
		}

		void Jogador::processarEntrada()
		{
			if (animacao == 2 || vida <= 0.0f)
			{
				velocity.x = 0.0f;
				atacando = false;
				atacandoAtivo = false;
				regiaoAtaque = Vector2f();
				return;
			}

			// Stun: bloqueia toda entrada
			if (atordoado)
			{
				velocity.x = 0.0f;
				bater(false);
				return;
			}

			const bool ataquePressionado = Keyboard::isKeyPressed(teclas.at(3));
			// Borda de subida: so e true no frame em que o botao foi
			// pressionado (transicao solto -> pressionado).
			const bool ataqueNovo = ataquePressionado && !ataquePressionadoAnterior;
			ataquePressionadoAnterior = ataquePressionado;

			const bool direita = Keyboard::isKeyPressed(teclas.at(1));
			const bool esquerda = Keyboard::isKeyPressed(teclas.at(0));

			// Ataque em andamento: so cancela com movimento, caso contrario
			// a animacao continua ate completar um ciclo completo.
			if (atacandoAtivo)
			{
				if ((direita || esquerda) && !isJumping)
				{
					// Movimento cancela o ataque
					atacandoAtivo = false;
					bater(false);
					mover(direita, esquerda);
					pular(Keyboard::isKeyPressed(teclas.at(2)));
				}
				else if (concluida)
				{
					// Animacao completou: para sempre; exige novo pressionamento
					atacandoAtivo = false;
					bater(false);
					mover(direita, esquerda);
					pular(Keyboard::isKeyPressed(teclas.at(2)));
				}
				else
				{
					// Animacao em progresso: continua automaticamente
					bater(true);
				}
				return;
			}

			// Iniciar novo ataque apenas na borda de subida do botao
			if (ataqueNovo && !isJumping)
			{
				atacandoAtivo = true;
				bater(true);
				return;
			}

			bater(false);
			mover(direita, esquerda);
			pular(Keyboard::isKeyPressed(teclas.at(2)));
		}

		void Jogador::setConcluiuFase(bool cf)
		{
			concluiuFase = cf;
		}

		bool Jogador::getConcluiuFase()
		{
			return concluiuFase;
		}

		void Jogador::salvar(int save)
		{
			std::ofstream arquivo("Saves/save"+ to_string(save) + "_jogador.txt", std::ios::app);

			if (arquivo.is_open())
			{
				arquivo << vida << "\n";
				arquivo << corpo.getPosition().x << "\n";
				arquivo << corpo.getPosition().y << "\n";

				//arquivo << jumpStrength << "\n";
				//arquivo << n_frames << "\n";
				//arquivo << count << "\n";
				//arquivo << lado << "\n";
				//arquivo << anterior << "\n";
				//arquivo << iteracoes << "\n";
				//arquivo << ataque << "\n";
				//arquivo << vidaAnterior << "\n";
				//arquivo << atacando << "\n";
				//arquivo << tomandoDano << "\n";

				//arquivo << envenenado << "\n";
				//arquivo << tempoVeneno << "\n";
				//arquivo << tempoDecorridoVeneno << "\n";
				//arquivo << forcaVeneno << "\n";

				//arquivo << lento << "\n";
				//arquivo << tempoLentidao << "\n";
				//arquivo << tempoDecorridoLentidao << "\n";
				//arquivo << forcaLentidao << "\n";
				//arquivo << forcaPulo << "\n";

				arquivo.close();
			}
		}

		void Jogador::limparArquivo(int save)
		{
			std::ofstream arquivo("Saves/save" + std::to_string(save) + "_jogador.txt", std::ofstream::out);

			if (arquivo.is_open()) 
			{
				arquivo.close();
			}
		}

		float Jogador::getVidaAtual() const
		{
			return vida;
		}

		void Jogador::carregarEstado(float vidaSalva, Vector2f pos)
		{
			vida = vidaSalva;
			corpo.setPosition(pos);
			sprite.setPosition(pos);
		}


		void Jogador::inicializaAnimacoes()
		{
			auto* recursos = Gerenciadores::Gerenciador_Recursos::getGerenciador();
			String pasta = (indiceJogador == 0) ? "Jogador" : "Jogador2";
			const std::string base = "Assets/" + pasta + "/";

			const int pedacoWidth = 120;  //Largura
			const int pedacoHeight = 80;  //Altura

			sf::Vector2f spriteOrigin(corpo.getSize().x / 0.55f, corpo.getSize().y / 2.0f);
			sprite.setOrigin(spriteOrigin);

			Animacao animacaoAndar;
			Animacao animacaoTomarDano;
			Animacao animacaoMorte;
			Animacao animacaoAtacar;
			Animacao animacaoAtacar2;
			Animacao animacaoParado;
			Animacao animacaoPulo;
			Animacao animacaoAgachar;

			//ANDAR 0
			animacaoAndar.fatiarSpritesheet(recursos->getTextura(base + "_Run.png"), pedacoWidth, pedacoHeight);
			animacaoAndar.setAnimationSpeed(25.0f);

			//TOMAR DANO 1
			animacaoTomarDano.fatiarSpritesheet(recursos->getTextura(base + "_Hit.png"), pedacoWidth, pedacoHeight);
			animacaoTomarDano.setAnimationSpeed(30.0f);

			//MORTE 2
			animacaoMorte.fatiarSpritesheet(recursos->getTextura(base + "_Death.png"), pedacoWidth, pedacoHeight);

			//ATAQUE PESADO 3
			animacaoAtacar.fatiarSpritesheet(recursos->getTextura(base + "_Attack.png"), pedacoWidth, pedacoHeight);
			animacaoAtacar.setAnimationSpeed(15.0f);

			//ATAQUE LEVE 4
			animacaoAtacar2.fatiarSpritesheet(recursos->getTextura(base + "_Attack2.png"), pedacoWidth, pedacoHeight);
			animacaoAtacar2.setAnimationSpeed(15.0f);

			//PARADO 5
			animacaoParado.fatiarSpritesheet(recursos->getTextura(base + "_Idle.png"), pedacoWidth, pedacoHeight);
			animacaoParado.setAnimationSpeed(40.0f);

			//PULO 6
			animacaoPulo.fatiarSpritesheet(recursos->getTextura(base + "_Jump.png"), pedacoWidth, pedacoHeight);

			//AGACHAR 7
			animacaoAgachar.fatiarSpritesheet(recursos->getTextura(base + "_Crouch.png"), pedacoWidth, pedacoHeight);

			animacoes.push_back(animacaoAndar);
			animacoes.push_back(animacaoTomarDano);
			animacoes.push_back(animacaoMorte);
			animacoes.push_back(animacaoAtacar);
			animacoes.push_back(animacaoAtacar2);
			animacoes.push_back(animacaoParado);
			animacoes.push_back(animacaoPulo);
			animacoes.push_back(animacaoAgachar);

		}

		void Jogador::aplicarHabilidades()
		{
			if (!mundo)
				return;

			const ArvoreHabilidades& arv = mundo->getArvore();

			const int nvVida = arv.getNivel(ArvoreHabilidades::VIDA_EXTRA);
			vida += 25.0f * nvVida;
			vidaMaxima += 25.0f * nvVida;

			const int nvDano = arv.getNivel(ArvoreHabilidades::DANO_EXTRA);
			dano += 5.0f * nvDano;

			const int nvVel = arv.getNivel(ArvoreHabilidades::VELOCIDADE);
			vel.x += 2.0f * nvVel;

			const int nvPulo = arv.getNivel(ArvoreHabilidades::PULO_FORTE);
			for (int i = 0; i < nvPulo; ++i)
				jumpStrength *= 1.25f;

			const int nvAlcance = arv.getNivel(ArvoreHabilidades::ALCANCE_EXTRA);
			bonusAlcance = 0.15f * nvAlcance;

			const int nvVampiro = arv.getNivel(ArvoreHabilidades::VAMPIRO);
			if (nvVampiro > 0)
			{
				vampiro = true;
				curaVampiro = 5.0f * nvVampiro;
			}

			if (arv.foiDesbloqueada(ArvoreHabilidades::PULO_DUPLO))
				puloDuploLiberado = true;

			const int nvArmadura = arv.getNivel(ArvoreHabilidades::ARMADURA);
			if (nvArmadura > 0)
				fatorArmadura = 1.0f - 0.125f * nvArmadura;
		}

		bool Jogador::getVampiro() const
		{
			return vampiro;
		}

		float Jogador::getCuraVampiro() const
		{
			return curaVampiro;
		}

		void Jogador::curar(float quanto)
		{
			vida += quanto;
			// O teto e o getVida da classe (constante VIDA_MAX). Aceitamos
			// que VIDA_EXTRA aumente o "atual" sem mexer no teto exposto;
			// a barra ja reescala via atualizarBarraVida.
		}

		void Jogador::recarregarPuloDuplo()
		{
			puloDuploDisponivel = puloDuploLiberado;
		}

		bool Jogador::tomarDano(float dano, int dirKnockback)
		{
			const bool acertou = Personagem::tomarDano(dano * fatorArmadura, dirKnockback);

			// Feedback cinematico: flash vermelho curto quando o golpe
			// realmente acerta (evita piscar se invulneravel).
			if (acertou && mundo && mundo->getCamera())
				mundo->getCamera()->dispararFlashDano(12, 140);

			return acertou;
		}

		void Jogador::inicializaTeclas()
		{
			// As teclas vem do singleton de configuracao (Saves/config.txt).
			// Ordem: 0 = esquerda, 1 = direita, 2 = pulo, 3 = ataque.
			auto* cfg = Gerenciadores::Configuracao::getInstancia();

			teclas.push_back(cfg->getTecla(indiceJogador, Gerenciadores::Configuracao::ESQUERDA));
			teclas.push_back(cfg->getTecla(indiceJogador, Gerenciadores::Configuracao::DIREITA));
			teclas.push_back(cfg->getTecla(indiceJogador, Gerenciadores::Configuracao::PULAR));
			teclas.push_back(cfg->getTecla(indiceJogador, Gerenciadores::Configuracao::ATACAR));
		}

	}
}