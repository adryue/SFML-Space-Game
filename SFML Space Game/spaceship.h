#pragma once
#include <SFML/Graphics.hpp>
#include "CommonDef.h"
#include "play.h"
#include "Bullet.h"
#include "Laser.h"
#include "Healthbar.h"

class Spaceship
{
public:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape hitbox;
	sf::CircleShape collisionBox;

	sf::Texture thrusterFireTexture;
	sf::Sprite thrusterFireSprite;

	sf::Vector2f position;
	sf::Vector2f velocity;
	float rotation; //in degrees

	float heat;
	Healthbar heatbar;

	int firingCooldown;
	bool isFiringBullet;

	int laserBuildup;
	bool isFiringLaser;

	bool isThrusting;

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
	void damage(float amount);

	void changeState(State newState);
};