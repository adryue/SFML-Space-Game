#pragma once
#include <SFML/Graphics.hpp>
#include "CommonDef.h"
#include "Bullet.h"
#include "Laser.h"
#include "Asteroid.h"

void addBullet(Bullet bullet);
void addLaser(Laser laser);
void addAsteroid(Asteroid asteroid);

void addCameraShake(float amount);

ScreenName playScreen(sf::RenderWindow& window);