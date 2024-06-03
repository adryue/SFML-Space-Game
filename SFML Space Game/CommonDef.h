#pragma once

enum ScreenName
{
	home,
	play,
	none
};

//defined in main.cpp
extern const int FRAMERATE;
extern const int WIN_X_LEN;
extern const int WIN_Y_LEN;

//defined in spaceship.cpp
extern const sf::Vector2f SHIP_SIZE;

extern const float SHIP_ROTATION_SPEED;
extern const float SHIP_MOVEMENT_SPEED;

//defined in bullet.cpp
extern const float BULLET_RADIUS;

extern const float BULLET_SPEED;