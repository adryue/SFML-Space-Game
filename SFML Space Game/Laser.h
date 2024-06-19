#pragma once
#include "CommonDef.h"
#include <SFML/Graphics.hpp>

class Laser
{
public:
	sf::RectangleShape hitbox;

	sf::Vector2f size;
	sf::Vector2f position;
	sf::Vector2f velocity;
	float damage;

	float slope;

	Laser(sf::Vector2f pos, float rot, float pwr, sf::Vector2f vel);

	bool update();

	void draw(sf::RenderWindow& window);
};

