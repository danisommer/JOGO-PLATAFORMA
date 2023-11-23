#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include "Tela.hpp"
#include "Ente.hpp"
#include "Principal.hpp"

class Menu : public Ente
{
private:
	Tela telaInicial;
	Tela tela1;
	Tela tela2;
	Tela tela3;
	Tela tela4;

	int n_jogadores;

	void inicializaTextos();
	void inicializaBotoes();


	std::stack<int> telaAtual; 
	sf::Font* fonte;

	Text titulo;

	sf::Texture* imagemFundo;
	sf::Sprite* sprite;

	sf::Vector2i posMouse;
	sf::Vector2f coordenadasMouse;

	const sf::Vector2f posicaoTela;

	Principal objPrincipal;

public:
	Menu();
	~Menu();

	void executar();

	void pushTela(int tela);
	void popTela();
};