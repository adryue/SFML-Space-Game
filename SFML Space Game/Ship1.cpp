#define _USE_MATH_DEFINES
#include <math.h>
#include "Ship1.h"

const int SHIP_BULLET_FIRING_COOLDOWN = 10; //number of frames until next bullet can be fired

class Ship1Constants : public ShipConstants
{
public:
	Ship1Constants()
	{
		SHIP_SIZE = sf::Vector2f(40, 40);
		SHIP_RADIUS = (SHIP_SIZE.x + SHIP_SIZE.y) / 4; //used for calculating collisions
		SHIP_EXPANDED_MAX_SIZE = sf::Vector2f(160, 160);
		SHIP_EXPANDED_MAX_RADIUS = (SHIP_EXPANDED_MAX_SIZE.x + SHIP_EXPANDED_MAX_SIZE.y) / 4;

		SHIP_MASS = 10.0;

		SHIP_ROTATION_SPEED = 3.5;
		SHIP_MOVEMENT_SPEED = 0.1;
		SHIP_MINI_MOVEMENT_SPEED = 0.01;
		SHIP_MEGA_MOVEMENT_SPEED = 0.3f;

		SHIP_MAX_HEAT = 300;
		SHIP_HEAT_DISSIPATION = 0.5;
		SHIP_HEAT_EXPANDED_MAX_DISSIPATION = 1.0;
		SHIP_MEGA_BOOST_HEAT = 1.1f;

		SHIP_BULLET_FIRING_KNOCKBACK = 0.01; //multiplier value based on bullet's speed
		SHIP_BULLET_CAMERA_SHAKE = 2.5f;

		SHIP_LASER_FIRING_KNOCKBACK = 1.3; //multiplier value based on laser damage
		SHIP_LASER_MAX_BUILDUP = 100; //number of frames required to build up to full strength
		SHIP_LASER_BUILDUP_MAX_HEAT = 1.0; //when charging up the laser, the ship gains heat
		SHIP_LASER_CAMERA_SHAKE = 0.08f;
	}
};

Ship1::Ship1(int ShipNumber) : Spaceship(ShipNumber, Ship1Constants())
{
	texture.loadFromFile("Assets/Images/Ship0.png");
	sprite.setTexture(texture);
	sprite.setOrigin(CONSTANTS.SHIP_SIZE.x / 2, CONSTANTS.SHIP_SIZE.y / 2);

	thrusterFireTexture.loadFromFile("Assets/Images/Thruster Fire.png");
	thrusterFireSprite.setTexture(thrusterFireTexture);
	thrusterFireSprite.setOrigin(thrusterFireTexture.getSize().x / 2, 0);

	bulletFiringCooldown = 0;
	laserBuildup = 0;
	isFiringLaser = false;

	heatDissipationMultiplier = 0;
}

void Ship1::handleFiring()
{
	//the ship can only fire if it is not expanded
	if (state != State::normal)
	{
		laserBuildup = 0;
		laserBar.setValue(0);
		return;
	}

	//---firing a bullet---
	if (bulletFiringCooldown == 0)
	{
		if (sf::Joystick::isButtonPressed(shipNumber, 1))
		{
			bulletFiringCooldown = SHIP_BULLET_FIRING_COOLDOWN;

			Bullet b(position, velocity, rotation); //bullet has the same starting position and velocity as the ship

			float rot = rotation * M_PI / 180;

			b.position.x += sin(rot) * collisionBox.getRadius();
			b.position.y -= cos(rot) * collisionBox.getRadius();

			b.velocity.x += sin(rot) * BULLET_SPEED;
			b.velocity.y -= cos(rot) * BULLET_SPEED;

			//ship gets knockback
			velocity.x -= sin(rot) * BULLET_SPEED * CONSTANTS.SHIP_BULLET_FIRING_KNOCKBACK;
			velocity.y += cos(rot) * BULLET_SPEED * CONSTANTS.SHIP_BULLET_FIRING_KNOCKBACK;

			addBullet(b);
		}
	}
	else
	{
		bulletFiringCooldown--;
	}

	//---firing a laser
	if (sf::Joystick::isButtonPressed(shipNumber, 2))
	{
		isFiringLaser = true;
		laserBuildup++;
		laserBuildup = std::min(CONSTANTS.SHIP_LASER_MAX_BUILDUP, laserBuildup);

		laserBar.setValue(laserBuildup);

		//charging up the laser also adds heat
		damage(CONSTANTS.SHIP_LASER_BUILDUP_MAX_HEAT * ((float)laserBuildup / CONSTANTS.SHIP_LASER_MAX_BUILDUP));
	}
	else
	{
		if (isFiringLaser)
		{
			isFiringLaser = false;
			float rot = rotation * M_PI / 180;
			float damage = (float)laserBuildup / CONSTANTS.SHIP_LASER_MAX_BUILDUP;

			Laser laser(sf::Vector2f(position.x + sin(rot) * collisionBox.getRadius(),
				position.y - cos(rot) * collisionBox.getRadius()),
				rotation,
				damage,
				sf::Vector2f(0.f, 0.f));

			addLaser(laser);

			laserBuildup = 0;
			laserBar.setValue(0);

			//ship gets knockback
			velocity.x -= sin(rot) * damage * CONSTANTS.SHIP_LASER_FIRING_KNOCKBACK;
			velocity.y += cos(rot) * damage * CONSTANTS.SHIP_LASER_FIRING_KNOCKBACK;
		}
	}
}