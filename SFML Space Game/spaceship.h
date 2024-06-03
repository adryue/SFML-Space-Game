#pragma once
#include <SFML/Graphics.hpp>
#include "CommonDef.h"
#include "play.h"
#include "Bullet.h"

class Spaceship
{
public:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape hitbox;
	sf::CircleShape collisionBox;

	sf::Vector2f position;
	sf::Vector2f velocity;
	float rotation; //in degrees

	float heat;

	int shipNumber;

	Spaceship(int ShipNumber);

	void handleInputs();
	void update();
	void draw(sf::RenderWindow& window);

	bool handleCollision(Bullet b);
	void damage(float amount);
};