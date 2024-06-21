#pragma once
#include <SFML/Graphics.hpp>
#include "CommonDef.h"
#include "Camera.h"
#include "Background.h"
#include "spaceship.h"
#include "Bullet.h"
#include "Laser.h"
#include "Asteroid.h"
#include "AsteroidSpawner.h"
#include "Text.h"

void addBullet(Bullet bullet);
void addLaser(Laser laser);
void addAsteroid(Asteroid asteroid);

ScreenName playScreen(sf::RenderWindow& window);