#include "Jogador.hpp"
#include "Gerenciador_Recursos.hpp"
#include <iostream>
#define VIDA_MAX 100.0f

namespace Entidades
{
	namespace Personagens
	{
		Jogador::Jogador(const Vector2f pos, const Vector2f tam, int indice) :
			Personagem(),
			indiceJogador(indice),
			jumpStrength(-0.16f),
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
			concluiuFase(false)
		{
			dano = 0.15f;
			vida = VIDA_MAX;
			sprite.setPosition(pos);
			corpo.setSize(tam);
			corpo.setPosition(pos);
			corpo.setFillColor(sf::Color::Red);
			vel = Vector2f(1.1f, 1.1f);
			healthBar.setScale(vida / 500.0f, 0.2f);
			inicializaAnimacoes();
			inicializaTeclas();
		}

		Jogador::~Jogador() = default;

		void Jogador::atualizar()
		{
			if (animacao != 2)
			{
				if (envenenado)
				{
					if (tempoDecorridoVeneno < tempoVeneno)
					{
						sprite.setColor(corEnvenenado);
						vida -= forcaVeneno;
						tempoDecorridoVeneno++;
					}
					else
					{
						sprite.setColor(Color{255, 255, 255});
						tempoDecorridoVeneno = 0;
						envenenado = false;
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
						jumpStrength = -0.16f;
						vel.x = 1.1f;
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
			lento = lentidao;
			tempoLentidao = tempo;
			forcaLentidao = fL;
			forcaPulo = fP;
		}


		void Jogador::mover(bool direita, bool esquerda)
		{
			if (direita)
			{
				direcao.x = vel.x;
				animacao = 0;
				lado = 1;
			}
			else if (esquerda)
			{
				direcao.x = -vel.x;
				animacao = 0;
				lado = -1;
			}
			
			if(!direita && !esquerda)
			{
				animacao = 5;
				direcao.x = 0.0f;
			}

			corpo.move(direcao.x, 0.0f);
		}

		void Jogador::bater(bool batendo)
		{
			if (batendo && !isJumping)
			{
				if (Keyboard::isKeyPressed(teclas.at(0)))
					lado = -1;
				if (Keyboard::isKeyPressed(teclas.at(1)))
					lado = 1;

				regiaoAtaque = Vector2f(corpo.getPosition().x + (65.0f * lado), corpo.getPosition().y);
				atacando = true;

				direcao.x = 0.0;

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
			if (pulando)
			{
				if (!isJumping)
				{
					velocity.y = jumpStrength;
					isJumping = true;
					animacao = 6;
				}

			}
			else
			{
				velocity.y = jumpStrength / 1.13f;
			}
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

		void Jogador::inicializaTeclas()
		{
			if (indiceJogador == 0)
			{
				teclas.push_back(Keyboard::A);
				teclas.push_back(Keyboard::D);
				teclas.push_back(Keyboard::W);
				teclas.push_back(Keyboard::E);
			}
			else
			{
				teclas.push_back(Keyboard::Left);
				teclas.push_back(Keyboard::Right);
				teclas.push_back(Keyboard::Up);
				teclas.push_back(Keyboard::M);
			}
		}

	}
}