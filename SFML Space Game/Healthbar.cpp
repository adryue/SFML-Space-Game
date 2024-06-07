#include "Healthbar.h"
#include <iostream>

const sf::Vector2f HEALTHBAR_MAX_SIZE(300.0, 30.0);

Healthbar::Healthbar(float maxVal, int orientation)
{
	//TODO: make health bar look better

	bar.setFillColor(sf::Color::Yellow);
	bar.setSize(sf::Vector2f(0, HEALTHBAR_MAX_SIZE.y));

	maxValue = maxVal;
	value = 0;

	switch (orientation)
	{
	case 0:
		break;
	case 1:
		bar.rotate(180);
		bar.setPosition(WIN_X_LEN, bar.getSize().y);
		break;
	}
}

void Healthbar::setValue(float val)
{
	value = val;
	bar.setSize(sf::Vector2f(value / maxValue * HEALTHBAR_MAX_SIZE.x, HEALTHBAR_MAX_SIZE.y));
}

void Healthbar::draw(sf::RenderWindow& window)
{
	window.draw(bar);
}
