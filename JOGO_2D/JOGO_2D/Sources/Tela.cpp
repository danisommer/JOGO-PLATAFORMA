#include "Tela.hpp"

Tela::Tela() :
	Ente(),
	larguraPadrao(0),
	larguraSelecionado(3),
	titulo()
{
}

Tela::~Tela()
{
	for (sf::Text& texto : textos) {
		delete& texto;
	}
	textos.clear();

	for (sf::RectangleShape* botao : botoes) {
		delete botao;
	}
	botoes.clear();
}

void Tela::desenharTela()
{
	gerenciador_grafico->desenhaTexto(titulo);

	for (auto t : textos)
	{
		gerenciador_grafico->desenhaTexto(t);
	}

	if (entradaAtiva)
	{
		gerenciador_grafico->desenhaHitbox(campoTexto);
		gerenciador_grafico->desenhaTexto(textoEntrada);
		gerenciador_grafico->desenhaTexto(textoCaixa);
	}

	if (entradaAtiva2)
	{
		gerenciador_grafico->desenhaHitbox(campoTexto2);
		gerenciador_grafico->desenhaTexto(textoEntrada2);
		gerenciador_grafico->desenhaTexto(textoCaixa2);

	}
}

int Tela::verificaEventoTela()
{
	sf::Event evento;

	while (gerenciador_grafico->getJanela()->pollEvent(evento))
	{
		if (evento.type == sf::Event::Closed)
			gerenciador_grafico->fecharJanela();

		posMouse = sf::Mouse::getPosition(*gerenciador_grafico->getJanela());
		coordenadasMouse = gerenciador_grafico->getJanela()->mapPixelToCoords(posMouse);

		for (int i = 0; i < botoes.size(); i++)
		{
			if (botoes.at(i)->getGlobalBounds().contains(coordenadasMouse))
			{
				textos[i].setOutlineThickness(larguraSelecionado);

				if (evento.type == sf::Event::MouseButtonReleased && evento.mouseButton.button == sf::Mouse::Left)
				{
					return i;
				}
			}
			else
			{
				textos[i].setOutlineThickness(larguraPadrao);
			}
		}

		if (campoTexto.getGlobalBounds().contains(coordenadasMouse))
		{
			caixa1selecionada = true;
			caixa2selecionada = false;
		}
		else if (campoTexto2.getGlobalBounds().contains(coordenadasMouse))
		{
			caixa1selecionada = false;
			caixa2selecionada = true;
		}
		else
		{
			caixa1selecionada = false;
			caixa2selecionada = false;
		}


		if (evento.type == sf::Event::TextEntered)
		{
			if (entradaAtiva && caixa1selecionada)
			{
				processarEntradaTexto(evento);
			}
			else if (entradaAtiva2 && caixa2selecionada)
			{
				processarEntradaTexto2(evento);
			}
		}
	}

	return -1;
}

void Tela::processarEntradaTexto(sf::Event evento)
{
	if (evento.text.unicode == 8 && entradaAtual.size() > 0)
	{
		entradaAtual.pop_back();
	}
	else if (evento.text.unicode < 128 && entradaAtual.size() < 15)
	{
		entradaAtual += static_cast<char>(evento.text.unicode);
	}

	textoEntrada.setString(entradaAtual);
}

void Tela::processarEntradaTexto2(sf::Event evento)
{
	if (evento.text.unicode == 8 && entradaAtual2.size() > 0)
	{
		entradaAtual2.pop_back();
	}
	else if (evento.text.unicode < 128 && entradaAtual2.size() < 15)
	{
		entradaAtual2 += static_cast<char>(evento.text.unicode);
	}

	textoEntrada2.setString(entradaAtual2);
}


void Tela::addBotao(sf::RectangleShape* botao)
{
	botoes.push_back(botao);
}

void Tela::addTexto(sf::Text texto)
{
	textos.push_back(texto);
}

void Tela::setTitulo(sf::Text texto)
{
	titulo = texto;
}

void Tela::setPosX(float X)
{
	titulo.setPosition(X, titulo.getPosition().y);

	for (int i = 0; i < textos.size(); i++)
	{
		textos.at(i).setPosition(X, textos.at(i).getPosition().y);
	}

	for (int i = 0; i < botoes.size(); i++)
	{
		botoes.at(i)->setPosition(X, botoes.at(i)->getPosition().y);
	}
}

void Tela::setLarguraPadrao(int L)
{
	larguraPadrao = L;
}

void Tela::setLarguraSelecionado(int L)
{
	larguraSelecionado = L;
}

void Tela::setEntradaAtiva(bool ativa)
{
	entradaAtiva = ativa;

	if (!entradaAtiva)
	{
		entradaAtual.clear();
		textoEntrada.setString(entradaAtual);

	}
}

void Tela::setEntradaAtiva2(bool ativa)
{
	entradaAtiva2 = ativa;

	if (!entradaAtiva2)
	{
		entradaAtual2.clear();
		textoEntrada2.setString(entradaAtual2);
	}
}

void Tela::setTextoCaixa(const sf::Font& fonte, unsigned int tamanho, const sf::Vector2f& posicao, const char* texto)
{
	textoCaixa.setCharacterSize(tamanho);
	textoCaixa.setPosition(posicao);
	textoCaixa.setString(texto);
	textoCaixa.setFont(fonte);
	textoCaixa.setOutlineColor(Color::Black);
}

void Tela::setTextoCaixa2(const sf::Font& fonte, unsigned int tamanho, const sf::Vector2f& posicao, const char* texto)
{
	textoCaixa2.setCharacterSize(tamanho);
	textoCaixa2.setPosition(posicao);
	textoCaixa2.setString(texto);
	textoCaixa2.setFont(fonte);
	textoCaixa2.setOutlineColor(Color::Black);
}


void Tela::configurarTextoEntrada(const sf::Font& fonte, unsigned int tamanho, const sf::Color& cor, const sf::Vector2f& posicao)
{
	textoEntrada.setFont(fonte);
	textoEntrada.setCharacterSize(tamanho);
	textoEntrada.setFillColor(cor);
	textoEntrada.setPosition(posicao);
}

void Tela::configurarCampoTexto(const sf::Vector2f& tamanho, const sf::Color& cor, const sf::Vector2f& posicao)
{
	campoTexto.setSize(tamanho);
	campoTexto.setFillColor(cor);
	campoTexto.setPosition(posicao);
}



string Tela::coletarNome()
{
	return entradaAtual;
}


void Tela::configurarTextoEntrada2(const sf::Font& fonte, unsigned int tamanho, const sf::Color& cor, const sf::Vector2f& posicao)
{
	textoEntrada2.setFont(fonte);
	textoEntrada2.setCharacterSize(tamanho);
	textoEntrada2.setFillColor(cor);
	textoEntrada2.setPosition(posicao);
}

void Tela::configurarCampoTexto2(const sf::Vector2f& tamanho, const sf::Color& cor, const sf::Vector2f& posicao)
{
	campoTexto2.setSize(tamanho);
	campoTexto2.setFillColor(cor);
	campoTexto2.setPosition(posicao);
}


string Tela::coletarNome2()
{
	return entradaAtual2;
}