#include "Tela.hpp"

Tela::Tela():
	Ente(),
	larguraPadrao(0),
	larguraSelecionado(3),
	titulo()
{
}

Tela::~Tela()
{
	for (sf::Text& texto : textos) {
		delete &texto;
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
    }

    return -1;
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


