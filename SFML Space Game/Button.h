#pragma once
#include "CommonDef.h"
#include <string>
#include "Text.h"

class Button
{
public:
	Text text;

	sf::RectangleShape hitbox;
	sf::Vector2f orientation;

	Button(std::string txt, sf::Vector2f orient); //orientation should be a multiplier between 0 and 1 for window size

	void resetPosition(); //this should be called when the window gets resized

	void setSelected(bool selected);

	void draw(sf::RenderWindow& window);
};

