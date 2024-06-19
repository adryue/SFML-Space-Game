#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "Laser.h"

const sf::Vector2f LASER_MAX_SIZE(20.0, 10000.0); //the y value is an arbitrary large number

const float LASER_MAX_DAMAGE = 8.0;
const float LASER_FADE_SPEED = 0.2; //size that laser shrinks by per frame

Laser::Laser(sf::Vector2f pos, float rot, float pwr, sf::Vector2f vel)
{
	position = pos;
	velocity = vel;
	damage = LASER_MAX_DAMAGE * pwr;

	size.x = LASER_MAX_SIZE.x * pwr;
	size.y = LASER_MAX_SIZE.y;

	hitbox.setFillColor(sf::Color::Cyan);
	hitbox.setSize(size);
	hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y);
	hitbox.setPosition(pos);
	hitbox.setRotation(rot);

	slope = 1 / tan(rot * M_PI / 180);
}

bool Laser::update()
{
	size.x -= LASER_FADE_SPEED;
	if (size.x <= 0)
	{
		return true;
	}

	position += velocity;

	hitbox.setSize(size);
	hitbox.setOrigin(size.x / 2, size.y);
	hitbox.setPosition(position);

	damage = LASER_MAX_DAMAGE * (size.x / LASER_MAX_SIZE.x);

	return false;
}

void Laser::draw(sf::RenderWindow& window)
{
	window.draw(hitbox);
}