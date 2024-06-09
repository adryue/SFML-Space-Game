#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "spaceship.h"

const sf::Vector2f SHIP_SIZE(40, 40);
const float SHIP_RADIUS = (SHIP_SIZE.x + SHIP_SIZE.y) / 4; //used for calculating collisions
const sf::Vector2f SHIP_EXPANDED_MAX_SIZE(160, 160);
const float SHIP_EXPANDED_MAX_RADIUS = (SHIP_EXPANDED_MAX_SIZE.x + SHIP_EXPANDED_MAX_SIZE.y) / 4;

const float SHIP_ROTATION_SPEED = 3.5;
const float SHIP_MOVEMENT_SPEED = 0.1;
const float SHIP_MINI_MOVEMENT_SPEED = 0.01;

const float SHIP_MAX_HEAT = 300;
const float SHIP_HEAT_DISSIPATION = 0.5;
const float SHIP_HEAT_EXPANDED_MAX_DISSIPATION = 1.0;

//const int SHIP_BULLET_FIRING_COOLDOWN = 20;
const float SHIP_BULLET_FIRING_KNOCKBACK = 0.01; //multiplier value based on bullet's speed

const float SHIP_LASER_FIRING_KNOCKBACK = 1.3; //multiplier value based on laser damage
const int SHIP_LASER_MAX_BUILDUP = 100; //number of frames required to build up to full strength
const float SHIP_LASER_BUILDUP_MAX_HEAT = 1.0; //when charging up the laser, the ship gains heat

const float JOYSTICK_THRESHOLD = 0.15; //the minimum value required to move (after the joystick is normalized)
const float JOYSTICK_X_MAX_VALUE = 70; //the maximum value from joystick x input
const float JOYSTICK_Z_MAX_VALUE = 60; //the maximum value from joystick Z input
const float JOYSTICK_R_MAX_VALUE = 60; //the maximum value from joystick R input
const float JOYSTICK_U_MAX_VALUE = 75;
const float JOYSTICK_V_MAX_VALUE = 75;


Spaceship::Spaceship(int ShipNumber) : heatBar(HEATBAR_MAX_SIZE, SHIP_MAX_HEAT, ShipNumber, 0, 0.f),
									   laserBar(LASERBAR_MAX_SIZE, SHIP_LASER_MAX_BUILDUP, ShipNumber, 1, HEATBAR_MAX_SIZE.y)
{
	heatBar.setMinMaxColors(sf::Color(255, 255, 0), sf::Color(255, 0, 0));
	laserBar.setMinMaxColors(sf::Color::Cyan, sf::Color::Cyan);

	hitbox.setSize(SHIP_SIZE);
	hitbox.setOrigin(SHIP_SIZE.x / 2, SHIP_SIZE.y / 2);
	hitbox.setFillColor(sf::Color::White);

	collisionBox.setRadius(SHIP_RADIUS);
	collisionBox.setOrigin(SHIP_RADIUS, SHIP_RADIUS);
	collisionBox.setFillColor(sf::Color(100, 100, 100, 100));
	collisionBox.setOutlineThickness(3.0);
	collisionBox.setOutlineColor(sf::Color::White);

	//hitbox.setPosition(WIN_X_LEN / 2, WIN_Y_LEN / 2);
	position = sf::Vector2f(WIN_X_LEN / 2.0, WIN_Y_LEN / 2.0);

	texture.loadFromFile("Images/Blue Ship.png");
	sprite.setTexture(texture);
	sprite.setOrigin(SHIP_SIZE.x / 2, SHIP_SIZE.y / 2);

	thrusterFireTexture.loadFromFile("Images/Thruster Fire.png");
	thrusterFireSprite.setTexture(thrusterFireTexture);
	thrusterFireSprite.setOrigin(thrusterFireTexture.getSize().x / 2, 0);

	rotation = 0;

	heat = 0;

	firingCooldown = 0;
	isFiringBullet = false;
	laserBuildup = 0;
	isFiringLaser = false;

	isThrusting = false;

	heatDissipationMultiplier = 0;

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

	state = State::normal;
}

