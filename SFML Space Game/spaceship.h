#pragma once
#include <SFML/Graphics.hpp>
#include "CommonDef.h"
#include "play.h"
#include "Bullet.h"
#include "Laser.h"
#include "Healthbar.h"
#include "Asteroid.h"

class Spaceship
{
public:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape hitbox;
	sf::CircleShape collisionBox;

	sf::Texture thrusterFireTexture;
	sf::Sprite thrusterFireSprite;

	sf::ConvexShape marker; //marks where the ship is on the screen
	sf::CircleShape markerCircle;

	sf::Vector2f position;
	sf::Vector2f velocity;
	float rotation; //in degrees

	float heat;
	Healthbar heatBar;

	int firingCooldown;
	bool isFiringBullet;

	int laserBuildup;
	bool isFiringLaser;
	Healthbar laserBar;

	//these variables are only for checking to see if you should draw certain objects
	bool isThrusting;
	bool drawMarker;

	float heatDissipationMultiplier;

	int shipNumber;

	enum State
	{
		normal,
		expanded
	};

	State state;

	Spaceship(int ShipNumber);

	void handleInputs();
	void update();
	void draw(sf::RenderWindow& window);
	void drawUI(sf::RenderWindow& window);

	bool handleCollision(Bullet b);
	bool handleCollision(Laser l);
	void handleCollision(Asteroid &a);
	void handleCollision(Spaceship& s);
	void damage(float amount);

	void setMarkerPosition(sf::Vector2f pos);
	void resetUIPositions();

	void changeState(State newState);
};