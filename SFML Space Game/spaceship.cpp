#include <math.h>
#include "spaceship.h"

#define PI 3.14159265358979323846

const sf::Vector2f SHIP_SIZE = sf::Vector2f(20, 100);

const float SHIP_ROTATION_SPEED = 5.0;
const float SHIP_MOVEMENT_SPEED = 0.1;

Spaceship::Spaceship()
{
	hitbox.setSize(SHIP_SIZE);
	hitbox.setOrigin(SHIP_SIZE.x / 2, SHIP_SIZE.y / 2);
	hitbox.setFillColor(sf::Color::White);
	hitbox.setPosition(WIN_X_LEN / 2, WIN_Y_LEN / 2);
	position = sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN / 2);

	//TODO: set up ship sprites

	rotation = 0;
}

void Spaceship::handleInputs()
{
	//TODO: add sprite functionality

	//rotating the ship
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		rotation -= SHIP_ROTATION_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		rotation += SHIP_ROTATION_SPEED;
	}

	//moving the ship forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		//convert from degrees to radians
		float rot = rotation * PI / 180;
		velocity.x += sin(rot) * SHIP_MOVEMENT_SPEED;
		velocity.y -= cos(rot) * SHIP_MOVEMENT_SPEED;
	}
}

void Spaceship::update()
{
	//TODO: add sprite functionality

	hitbox.setRotation(rotation);
	position += velocity;
	hitbox.setPosition(position);
}

void Spaceship::draw(sf::RenderWindow& window)
{
	window.draw(hitbox);
}