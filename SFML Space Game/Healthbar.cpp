#include "Healthbar.h"
#include <iostream>

const sf::Vector2f HEATBAR_MAX_SIZE(WIN_X_LEN / 3, 30.0);
const sf::Vector2f LASERBAR_MAX_SIZE(WIN_X_LEN / 4, 25.0);

const float HEALTHBAR_OUTLINE_THICKNESS = 3.0;

Healthbar::Healthbar(sf::Vector2f maxSz, float maxVal, int orient, int barNum, float posOffset)
{
	maxSize = maxSz;

	outline.setFillColor(sf::Color(30, 30, 30));
	outline.setOutlineThickness(HEALTHBAR_OUTLINE_THICKNESS);
	outline.setSize(maxSize);

	bar.setFillColor(sf::Color::Yellow);
	bar.setSize(sf::Vector2f(0, maxSize.y));

	maxValue = maxVal;
	value = 0;

	orientation = orient;
	barNumber = barNum;
	positionOffset = posOffset;

	resetPosition();
}

void Healthbar::resetPosition()
{
	switch (orientation)
	{
	case 0:
		outline.setRotation(-90);
		outline.setPosition(outline.getOutlineThickness() + (barNumber * outline.getOutlineThickness()) + positionOffset,
							WIN_Y_LEN - outline.getOutlineThickness());

		bar.setRotation(-90);
		bar.setPosition(outline.getOutlineThickness() + (barNumber * outline.getOutlineThickness()) + positionOffset,
						WIN_Y_LEN - outline.getOutlineThickness());

		break;
	case 1:
		outline.setRotation(-90);
		outline.setPosition(WIN_X_LEN - (barNumber * outline.getOutlineThickness()) - (bar.getSize().y + outline.getOutlineThickness()) - positionOffset,
							WIN_Y_LEN - outline.getOutlineThickness());

		bar.setRotation(-90);
		bar.setPosition(WIN_X_LEN - (barNumber * outline.getOutlineThickness()) - (bar.getSize().y + outline.getOutlineThickness()) - positionOffset,
						WIN_Y_LEN - outline.getOutlineThickness());
		break;
	}
}

void Healthbar::setValue(float val)
{
	value = std::min(val, maxValue);
	bar.setSize(sf::Vector2f(value / maxValue * maxSize.x, maxSize.y));
	/*bar.setFillColor(sf::Color(minColor.r + (minColor.r - maxColor.r) * (1 - value / maxValue),
							   minColor.g + (minColor.g - maxColor.g) * (1 - value / maxValue),
							   minColor.b + (minColor.b - maxColor.b) * (1 - value / maxValue)));*/

	bar.setFillColor(sf::Color(minColor.r + (maxColor.r - minColor.r) * (value / maxValue),
							   minColor.g + (maxColor.g - minColor.g) * (value / maxValue),
							   minColor.b + (maxColor.b - minColor.b) * (value / maxValue)));
}

void Healthbar::setMinMaxColors(sf::Color minClr, sf::Color maxClr)
{
	minColor = minClr;
	maxColor = maxClr;
}

void Healthbar::draw(sf::RenderWindow& window)
{
	window.draw(outline);
	window.draw(bar);
}
