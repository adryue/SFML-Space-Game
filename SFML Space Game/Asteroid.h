#pragma once
#include <SFML/Graphics.hpp>
class Asteroid
{
public:
	sf::CircleShape collisionBox;

	float radius;
	sf::Vector2f position;
	sf::Vector2f velocity;

	int outOfViewCount; //number of frames since asteroid became out of view

	Asteroid(float radi, sf::Vector2f pos, sf::Vector2f vel);

	bool update(sf::View camView); //returns true if asteroid should despawn (is outside of camera for too long)
	void draw(sf::RenderWindow& window);
};

