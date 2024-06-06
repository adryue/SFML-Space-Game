#pragma once
#include <SFML/Graphics.hpp>
#include "CommonDef.h"
#include "spaceship.h"
#include "Bullet.h"
#include "Laser.h"

void addBullet(Bullet bullet);
void addLaser(Laser laser);

ScreenName playScreen(sf::RenderWindow& window);