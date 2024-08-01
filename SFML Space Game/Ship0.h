#pragma once
#include "CommonDef.h"
#include "spaceship.h"
#include "Healthbar.h"

class Ship0 : public Spaceship
{
public:
	bool isFiringBullet;

	Ship0(int ShipNumber);

	void handleFiring();
};

