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
	//
	// Tambem aplica efeitos cinematicos (tela tremendo, flash de dano,
	// tint de veneno) que sao usados como feedback visual durante o
	// combate. Os efeitos sao decaem por si - basta chamar disparar*
	// uma vez para que o efeito aparece no proximo frame.
	class Camera
	{
	private:
		Gerenciadores::Gerenciador_Grafico* gerenciadorGrafico;
		float centroX;
		bool inicializada;

		// Limites horizontais opcionais. Quando minX < maxX, o centro
		// da camera fica clampeado dentro do intervalo, evitando que
		// o "fim" da fase apareca de forma brusca na tela.
		float limiteMinX;
		float limiteMaxX;
		bool temLimites;

		// Efeitos:
		// - shakeFrames: numero de frames restantes de tremor.
		// - shakeIntensidade: deslocamento maximo aplicado ao centro.
		// - flashFrames: numero de frames restantes do flash vermelho.
		// - flashIntensidade: alpha inicial (0..255).
		// - venenoAtivo: enquanto true, desenha tint verde sutil.
		int shakeFrames;
		float shakeIntensidade;
		int flashFrames;
		int flashFramesMax;
		float flashIntensidade;
		bool venenoAtivo;

	public:
		Camera();
		void atualizar(Mundo& mundo, const sf::Texture& texturaFundo);
		// Desenha os overlays cinematicos (flash, tint, etc) por cima
		// das entidades. Deve ser chamado pelo EstadoJogo apos
		// AtualizarPersonagens, antes do HUD, para que os efeitos
		// realmente cubram o jogador.
		void desenharOverlays();

		void definirLimites(float minX, float maxX);
		void limparLimites();

		// Tela treme por 'frames' quadros com intensidade dada (px).
		void dispararTremor(int frames, float intensidade);
		// Tela pisca vermelho (dano). Decai linearmente em 'frames'.
		void dispararFlashDano(int frames, int alphaInicial);
		// Liga/desliga o tint verde (envenenamento).
		void setEnvenenado(bool ativo);
	};
}
