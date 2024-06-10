#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "CommonDef.h"

class Camera
{
public:
	sf::View view;

	std::vector<sf::Vector2f> coordinates; //stores all the coordinates that need to be able to be viewed

	Camera(int initialCoordSize);

	void updateView();

	sf::Vector2f getRelativePosition(int index); //gives the position of a ship relative to the screen
};

