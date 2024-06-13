#pragma once
#include "CommonDef.h"

class Background
{
public:
	sf::Texture texture;
	sf::Sprite sprite;

	float intensity;

	Background(int backgroundNum, float intens, float opacity);

	void update(sf::Vector2f camPosition, sf::Vector2f camSize);
	void draw(sf::RenderWindow& window);
};

