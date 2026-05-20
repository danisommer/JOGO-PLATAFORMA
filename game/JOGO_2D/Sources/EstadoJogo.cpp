#include "EstadoJogo.hpp"
#include "EstadoPausa.hpp"
#include "Gerenciador_Estados.hpp"
#include "Principal.hpp"
#include "Fase.hpp"
#include "Mundo.hpp"

namespace Estados
{
	namespace
	{
		// Duracao de um passo de simulacao (60 Hz). Como as constantes de
		// movimento do jogo foram ajustadas para 60 quadros por segundo,
		// manter este passo fixo preserva o "game feel" original e ainda
		// torna a simulacao independente da taxa de quadros real.
		const float PASSO_FIXO = 1.0f / 60.0f;
		const int   MAX_PASSOS = 5;       // teto anti "spiral of death"
		const float MAX_ACUMULADO = 0.25f;
	}

	EstadoJogo::EstadoJogo(Gerenciadores::Gerenciador_Estados* g, Principal* p,
		int numFase, int numJogadores, bool zerarPontuacao, int slotCarregar) :
		Estado(g),
		principal(p),
		fase(nullptr),
		numFase(numFase),
		numJogadores(numJogadores),
		gerenciadorGrafico(Gerenciadores::Gerenciador_Grafico::getGerenciador()),
		gerenciadorEventos(Gerenciadores::Gerenciador_Eventos::getGerenciador()),
		gerenciadorColisoes(Gerenciadores::Gerenciador_Colisoes::getGerenciador()),
		acumulador(0.0f),
		finalizado(false)
	{
		// Iniciar uma nova run reinicia a contagem de fase/kills do
		// Mundo, MAS preserva a arvore de habilidades (meta-progressao).
		if (zerarPontuacao)
		{
			principal->getMundo().reiniciarRun();
			principal->getMundo().setFaseAtual(numFase);
		}
		else
		{
			principal->getMundo().setFaseAtual(numFase);
		}

		fase = principal->prepararFase(numFase, numJogadores);

		// Continuar um jogo salvo: aplica o estado dos jogadores.
		if (slotCarregar > 0)
			fase->aplicarSave(slotCarregar);

		relogio.reiniciar();
	}

	void EstadoJogo::processarEventos()
	{
		gerenciadorEventos->Executar();
	}

	void EstadoJogo::executarPasso()
	{
		gerenciadorGrafico->limpaTela();
		fase->atualizaCamera();
		fase->AtualizarPersonagens();
		gerenciadorColisoes->Executar();
		fase->desenharHUD();
		gerenciadorGrafico->mostraElemento();

		fase->verificaFase();

		if (fase->getConcluida() || fase->getDerrota())
			finalizado = true;
	}

	void EstadoJogo::atualizar()
	{
		// ESC durante o jogo abre o estado de pausa por cima deste.
		if (gerenciadorEventos->getJogoPausado())
		{
			gerenciadorEstados->empilhar(
				std::make_unique<EstadoPausa>(gerenciadorEstados, principal, fase));
			relogio.reiniciar();   // descarta o tempo gasto na pausa
			return;
		}

		// Passo fixo com acumulador: a simulacao consome o tempo real em
		// fatias de PASSO_FIXO. Numa maquina saudavel (60 fps) isso roda
		// exatamente um passo por quadro - comportamento identico ao
		// original; em quedas de fps, recupera o atraso (ate MAX_PASSOS).
		acumulador += relogio.reiniciar();
		if (acumulador > MAX_ACUMULADO)
			acumulador = MAX_ACUMULADO;

		int passos = 0;
		while (acumulador >= PASSO_FIXO && passos < MAX_PASSOS && !finalizado)
		{
			executarPasso();
			acumulador -= PASSO_FIXO;
			passos++;
		}

		if (finalizado)
		{
			// Roguelike: ao concluir a fase, geramos a proxima
			// procedural. Ao perder, voltamos ao menu (preservando a
			// arvore de habilidades em Mundo para a proxima run).
			if (fase->getConcluida())
			{
				principal->getMundo().avancarFase();
				const int prox = principal->getMundo().getFaseAtual();
				gerenciadorEstados->trocar(std::make_unique<EstadoJogo>(
					gerenciadorEstados, principal, prox, numJogadores, false));
			}
			else
			{
				gerenciadorEstados->desempilhar();
			}
		}
	}

	void EstadoJogo::desenhar()
	{
		// O desenho da fase ocorre dentro de executarPasso() (atualizacao
		// e renderizacao das entidades ainda estao acopladas).
	}
}
