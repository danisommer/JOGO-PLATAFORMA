#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Animacao {
private:
	std::vector<sf::Texture> frames;
	float animationSpeed;
	int lado;

public:
	Animacao();
	~Animacao();

	void addFrame(const sf::Texture& texture);
	void setAnimationSpeed(float speed);
	int getNumFrames() const;
	const sf::Texture& getFrame(int index) const;
	float getAnimationSpeed() const;
};

