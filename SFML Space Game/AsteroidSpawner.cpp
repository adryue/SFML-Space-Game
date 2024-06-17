#include "AsteroidSpawner.h"
#include <iostream>
#include <random>

const float ASTEROID_MIN_RADIUS = 50.0;
const float ASTEROID_MAX_RADIUS = 150.0;

const float ASTEROID_MIN_SPAWN_CHANCE = 0.001; //chance every frame to spawn an asteroid
const float ASTEROID_MAX_SPAWN_CHANCE = 1.0 / (float)FRAMERATE;

const float ASTEROID_MIN_SPEED = 2.0;
const float ASTEROID_MAX_SPEED = 4.0;

AsteroidSpawner::AsteroidSpawner()
{
	srand(time(NULL));
}

void AsteroidSpawner::updateCamLocation(sf::View camView)
{
	topLeft.x = camView.getCenter().x - camView.getSize().x / 2;
	topLeft.y = camView.getCenter().y - camView.getSize().y / 2;

	width = camView.getSize().x;
	height = camView.getSize().y;

	spawnChance = ASTEROID_MIN_SPAWN_CHANCE * (width / CAMERA_MIN_SIZE.x);
	spawnChance = std::min(spawnChance, ASTEROID_MAX_SPAWN_CHANCE);
}

void AsteroidSpawner::updateSpawn()
{
	//std::cout << "spawnChance: " << spawnChance << std::endl;
	if ((float)rand() / RAND_MAX <= spawnChance)
	{
		float radius = (float)rand() / RAND_MAX * (ASTEROID_MAX_RADIUS - ASTEROID_MIN_RADIUS) + ASTEROID_MIN_RADIUS;
		sf::Vector2f position;
		sf::Vector2f velocity;

		sf::Vector2f pos2;

		int side = rand() % 4;
		switch (side)
		{
		case 0: //spawn on top side
			position.x = topLeft.x + ((float)rand() / RAND_MAX) * width;
			position.y = topLeft.y - radius;

			//choose random point on opposite side that asteroid will fly to
			pos2.x = topLeft.x + (float)rand() / RAND_MAX * width;
			pos2.y = topLeft.y + height;
			break;

		case 1: //spawn on bottom side
			position.x = topLeft.x + ((float)rand() / RAND_MAX) * width;
			position.y = topLeft.y + height + radius;

			//choose random point on opposite side that asteroid will fly to
			pos2.x = topLeft.x + (float)rand() / RAND_MAX * width;
			pos2.y = topLeft.y;
			break;

		case 2: //spawn on left side
			position.x = topLeft.x - radius;
			position.y = topLeft.y + ((float)rand() / RAND_MAX) * height;

			//choose random point on opposite side that asteroid will fly to
			pos2.x = topLeft.x + width;
			pos2.y = topLeft.y + ((float)rand() / RAND_MAX) * height;
			break;

		case 3: //spawn on right side
			position.x = topLeft.x + width + radius;
			position.y = topLeft.y + ((float)rand() / RAND_MAX) * height;

			//choose random point on opposite side that asteroid will fly to
			pos2.x = topLeft.x;
			pos2.y = topLeft.y + ((float)rand() / RAND_MAX) * height;
			break;

		default:
			break;
		}

		velocity = pos2 - position;

		//normalize the velocity
		float magnitude = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
		velocity /= magnitude;

		//apply a random multiplier based on min and max asteroid speed
		velocity *= ((float)rand() / RAND_MAX) * (ASTEROID_MAX_SPEED - ASTEROID_MIN_SPEED) + ASTEROID_MIN_SPEED;

		addAsteroid(Asteroid(radius, position, velocity));
	}
}