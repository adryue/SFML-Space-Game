#pragma once
#include <SFML/Graphics.hpp>
#include "CommonDef.h"


class Bullet
{
public:
	sf::RectangleShape hitbox;
	sf::CircleShape collisionBox;

	sf::Vector2f position;
	sf::Vector2f velocity;

	float rotation;

	float lifespan;

	Bullet(sf::Vector2f pos, sf::Vector2f vel, float rot);
	Bullet();

	bool update(); //returns true if the bullet should despawn
	void draw(sf::RenderWindow& window);
};

