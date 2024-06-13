#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "CommonDef.h"

class Camera
{
public:
	sf::View view;

	std::vector<sf::Vector2f> coordinates; //stores all the coordinates that need to be able to be viewed

	sf::RectangleShape viewOutline; //this is used for debugging

	Camera(int initialCoordSize);

	void resetSize(); //this should be called when the window gets resized
	void updateView();

	sf::Vector2f getRelativePosition(int index); //gives the position of a ship relative to the screen
};

