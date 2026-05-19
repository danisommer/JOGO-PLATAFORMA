#pragma once

#include <string>
#include <vector>

namespace Persistencia
{
	// Estado persistido de um jogador.
	struct EstadoJogador
	{
		float vida = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
	};

	// Conteudo completo de um save.
	struct DadosSave
	{
		int fase = 1;
		int numJogadores = 1;
		std::vector<EstadoJogador> jogadores;
	};

	// Camada de serializacao do jogo.
	//
	// Substitui o antigo esquema fragil (um arquivo por TIPO de entidade,
	// leitura posicional, contador static para pular linhas). Agora:
	//  - um unico arquivo por slot (Saves/saveN.txt);
	//  - cabecalho VERSIONADO;
	//  - linhas MARCADAS por tag ("FASE 2", "JOGADOR vida x y");
	//  - tags desconhecidas sao ignoradas na leitura, o que mantem
	//    compatibilidade com saves de versoes futuras.
	class GerenciadorSave
	{
	public:
		static const int VERSAO = 1;

		// Devolve false se o slot for invalido ou o arquivo nao abrir.
		static bool salvar(int slot, const DadosSave& dados);

		// Devolve false se o save nao existir ou estiver corrompido.
		static bool carregar(int slot, DadosSave& dados);

		static bool existe(int slot);
	};
}
