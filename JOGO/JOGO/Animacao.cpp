#include "Animacao.hpp"

Animacao::Animacao():
	animationSpeed(30.0f),
	lado(1)
{
}

Animacao::~Animacao()
{
}

void Animacao::addFrame(const sf::Texture& texture) {
	frames.push_back(texture);
}

void Animacao::setAnimationSpeed(float speed) {
	animationSpeed = speed;
}

int Animacao::getNumFrames() const {
	return frames.size();
}

const sf::Texture& Animacao::getFrame(int index) const {
	return frames[index];
}

float Animacao::getAnimationSpeed() const {
	return animationSpeed;
}
