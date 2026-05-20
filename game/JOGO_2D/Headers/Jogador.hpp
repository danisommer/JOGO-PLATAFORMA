#pragma once
#include "Personagem.hpp"
#include "Animacao.hpp"
#include <fstream>

#include <SFML/Graphics.hpp>
using namespace sf;

namespace Entidades
{
	namespace Personagens
	{
		class Jogador : public Personagem
		{
		private:
			// Indice do jogador (0 = P1, 1 = P2). Substitui o antigo
			// 'static bool jogadorCriado', que tornava o comportamento
			// dependente da ordem de criacao e impedia mais de 2 jogadores.
			const int indiceJogador;

			float jumpStrength;
			int n_frames;
			int count;
			int lado;
			int anterior;
			int iteracoes;
			int ataque;
			float vidaAnterior;
			bool atacando;
			bool tomandoDano;

			bool envenenado;
			int tempoVeneno;
			int tempoDecorridoVeneno;
			float forcaVeneno;
			sf::Color corEnvenenado;

			bool lento;
			int tempoLentidao;
			int tempoDecorridoLentidao;
			float forcaLentidao;
			float forcaPulo;

			bool atordoado;
			int tempoAtordoado;
			int tempoDecorridoAtordoado;

			// Controla o ciclo completo do ataque: true enquanto a animacao
			// de ataque nao completou um ciclo apos o primeiro disparo.
			bool atacandoAtivo;
		
			Vector2f regiaoAtaque;
			std::vector<Animacao> animacoes;
			std::vector<Keyboard::Key> teclas;
			Animacao* animacaoAtual;
			const Vector2f tamanhoCorpo;

			bool concluiuFase;
			
			void inicializaAnimacoes();
			void inicializaTeclas();
			void desenharEfeitosAtivos();

			// Bonus de pulo duplo (skill PULO_DUPLO). Quando true, o
			// jogador pode pular novamente uma vez no ar.
			bool puloDuploDisponivel;
			bool puloDuploLiberado;
			// Edge detection: detecta o instante em que o jogador
			// pressionou a tecla de pulo a partir de "solta".
			bool puloPressionadoAnterior;

			// Bonus de armadura (skill ARMADURA): reduz dano recebido.
			float fatorArmadura;
			// Vampiro (skill VAMPIRO): cura ao matar.
			bool vampiro;
			float curaVampiro;
			// Bonus de alcance dos ataques (skill ALCANCE_EXTRA).
			float bonusAlcance;

		public:
			Jogador(Vector2f pos, Vector2f tam, int indiceJogador);
			~Jogador();
			void atualizar();
			// Aplica bonus permanentes vindos da arvore de habilidades.
			// Chamado uma vez na construcao (apos a configuracao dos
			// atributos base) e nao deve ser invocado repetidamente.
			void aplicarHabilidades();
			// Acessores para o sistema de combate alimentar a habilidade
			// Vampiro (cura por kill) e o pulo duplo (consumir/recarregar).
			bool getVampiro() const;
			float getCuraVampiro() const;
			void curar(float quanto);
			void recarregarPuloDuplo();
			// Sobrescreve para aplicar a reducao de dano da habilidade
			// Armadura. Reduz 'dano' antes de delegar ao Personagem base.
			bool tomarDano(float dano, int dirKnockback = 0) override;
			void atualizarAnimacao(int animacao);
			void setAnimacao(int anim);
			const Vector2f getRegiaoAtaque() const;
			const float getDano() const;
			const bool getAtacando() const;
			float getVida();
			void setEnvenenado(bool veneno, int tempo, float intensidade, Color cor);
			void setLento(bool lentidao, int tempo, float fL, float fP);
			void setAtordoado(bool atoroado, int tempo);
			bool getAtordoado() const;
			void mover(bool direita, bool esquerda);
			void bater(bool batendo);
			void pular(bool pulando);
			// Le o proprio mapeamento de teclas e aciona mover/pular/bater.
			void processarEntrada();
			void setConcluiuFase(bool cf);
			bool getConcluiuFase();

			void salvar(int save);
			void limparArquivo(int save);

			// Usados pela camada de serializacao (GerenciadorSave).
			float getVidaAtual() const;
			void carregarEstado(float vidaSalva, Vector2f pos);

		};
	}

}



