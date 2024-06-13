#pragma once
#include "CommonDef.h"
#include "play.h"
#include "Camera.h"

class AsteroidSpawner
{
public:
	sf::Vector2f topLeft;
	
	float width;
	float height;

	float spawnChance;

	AsteroidSpawner();

	void updateCamLocation(sf::View camView);

	void updateSpawn(); //ticks down spawnTime and spawns a random asteroid when spawnTime is 0
};

