#pragma once
#include <SFML/Graphics.hpp>
#include "CommonDef.h"


class Bullet
{
public:
	sf::CircleShape hitbox;

	sf::Vector2f position;
	sf::Vector2f velocity;

	Bullet(sf::Vector2f pos, sf::Vector2f vel);
	Bullet();

	void update();
	void draw(sf::RenderWindow& window);
};

