#pragma once
#include "spaceship.h"

class Ship1 : public Spaceship
{
public:
	int bulletFiringCooldown;

	Ship1(int ShipNumber);

	void handleFiring();
};

