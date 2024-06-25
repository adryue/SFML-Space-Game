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

	float shake; //camera shake intensity

	Camera(int initialCoordSize);

	void resetSize(); //this should be called when the window gets resized
	void updateView();

	void addShake(float amount);

	sf::Vector2f getRelativePosition(int index); //gives the position of a ship relative to the screen
};

