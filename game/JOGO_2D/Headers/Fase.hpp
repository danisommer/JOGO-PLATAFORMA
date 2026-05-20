#pragma once

#include <cstdlib>
#include <string>
#include <SFML/Graphics.hpp>
#include "Ente.hpp"
#include "Mundo.hpp"
#include "Camera.hpp"
#include "SistemaCombate.hpp"
#include "Gerenciador_Grafico.hpp"
#include "Gerenciador_Colisoes.hpp"
#include "Gerenciador_Eventos.hpp"
#include "ListaEntidade.hpp"

namespace Fases
{
	// Orquestra uma fase: mantem as listas de entidades e delega as
	// responsabilidades especificas a colaboradores dedicados
	// (CarregadorFase, SistemaCombate, Camera). Antes era uma "god class".
	class Fase : public Ente
	{
	protected:
		Gerenciadores::Gerenciador_Grafico* gerenciador_grafico;
		Gerenciadores::Gerenciador_Eventos* gerenciador_eventos;
		Gerenciadores::Gerenciador_Colisoes* gerenciador_colisoes;

		Lista::ListaEntidade listaPersonagem;
		Lista::ListaEntidade listaObstaculo;

		// Estado de sessao compartilhado; pertence a Principal.
		Mundo* mundo;

		Sistemas::Camera camera;
		Sistemas::SistemaCombate sistemaCombate;

		sf::Texture texturaFundo;

		int numJogadores;
		bool concluida;
		bool derrota;
		int fase;
		bool morreu;

	public:
		Fase();
		~Fase();

		void setMundo(Mundo* m);

		void instanciaEntidades(const std::string& arquivoTxt);
		// Substitui o conteudo desta fase por uma geracao procedural
		// do roguelike. Usa o numero passado para variar dificuldade
		// e disparar fases de chefao a cada 5.
		void instanciaProcedural(int numeroFase, int numJogadores);
		// Define o numero da fase (usado pelo modo roguelike) e atualiza
		// a texturaFundo correspondente.
		void setFase(int f);
		void desalocaEntidades();
		void AtualizarPersonagens();
		void atualizaCamera();
		// Renderiza o HUD do modo roguelike (fase atual, kills, pontos
		// para a arvore) sobre o cenario. Chamado pelo EstadoJogo apos
		// AtualizarPersonagens e antes de mostraElemento.
		void desenharHUD();
		int getFase();
		bool getConcluida();
		bool getDerrota();
		void setDerrota(bool d);
		void verificaFase();

		// Serializacao da fase (via GerenciadorSave).
		void salvarJogo(int slot);
		void aplicarSave(int slot);

		// Acesso a camera para que outras entidades (jogador, chefao)
		// possam disparar efeitos cinematicos sem conhecer a Camera
		// diretamente.
		Sistemas::Camera& getCamera();
	};
}
