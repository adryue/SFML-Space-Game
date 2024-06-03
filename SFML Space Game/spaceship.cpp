#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "spaceship.h"

//const sf::Vector2f SHIP_SIZE = sf::Vector2f(5, 20);
const sf::Vector2f SHIP_SIZE = sf::Vector2f(10, 40);

const float SHIP_ROTATION_SPEED = 3.5;
const float SHIP_MOVEMENT_SPEED = 0.1;

const float JOYSTICK_THRESHOLD = 0.15; //the minimum value required to move (after the joystick is normalized)
const float JOYSTICK_X_MAX_VALUE = 70; //the maximum value from joystick x input
const float JOYSTICK_Z_MAX_VALUE = 60; //the maximum value from joystick Z input
const float JOYSTICK_R_MAX_VALUE = 60; //the maximum value from joystick R input
const float JOYSTICK_U_MAX_VALUE = 75;


Spaceship::Spaceship(int ShipNumber)
{
	hitbox.setSize(SHIP_SIZE);
	hitbox.setOrigin(SHIP_SIZE.x / 2, SHIP_SIZE.y / 2);
	hitbox.setFillColor(sf::Color::White);

	collisionBox.setRadius((SHIP_SIZE.x + SHIP_SIZE.y) / 2);
	collisionBox.setOrigin(collisionBox.getRadius(), collisionBox.getRadius());
	collisionBox.setFillColor(sf::Color::Green);

	//hitbox.setPosition(WIN_X_LEN / 2, WIN_Y_LEN / 2);
	position = sf::Vector2f(WIN_X_LEN / 2.0, WIN_Y_LEN / 2.0);

	//TODO: set up ship sprites

	rotation = 0;

	heat = 0;

	shipNumber = ShipNumber;
	switch (shipNumber)
	{
	case 0:
		position = sf::Vector2f(WIN_X_LEN / 3.0, WIN_Y_LEN / 2.0);
		break;
	case 1:
		position = sf::Vector2f(WIN_X_LEN / 3.0 * 2.0, WIN_Y_LEN / 2.0);
		break;
	}
}

void Spaceship::handleInputs()
{
	//TODO: add sprite functionality
	
	//---rotating the ship---
	float joyStickX = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::X);
	joyStickX = std::min(joyStickX, JOYSTICK_X_MAX_VALUE);
	joyStickX = std::max(joyStickX, -JOYSTICK_X_MAX_VALUE);
	joyStickX /= JOYSTICK_X_MAX_VALUE;

	if (abs(joyStickX) > JOYSTICK_THRESHOLD)
	{
		rotation += SHIP_ROTATION_SPEED * joyStickX;
	}

	//---moving the ship forward---
	float joyStickU = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::U);
	joyStickU += JOYSTICK_U_MAX_VALUE;
	joyStickU = std::min(joyStickU, JOYSTICK_U_MAX_VALUE * 2);
	joyStickU = std::max(joyStickU, 0.0f);
	joyStickU /= JOYSTICK_U_MAX_VALUE * 2;
	if (joyStickU >= JOYSTICK_THRESHOLD)
	{
		//convert from degrees to radians
		float rot = rotation * M_PI / 180;
		velocity.x += sin(rot) * SHIP_MOVEMENT_SPEED * joyStickU;
		velocity.y -= cos(rot) * SHIP_MOVEMENT_SPEED * joyStickU;
	}

	//---firing a bullet---
	if (sf::Joystick::isButtonPressed(shipNumber, 1))
	{
		Bullet b(position, velocity); //bullet has the same starting position and velocity as the ship

		//get the new rotation for the bullet
		/*float joyStickRotation = atan(-joyStickR / joyStickZ);
		if (joyStickZ < 0)
		{
			joyStickRotation += M_PI;
		}
		float bulletRotation = (rotation * M_PI / 180) + joyStickRotation;*/
		
		float rot = rotation * M_PI / 180;

		b.position.x += sin(rot) * collisionBox.getRadius();
		b.position.y -= cos(rot) * collisionBox.getRadius();

		b.velocity.x += sin(rot) * BULLET_SPEED;
		b.velocity.y -= cos(rot) * BULLET_SPEED;

		addBullet(b);
	}
	/*float joyStickZ = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::Z);
	joyStickZ = std::min(joyStickZ, JOYSTICK_Z_MAX_VALUE);
	joyStickZ = std::max(joyStickZ, -JOYSTICK_Z_MAX_VALUE);
	joyStickZ /= JOYSTICK_Z_MAX_VALUE;
	float joyStickR = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::R);
	joyStickR = std::min(joyStickR, JOYSTICK_R_MAX_VALUE);
	joyStickR = std::max(joyStickR, -JOYSTICK_R_MAX_VALUE);
	joyStickR /= JOYSTICK_R_MAX_VALUE;

	float joyStickZR = sqrt(joyStickZ * joyStickZ + joyStickR * joyStickR);

	if (joyStickZR > JOYSTICK_THRESHOLD)
	{
		Bullet b(position, velocity); //bullet has the same starting position and velocity as the ship

		//get the new rotation for the bullet
		float joyStickRotation = atan(-joyStickR / joyStickZ);
		if (joyStickZ < 0)
		{
			joyStickRotation += M_PI;
		}
		float bulletRotation = (rotation * M_PI / 180) + joyStickRotation;

		b.position.x += sin(bulletRotation) * collisionBox.getRadius();
		b.position.y -= cos(bulletRotation) * collisionBox.getRadius();

		b.velocity.x += sin(bulletRotation) * BULLET_SPEED;
		b.velocity.y -= cos(bulletRotation) * BULLET_SPEED;
		
		addBullet(b);
	}*/

	//---reset button---
	if (sf::Joystick::isButtonPressed(shipNumber, 9))
	{
		position = sf::Vector2f(WIN_X_LEN / 2.0, WIN_Y_LEN / 2.0);
		velocity.x = 0;
		velocity.y = 0;
	}
}

void Spaceship::update()
{
	//TODO: add sprite functionality

	hitbox.setRotation(rotation);
	position += velocity;
	hitbox.setPosition(position);
	collisionBox.setPosition(position);
}

void Spaceship::draw(sf::RenderWindow& window)
{
	//window.draw(collisionBox);
	window.draw(hitbox);
}

bool Spaceship::handleCollision(Bullet b)
{
	float posX = b.position.x - position.x;
	float posY = b.position.y - position.y;
	float minDistance = b.hitbox.getRadius() + collisionBox.getRadius();
	if (posX * posX + posY * posY <= minDistance * minDistance)
	{
		damage(BULLET_DAMAGE);
		return true;
	}
	return false;
}

void Spaceship::damage(float amount)
{
	heat += amount;
	std::cout << "heat: " << heat << std::endl;
}


/*
	For gamecube controllers
	Left Joystick:
		X Axis (left is -74, right is 78) -80 88
		Y Axis (up is -84, down is 69) -85 76
	Right Joystick:
		Z Axis (down is -74, up is 69) -64 80
		R Axis (left is 61, right is -75) 69 -78
	Left Trigger:
		U Axis (unpressed is -76, fully pressed is 80)
	Right Trigger:
		V Axis (unpressed is -75, fully pressed is 80)
	Buttons:
		0: X
		1: A
		2: B
		3: Y
		4: Left Trigger
		5: Right Trigger
		7: Z Trigger
		9: Start
		12: Up
		13: Right
		14: Down
		15: Left
*/