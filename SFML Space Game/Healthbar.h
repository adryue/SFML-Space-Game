#pragma once
#include "CommonDef.h"
#include <SFML/Graphics.hpp>

class Healthbar
{
public:
	sf::RectangleShape outline;
	sf::RectangleShape bar;
	
	float value;
	float maxValue;

	Healthbar(float maxVal, int orientation);

	void setValue(float val);
	void draw(sf::RenderWindow& window);
};

