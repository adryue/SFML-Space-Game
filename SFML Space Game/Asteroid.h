#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "Utilities.h"

class Asteroid
{
public:
	sf::CircleShape collisionBox;

	float mass;
	float radius;
	sf::Vector2f position;
	sf::Vector2f velocity;

	int outOfViewCount; //number of frames since asteroid became out of view

	Asteroid(float radi, sf::Vector2f pos, sf::Vector2f vel);

	bool update(sf::View camView); //returns true if asteroid should despawn (is outside of camera for too long)

	bool handleCollision(Bullet b);
	void handleCollision(Asteroid& a);

	void draw(sf::RenderWindow& window);
};

