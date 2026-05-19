#pragma once

#include <SFML/Graphics.hpp>
#include "Gerenciador_Grafico.hpp"

class Mundo;

namespace Sistemas
{
	// Responsavel apenas pela camera: centraliza a view nos jogadores
	// e desenha o plano de fundo. Extraido de Fase (Passo 6).
	//
	// A view segue o alvo com uma leve suavizacao (lerp) e o centro e
	// arredondado para pixel inteiro - sem isso, um centro fracionario
	// faz a arte "tremer" horizontalmente ao andar.
	class Camera
	{
	private:
		Gerenciadores::Gerenciador_Grafico* gerenciadorGrafico;
		float centroX;
		bool inicializada;

	public:
		Camera();
		void atualizar(Mundo& mundo, const sf::Texture& texturaFundo);
	};
}
