#pragma once
#include "CommonDef.h"

class Background
{
public:
	sf::Texture texture;
	sf::Sprite sprite;

	float intensity;

	Background(int backgroundNum, float intens);

	void update(sf::Vector2f camPosition);
	void draw(sf::RenderWindow& window);
};