void Spaceship::handleInputs()
{
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
		isThrusting = true;

		//convert from degrees to radians
		float rot = rotation * M_PI / 180;
		velocity.x += sin(rot) * SHIP_MOVEMENT_SPEED * joyStickU;
		velocity.y -= cos(rot) * SHIP_MOVEMENT_SPEED * joyStickU;

		//show more of the thruster fire sprite if the trigger is pressed more
		thrusterFireSprite.setOrigin(thrusterFireTexture.getSize().x / 2,
									(-SHIP_RADIUS * 0.7 + thrusterFireTexture.getSize().y) - (thrusterFireTexture.getSize().y * joyStickU));
	}
	else
	{
		isThrusting = false;
	}

	//---mini thruster adjustments---
	float joyStickZ = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::Z);
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
		velocity.x -= SHIP_MINI_MOVEMENT_SPEED * joyStickR;
		velocity.y -= SHIP_MINI_MOVEMENT_SPEED * joyStickZ;

		//position.x -= SHIP_MOVEMENT_SPEED * joyStickR * 100;
		//position.y -= SHIP_MOVEMENT_SPEED * joyStickZ * 100;
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

	//---changing the state to normal / expanded---
	float joyStickV = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::V);
	joyStickV += JOYSTICK_V_MAX_VALUE;
	joyStickV = std::min(joyStickV, JOYSTICK_V_MAX_VALUE * 2);
	joyStickV = std::max(joyStickV, 0.0f);
	joyStickV /= JOYSTICK_U_MAX_VALUE * 2;
	if (joyStickV >= JOYSTICK_THRESHOLD)
	{
		hitbox.setSize(sf::Vector2f(SHIP_SIZE.x + SHIP_EXPANDED_MAX_SIZE.x * joyStickV,
									SHIP_SIZE.y + SHIP_EXPANDED_MAX_SIZE.y * joyStickV));
		hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);

		collisionBox.setRadius(SHIP_RADIUS + SHIP_EXPANDED_MAX_RADIUS * joyStickV);
		collisionBox.setOrigin(collisionBox.getRadius(), collisionBox.getRadius());
		
		heatDissipationMultiplier = joyStickV;
		state = State::expanded;

		//reset values for firing lasers
		laserBuildup = 0;
		laserBar.setValue(0);
	}
	else
	{
		hitbox.setSize(SHIP_SIZE);
		hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);

		collisionBox.setRadius(SHIP_RADIUS);
		collisionBox.setOrigin(collisionBox.getRadius(), collisionBox.getRadius());

		state = State::normal;

		//the ship can only fire if it is not expanded
		
		//---firing a bullet---
		if (sf::Joystick::isButtonPressed(shipNumber, 1))
		{
			if (!isFiringBullet)
			{
				isFiringBullet = true;
				Bullet b(position, velocity, rotation); //bullet has the same starting position and velocity as the ship

				float rot = rotation * M_PI / 180;

				b.position.x += sin(rot) * collisionBox.getRadius();
				b.position.y -= cos(rot) * collisionBox.getRadius();

				b.velocity.x += sin(rot) * BULLET_SPEED;
				b.velocity.y -= cos(rot) * BULLET_SPEED;

				//ship gets knockback
				velocity.x -= sin(rot) * BULLET_SPEED * SHIP_BULLET_FIRING_KNOCKBACK;
				velocity.y += cos(rot) * BULLET_SPEED * SHIP_BULLET_FIRING_KNOCKBACK;

				addBullet(b);
			}
		}
		else
		{
			isFiringBullet = false;
		}

		//---firing a laser
		if (sf::Joystick::isButtonPressed(shipNumber, 2))
		{
			isFiringLaser = true;
			laserBuildup++;
			laserBuildup = std::min(SHIP_LASER_MAX_BUILDUP, laserBuildup);

			laserBar.setValue(laserBuildup);

			//charging up the laser also adds heat
			damage(SHIP_LASER_BUILDUP_MAX_HEAT * ((float)laserBuildup / SHIP_LASER_MAX_BUILDUP));
		}
		else
		{
			if (isFiringLaser)
			{
				isFiringLaser = false;
				float rot = rotation * M_PI / 180;
				float damage = (float)laserBuildup / SHIP_LASER_MAX_BUILDUP;

				Laser laser(sf::Vector2f(position.x + sin(rot) * collisionBox.getRadius(),
							position.y - cos(rot) * collisionBox.getRadius()),
							rotation,
							damage);

				addLaser(laser);

				laserBuildup = 0;
				laserBar.setValue(0);

				//ship gets knockback
				velocity.x -= sin(rot) * damage * SHIP_LASER_FIRING_KNOCKBACK;
				velocity.y += cos(rot) * damage * SHIP_LASER_FIRING_KNOCKBACK;
			}
		}
	}

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
	hitbox.setRotation(rotation);
	sprite.setRotation(rotation);

	position += velocity;
	hitbox.setPosition(position);
	collisionBox.setPosition(position);
	sprite.setPosition(position);

	if (isThrusting)
	{
		thrusterFireSprite.setRotation(rotation);
		thrusterFireSprite.setPosition(position);
	}

	switch (state)
	{
	case State::normal:
		heat -= SHIP_HEAT_DISSIPATION;
		break;
	case State::expanded:
		heat -= SHIP_HEAT_DISSIPATION + SHIP_HEAT_EXPANDED_MAX_DISSIPATION * heatDissipationMultiplier;
		break;
	}
	heat = std::max(heat, 0.0f);
	heatBar.setValue(heat);
}

