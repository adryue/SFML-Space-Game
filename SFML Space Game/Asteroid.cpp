#include "Asteroid.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

const int ASTEROID_DESPAWN_TIME = 600; //number of frames asteroid is out of view before despawning

const float ASTEROID_MASS_MULTIPLIER = 0.00005;

Asteroid::Asteroid(float radi, sf::Vector2f pos, sf::Vector2f vel)
{
	collisionBox.setFillColor(sf::Color(100, 100, 100));
	collisionBox.setRadius(radi);
	collisionBox.setOrigin(radi, radi);
	collisionBox.setPosition(pos);

	mass = (4.f / 3.f) * M_PI * radi * radi * radi * ASTEROID_MASS_MULTIPLIER;
	//mass = 10;
	radius = radi;
	position = pos;
	velocity = vel;

	outOfViewCount = 0;
}

bool Asteroid::update(sf::View camView)
{
	position += velocity;
	collisionBox.setPosition(position);

	if (position.x + radius < camView.getCenter().x - camView.getSize().x / 2 ||
		position.x - radius > camView.getCenter().x + camView.getSize().x / 2 ||
		position.y + radius < camView.getCenter().y - camView.getSize().y / 2 ||
		position.y - radius > camView.getCenter().y + camView.getSize().y / 2)
	{
		outOfViewCount++;
		return outOfViewCount >= ASTEROID_DESPAWN_TIME;
	}
	else
	{
		outOfViewCount = 0;
	}
	return false;
}

void Asteroid::draw(sf::RenderWindow& window)
{
	window.draw(collisionBox);
}