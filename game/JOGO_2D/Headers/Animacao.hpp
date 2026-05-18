#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Uma animacao e uma sequencia de quadros. Cada quadro aponta para uma
// textura COMPARTILHADA (mantida pelo Gerenciador_Recursos) mais o recorte
// dentro dela. Copiar uma Animacao copia apenas ponteiros + retangulos,
// nunca pixels - varias entidades do mesmo tipo compartilham as texturas.
class Animacao {
private:
	struct Quadro {
		const sf::Texture* textura;
		sf::IntRect rect;
	};

	std::vector<Quadro> quadros;
	float animationSpeed;

public:
	Animacao();

	// Fatia um spritesheet horizontal em quadros de (largura x altura).
	void fatiarSpritesheet(const sf::Texture& sheet, int largura, int altura);

	// Adiciona um quadro que ocupa a textura inteira (animacoes cujos
	// quadros estao em arquivos separados).
	void adicionarTextura(const sf::Texture& textura);

	void setAnimationSpeed(float speed);
	float getAnimationSpeed() const;
	int getNumFrames() const;

	// Aplica o quadro 'indice' a um sprite (textura + recorte).
	void aplicar(sf::Sprite& sprite, int indice) const;
};
