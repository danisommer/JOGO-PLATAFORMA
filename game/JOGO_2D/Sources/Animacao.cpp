#include "Animacao.hpp"

Animacao::Animacao() :
	animationSpeed(50.0f)
{
}

void Animacao::fatiarSpritesheet(const sf::Texture& sheet, int largura, int altura)
{
	const int total = static_cast<int>(sheet.getSize().x);
	for (int x = 0; x + largura <= total; x += largura)
		quadros.push_back({ &sheet, sf::IntRect(x, 0, largura, altura) });
}

void Animacao::adicionarTextura(const sf::Texture& textura)
{
	quadros.push_back({ &textura,
		sf::IntRect(0, 0,
			static_cast<int>(textura.getSize().x),
			static_cast<int>(textura.getSize().y)) });
}

void Animacao::setAnimationSpeed(float speed)
{
	animationSpeed = speed;
}

float Animacao::getAnimationSpeed() const
{
	return animationSpeed;
}

int Animacao::getNumFrames() const
{
	return static_cast<int>(quadros.size());
}

void Animacao::aplicar(sf::Sprite& sprite, int indice) const
{
	if (quadros.empty())
		return;

	if (indice < 0)
		indice = 0;
	else if (indice >= static_cast<int>(quadros.size()))
		indice = static_cast<int>(quadros.size()) - 1;

	const Quadro& q = quadros[indice];
	sprite.setTexture(*q.textura);
	sprite.setTextureRect(q.rect);
}
