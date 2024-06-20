#pragma once
#include <SFML/Graphics.hpp>
#include "Utilities.h"

enum ScreenName
{
	home,
	play,
	end,
	none
};

//defined in main.cpp
extern const int FRAMERATE;
extern int WIN_X_LEN;
extern int WIN_Y_LEN;
extern sf::Font FONT;
extern int winner;

//defined in spaceship.cpp
extern const sf::Vector2f SHIP_SIZE;

extern const float SHIP_ROTATION_SPEED;
extern const float SHIP_MOVEMENT_SPEED;

extern const float SHIP_MAX_HEAT;

//defined in Bullet.cpp
extern const float BULLET_RADIUS;
extern const float BULLET_SPEED;
extern const float BULLET_DAMAGE;

//defined in Laser.cpp
extern const float LASER_MAX_DAMAGE;

//defined in Healthbar.cpp
extern const sf::Vector2f HEATBAR_MAX_SIZE;
extern const sf::Vector2f LASERBAR_MAX_SIZE;

//defined in Camera.cpp
extern const sf::Vector2f CAMERA_MIN_SIZE;