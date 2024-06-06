#pragma once
#include "CommonDef.h"
#include <SFML/Graphics.hpp>

class Laser
{
public:
	sf::RectangleShape hitbox;

	sf::Vector2f size;
	sf::Vector2f center;
	sf::Vector2f position;
	float damage;

	float slope;

	Laser(sf::Vector2f pos, float rot, float pwr);

	bool update();

	void draw(sf::RenderWindow& window);
};