void Spaceship::draw(sf::RenderWindow& window)
{
	switch (state)
	{
	case State::normal:
		break;
	case State::expanded:
		window.draw(collisionBox);
		break;
	}
	if (isThrusting)
	{
		window.draw(thrusterFireSprite);
	}
	//window.draw(collisionBox);
	//window.draw(hitbox);
	window.draw(sprite);
}

void Spaceship::drawUI(sf::RenderWindow& window)
{
	heatBar.draw(window);
	laserBar.draw(window);
}

bool Spaceship::handleCollision(Bullet b)
{
	float posX = b.position.x - position.x;
	float posY = b.position.y - position.y;
	float minDistance = b.collisionBox.getRadius() + collisionBox.getRadius();
	if (posX * posX + posY * posY <= minDistance * minDistance)
	{
		damage(BULLET_DAMAGE);
		return true;
	}
	return false;
}

bool Spaceship::handleCollision(Laser l)
{
	//if the laser is facing the opposite direction, it's guaranteed to not hit
	float aX = sin(l.hitbox.getRotation() * M_PI / 180);
	float aY = -cos(l.hitbox.getRotation() * M_PI / 180);

	float bX = l.position.x - position.x;
	float bY = l.position.y - position.y;

	if (aX * bX + aY * bY > 0) //if the dot product is negative (for some reason it's positive instead)
	{
		return false;
	}

	//calculate the intersection point between the equation representing the laser and the normal line going through the ship's circle
	float x = ((l.slope * l.position.x) + l.position.y + (position.x / l.slope) - position.y) / (l.slope + 1 / l.slope);
	float y = l.slope * (l.position.x - x) + l.position.y;

	//distance from the circle to the laser line
	float distX = x - position.x;
	float distY = y - position.y;
	float dist = std::sqrt(distX * distX + distY * distY);

	if (dist < l.size.x / 2 + collisionBox.getRadius())
	{
		damage(l.damage);
		return true;
	}

	return false;
}

void Spaceship::damage(float amount)
{
	heat += amount;
	heatBar.setValue(heat);
}

void Spaceship::changeState(State newState)
{
	state = newState;
	switch (state)
	{
	case State::normal:
		hitbox.setSize(SHIP_SIZE);
		hitbox.setOrigin(SHIP_SIZE.x / 2, SHIP_SIZE.y / 2);

		collisionBox.setRadius(SHIP_RADIUS);
		collisionBox.setOrigin(collisionBox.getRadius(), collisionBox.getRadius());
		break;
	case State::expanded:
		hitbox.setSize(SHIP_EXPANDED_MAX_SIZE);
		hitbox.setOrigin(SHIP_EXPANDED_MAX_SIZE.x / 2, SHIP_EXPANDED_MAX_SIZE.y / 2);

		collisionBox.setRadius(SHIP_EXPANDED_MAX_RADIUS);
		collisionBox.setOrigin(collisionBox.getRadius(), collisionBox.getRadius());
		break;
	}
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