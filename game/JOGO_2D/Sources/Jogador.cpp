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
			tamanhoCorpo(tam),
			concluiuFase(false),
			puloDuploDisponivel(false),
			puloDuploLiberado(false),
			puloPressionadoAnterior(false),
			fatorArmadura(1.0f),
			vampiro(false),
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
			// Recarrega o pulo duplo sempre que o jogador esta no chao.
			// isJumping == false logo apos a resolucao da colisao com o
			// piso (Gerenciador_Colisoes::verificaColisao zera isto).
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

						// Tint verde na camera enquanto durar o veneno.
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
			}

			if (isJumping)
			{
				animacao = 6;
			}


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
			// Reativar o efeito reinicia o contador. Sem isto, contatos
			// repetidos com o Slime nao recomecavam o temporizador e o
			// estado podia expirar enquanto o jogador ainda estava em
			// cima da plataforma gosmenta.
			lento = lentidao;
			tempoLentidao = tempo;
			tempoDecorridoLentidao = 0;
			forcaLentidao = fL;
			forcaPulo = fP;
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
			// Durante a animacao de morte (animacao == 2) ou apos
			// zerar a vida, o jogador nao responde mais a teclas.
			// Sem este bloqueio, era possivel virar o sprite para os
			// lados (e disparar ataques) enquanto o personagem caia.
			if (animacao == 2 || vida <= 0.0f)
			{
				velocity.x = 0.0f;
				atacando = false;
				regiaoAtaque = Vector2f();
				return;
			}

			// teclas: 0 = esquerda, 1 = direita, 2 = pulo, 3 = ataque.
			// Cada jogador traz seu proprio mapeamento (ver inicializaTeclas),
			// portanto o gerenciador de eventos nao precisa codificar teclas.
			if (Keyboard::isKeyPressed(teclas.at(3)))
			{
				bater(true);
				return;
			}

			bater(false);

			const bool direita = Keyboard::isKeyPressed(teclas.at(1));
			const bool esquerda = Keyboard::isKeyPressed(teclas.at(0));
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
			// Os efeitos das habilidades sao aplicados como bonus
			// aditivos/multiplicativos sobre os atributos base. A vida
			// e curada para o novo maximo, evitando comecar uma fase
			// com a barra parcialmente vazia apos a compra de Vida +.
			if (!mundo)
				return;

			const ArvoreHabilidades& arv = mundo->getArvore();

			if (arv.foiDesbloqueada(ArvoreHabilidades::VIDA_EXTRA))
			{
				vida += 25.0f;
				vidaMaxima += 25.0f;
			}

			if (arv.foiDesbloqueada(ArvoreHabilidades::DANO_EXTRA))
				dano += 5.0f;

			if (arv.foiDesbloqueada(ArvoreHabilidades::VELOCIDADE))
				vel.x += 2.0f;

			if (arv.foiDesbloqueada(ArvoreHabilidades::PULO_FORTE))
				jumpStrength *= 1.25f; // mais forca (mais negativo)

			if (arv.foiDesbloqueada(ArvoreHabilidades::ALCANCE_EXTRA))
				bonusAlcance = 0.30f;

			if (arv.foiDesbloqueada(ArvoreHabilidades::VAMPIRO))
				vampiro = true;

			if (arv.foiDesbloqueada(ArvoreHabilidades::PULO_DUPLO))
				puloDuploLiberado = true;

			if (arv.foiDesbloqueada(ArvoreHabilidades::ARMADURA))
				fatorArmadura = 0.75f;
		}

		bool Jogador::getVampiro() const
		{
			return vampiro;
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