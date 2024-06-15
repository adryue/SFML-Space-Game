#include "Background.h"
#include <iostream>

Background::Background(int backgroundNum, float intens, float opacity)
{
	texture.loadFromFile("Images/Background " + std::to_string(backgroundNum) + ".png");
	texture.setRepeated(true);
	sprite.setTexture(texture);
	sprite.setColor(sf::Color(255, 255, 255, 255.0 * opacity));
	sprite.setTextureRect(sf::IntRect(0, 0, 100000, 100000));
	sprite.setOrigin(50000, 50000);
	sprite.setPosition(0, 0);

	intensity = intens;
}

void Background::update(sf::Vector2f camPosition, sf::Vector2f camSize)
{
	//TODO: fix background 3d zoom
	sprite.setPosition(camPosition.x * intensity,
					   camPosition.y * intensity);

	//sprite.setPosition(camPosition.x * intensity - sprite.getTextureRect().width / 2,
	//				   camPosition.y * intensity - sprite.getTextureRect().height / 2);

	//sprite.setScale(WIN_X_LEN / camSize.x, WIN_Y_LEN / camSize.y);

	//sprite.setScale((camSize.x / WIN_X_LEN - 1) * intensity + 1,
	//				(camSize.y / WIN_Y_LEN - 1) * intensity + 1);
	//std::cout << (camSize.x / WIN_X_LEN - 1) * intensity + 1 << std::endl;
}

void Background::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}