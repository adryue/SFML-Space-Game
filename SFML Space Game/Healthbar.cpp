#include "Healthbar.h"
#include <iostream>

const sf::Vector2f HEALTHBAR_MAX_SIZE(WIN_X_LEN / 3, 30.0);

Healthbar::Healthbar(float maxVal, int orientation)
{
	//TODO: make health bar look better

	outline.setFillColor(sf::Color(30, 30, 30));
	outline.setOutlineThickness(3);
	outline.setSize(HEALTHBAR_MAX_SIZE);

	bar.setFillColor(sf::Color::Yellow);
	bar.setSize(sf::Vector2f(0, HEALTHBAR_MAX_SIZE.y));

	maxValue = maxVal;
	value = 0;

	switch (orientation)
	{
	case 0:
		outline.rotate(-90);
		outline.setPosition(outline.getOutlineThickness(), WIN_Y_LEN - outline.getOutlineThickness());
		//outline.move(outline.getOutlineThickness(), outline.getOutlineThickness());
		bar.rotate(-90);
		bar.setPosition(outline.getOutlineThickness(), WIN_Y_LEN - outline.getOutlineThickness());
		//bar.move(outline.getOutlineThickness(), outline.getOutlineThickness());
		break;
	case 1:
		outline.rotate(-90);
		outline.setPosition(WIN_X_LEN - (bar.getSize().y + outline.getOutlineThickness()), WIN_Y_LEN - outline.getOutlineThickness());

		bar.rotate(-90);
		bar.setPosition(WIN_X_LEN - (bar.getSize().y + outline.getOutlineThickness()), WIN_Y_LEN - outline.getOutlineThickness());
		break;
	}
}

void Healthbar::setValue(float val)
{
	value = val;
	bar.setSize(sf::Vector2f(value / maxValue * HEALTHBAR_MAX_SIZE.x, HEALTHBAR_MAX_SIZE.y));
	bar.setFillColor(sf::Color(255, 255 * (1 - value / maxValue), 0));
}

void Healthbar::draw(sf::RenderWindow& window)
{
	window.draw(outline);
	window.draw(bar);
}
