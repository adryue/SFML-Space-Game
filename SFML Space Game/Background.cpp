#include "Background.h"

Background::Background(int backgroundNum, float intens, float opacity)
{
	texture.loadFromFile("Images/Background " + std::to_string(backgroundNum) + ".png");
	texture.setRepeated(true);
	sprite.setTexture(texture);
	sprite.setColor(sf::Color(255, 255, 255, 255.0 * opacity));
	sprite.setTextureRect(sf::IntRect(0, 0, 100000, 100000));
	sprite.setPosition(0, 0);

	intensity = intens;
}

void Background::update(sf::Vector2f camPosition)
{
	sprite.setPosition(camPosition.x * intensity - sprite.getTextureRect().width / 2,
					   camPosition.y * intensity - sprite.getTextureRect().height / 2);
}

void Background::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}