#pragma once

#include <SFML/System/Clock.hpp>

// Pequeno utilitario de tempo: mede o intervalo (delta time) entre
// chamadas. Usado pelo passo fixo de simulacao para desacoplar a
// velocidade do jogo da taxa de quadros da maquina.
class Relogio
{
private:
	sf::Clock clock;

public:
	// Devolve os segundos decorridos desde a ultima chamada (ou desde
	// a construcao) e reinicia a contagem.
	float reiniciar()
	{
		return clock.restart().asSeconds();
	}
};
