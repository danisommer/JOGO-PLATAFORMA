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

	sf::RectangleShape campoTexto;
	sf::Text textoEntrada;
	sf::Text textoCaixa;
	std::string entradaAtual;
	bool entradaAtiva;
	
	sf::RectangleShape campoTexto2;
	sf::Text textoEntrada2;
	sf::Text textoCaixa2;
	std::string entradaAtual2;
	bool entradaAtiva2;

	bool caixa1selecionada;
	bool caixa2selecionada;

	void processarEntradaTexto(sf::Event evento);
	
	void processarEntradaTexto2(sf::Event evento);
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
	void setEntradaAtiva(bool ativa);
	void setEntradaAtiva2(bool ativa);

	void configurarTextoEntrada(const sf::Font& fonte, unsigned int tamanho, const sf::Color& cor, const sf::Vector2f& posicao);
	void configurarCampoTexto(const sf::Vector2f& tamanho, const sf::Color& cor, const sf::Vector2f& posicao);
	void setTextoCaixa(const sf::Font& fonte, unsigned int tamanho, const sf::Vector2f& posicao, const char* texto);

	void configurarTextoEntrada2(const sf::Font& fonte, unsigned int tamanho, const sf::Color& cor, const sf::Vector2f& posicao);
	void configurarCampoTexto2(const sf::Vector2f& tamanho, const sf::Color& cor, const sf::Vector2f& posicao);
	void setTextoCaixa2(const sf::Font& fonte, unsigned int tamanho, const sf::Vector2f& posicao, const char* texto);


	string coletarNome();
	string coletarNome2();
};