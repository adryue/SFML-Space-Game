#pragma once
#include <SFML/Graphics.hpp>
#include "CommonDef.h"

class Spaceship
{
public:

	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape hitbox;

	sf::Vector2f position;
	sf::Vector2f velocity;
	float rotation; //in degrees

	Spaceship();

	void handleInputs();
	void update();
	void draw(sf::RenderWindow& window);
};