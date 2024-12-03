#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include "Tela.hpp"
#include "Ente.hpp"
#include "Principal.hpp"

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
};