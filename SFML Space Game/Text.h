#pragma once
#include "CommonDef.h"

class Text : public sf::Text
{
public:
	sf::Vector2f orientation;

	Text(std::string txt, sf::Vector2f orient);

	void resetPosition();
};

