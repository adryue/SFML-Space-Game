#pragma once
#include <SFML/Graphics.hpp>

class Bullet
{
	sf::CircleShape hitbox;

	sf::Vector2f position;
	sf::Vector2f velocity;

	Bullet();

	void update();
	void draw(sf::RenderWindow& window);
};

