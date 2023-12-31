#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#define TELA_X 1600.0f
#define TELA_Y 900.0f

using namespace sf;
using namespace std;

namespace Gerenciadores
{

	class Gerenciador_Grafico
	{
	private:
		RenderWindow* janela;
		View view;
		static Gerenciador_Grafico* pGerenciador;
		Gerenciador_Grafico();

	public:
		~Gerenciador_Grafico();
		static Gerenciador_Grafico* getGerenciador();
		RenderWindow* getJanela();
		void limpaTela();
		void desenhaHitbox(const RectangleShape& corpo);
		void desenhaSprite(const Sprite& sprite);
		void desenhaTela(sf::Drawable* tela);
		void desenhaTexto(sf::Text texto);
		void mostraElemento();
		void fecharJanela();
		bool getOpen();
		void setCentro(sf::Vector2f centro);
		Vector2f getViewCenter();
	};

}
