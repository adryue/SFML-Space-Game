#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "spaceship.h"

const sf::Vector2f SHIP_SIZE = sf::Vector2f(20, 100);

const float SHIP_ROTATION_SPEED = 3.0;
const float SHIP_MOVEMENT_SPEED = 0.1;

const float JOYSTICK_THRESHOLD = 10; //the minimum value required to move
const float JOYSTICK_X_MAX_VALUE = 70; //the maximum value from joystick x input

Spaceship::Spaceship(int ShipNumber)
{
	hitbox.setSize(SHIP_SIZE);
	hitbox.setOrigin(SHIP_SIZE.x / 2, SHIP_SIZE.y / 2);
	hitbox.setFillColor(sf::Color::White);
	//hitbox.setPosition(WIN_X_LEN / 2, WIN_Y_LEN / 2);
	position = sf::Vector2f(WIN_X_LEN / 2, WIN_Y_LEN / 2);

	//TODO: set up ship sprites

	rotation = 0;

	shipNumber = ShipNumber;
	switch (shipNumber)
	{
	case 0:
		position = sf::Vector2f(WIN_X_LEN / 3, WIN_Y_LEN / 2);
		break;
	case 1:
		position = sf::Vector2f(WIN_X_LEN / 3 * 2, WIN_Y_LEN / 2);
		break;
	}
}

void Spaceship::handleInputs()
{
	//TODO: add sprite functionality
	
	//rotating the ship
	float joyStickX = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::X);
	if (abs(joyStickX) > JOYSTICK_THRESHOLD)
	{
		//normalize our joystick values
		joyStickX = std::min(joyStickX, JOYSTICK_X_MAX_VALUE);
		joyStickX = std::max(joyStickX, -JOYSTICK_X_MAX_VALUE);
		rotation += SHIP_ROTATION_SPEED * joyStickX / JOYSTICK_X_MAX_VALUE;
		std::cout << joyStickX << std::endl;
	}

	//moving the ship forward
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	if (sf::Joystick::isButtonPressed(shipNumber, 5))
	{
		//convert from degrees to radians
		float rot = rotation * M_PI / 180;
		velocity.x += sin(rot) * SHIP_MOVEMENT_SPEED;
		velocity.y -= cos(rot) * SHIP_MOVEMENT_SPEED;
	}

	//firing a bullet

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

/*
	For gamecube controllers
	Left Joystick:
		X Axis (left is -74, right is 78) -80 88
		Y Axis (up is -84, down is 69) -85 76
	Right Joystick:
		Z Axis (down is -74, up is 69) -64 80
		R Axis (left is 61, right is -75) 69 -78
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