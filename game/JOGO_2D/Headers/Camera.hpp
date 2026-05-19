#pragma once

#include <SFML/Graphics.hpp>
#include "Gerenciador_Grafico.hpp"

class Mundo;

namespace Sistemas
{
	// Responsavel apenas pela camera: centraliza a view nos jogadores
	// e desenha o plano de fundo. Extraido de Fase (Passo 6).
	class Camera
	{
	private:
		Gerenciadores::Gerenciador_Grafico* gerenciadorGrafico;

	public:
		Camera();
		void atualizar(Mundo& mundo, const sf::Texture& texturaFundo);
	};
}
