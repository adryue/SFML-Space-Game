#pragma once
#include "CommonDef.h"
#include <SFML/Graphics.hpp>

class Healthbar
{
public:
	sf::RectangleShape outline;
	sf::RectangleShape bar;
	
	sf::Color minColor;
	sf::Color maxColor;

	sf::Vector2f maxSize;
	float value;
	float maxValue;

	int orientation;
	int barNumber;
	float positionOffset;

	Healthbar(sf::Vector2f maxSz, float maxVal, int orient, int barNum, float posOffset);
	//barNum is for when there are multiple bars, so that we know how far to offset (based on outline thickness)
	//posOffset is used when there are multiple bars, so you have to offset the position of each one (based on bar sizes)

	void resetPosition(); //this should be called when the window gets resized
	void setValue(float val);
	void setMinMaxColors(sf::Color minClr, sf::Color maxClr);
	void draw(sf::RenderWindow& window);
};

