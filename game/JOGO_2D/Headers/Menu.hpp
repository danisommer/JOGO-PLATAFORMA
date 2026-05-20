#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include "Tela.hpp"
#include "Ente.hpp"
#include "Principal.hpp"
#include "Mundo.hpp"

#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

class Menu : public Ente
{
private:
	Tela telaInicial;
	Tela tela1;
	Tela tela2;
	Tela tela3;
	Tela tela4;
	Tela telaGameOver;

	int n_jogadores;

	void inicializaTextos();
	void inicializaBotoes();


	std::stack<int> telaAtual; 
	sf::Font* fonte;

	sf::Texture* imagemFundo;
	sf::Sprite* sprite;

	sf::Vector2i posMouse;
	sf::Vector2f coordenadasMouse;
	std::vector<sf::Text> ranking;

	const sf::Vector2f posicaoTela;

	std::string nomeDoJogador;
	std::string nomeDoJogador2;

	Principal objPrincipal;
	bool ordenado;

	struct PlayerScore
	{
		int score;
		std::string name;
	};

public:
	Menu();
	~Menu();

	void executar();

	void pushTela(int tela);
	void popTela();

	void mostrarRanking();
	void limparRanking();

	// Telas de configuracao - cada uma assume o controle do loop ate
	// o usuario voltar. Mantemos fora do switch de telaAtual porque
	// estas precisam desenhar dinamicamente (teclas atuais, valores).
	void executarTelaConfiguracoes();
	void executarTelaControles();
	void executarTelaTela();

	// Skill tree do modo roguelike. Mostra todas as habilidades, custo
	// e estado (desbloqueada / disponivel para compra / sem pontos),
	// permitindo gastar pontos acumulados no Mundo. Acessivel pelo
	// menu principal e tambem pelo menu de pausa (via Principal).
	void executarTelaHabilidades(Mundo* mundoExterno = nullptr);

	// Versao estatica que nao depende do estado do Menu (sem objPrincipal
	// nem sprite de fundo); usada pelo EstadoPausa, que so quer abrir a
	// tela de habilidades para o Mundo da run em curso.
	static void abrirTelaHabilidades(Mundo& mundo);
};