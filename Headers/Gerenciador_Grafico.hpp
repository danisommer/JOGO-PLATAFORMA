#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Jogador.hpp"

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

		Entidades::Personagens::Jogador* jogador;
		Entidades::Personagens::Jogador* jogador2;
		static Gerenciador_Grafico* pGerenciador;
		sf::Texture backgroundTexture;
		Gerenciador_Grafico();
		int fase;

	public:
		~Gerenciador_Grafico();
		static Gerenciador_Grafico* getGerenciador();
		RenderWindow* getJanela();
		void limpaTela();
		void desenhaHitbox(const RectangleShape& corpo);
		void desenhaSprite(const Sprite& sprite);
		void mostraElemento();
		void fecharJanela();
		void atualizaCamera();
		bool getOpen();
		void setJogador(Entidades::Personagens::Jogador* jogador);
		void setJogador2(Entidades::Personagens::Jogador* jogador);
		void setFase(int fase);
	};

}
