#pragma once
#include "Ente.hpp"

class Tela : public Ente
{
private:
	sf::Text titulo;
	std::vector<sf::Text> textos;
	std::vector<sf::RectangleShape*> botoes;
	sf::Vector2i posMouse;
	sf::Vector2f coordenadasMouse;
	int larguraPadrao;
	int larguraSelecionado;

public:
	Tela();
	~Tela();
	void desenharTela();
	int verificaEventoTela();
	void addBotao(sf::RectangleShape* botao);
	void addTexto(sf::Text texto);
	void setTitulo(sf::Text texto);
	void setPosX(float X);
	void setLarguraPadrao(int L);
	void setLarguraSelecionado(int L);

};