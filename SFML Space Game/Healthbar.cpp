#include "Healthbar.h"
#include <iostream>

const sf::Vector2f HEATBAR_MAX_SIZE(WIN_X_LEN / 3, 30.0);
const sf::Vector2f LASERBAR_MAX_SIZE(WIN_X_LEN / 4, 25.0);

const float HEALTHBAR_OUTLINE_THICKNESS = 3.0;

Healthbar::Healthbar(sf::Vector2f maxSz, float maxVal, int orientation, int barNum, float posOffset)
{
	maxSize = maxSz;

	outline.setFillColor(sf::Color(30, 30, 30));
	outline.setOutlineThickness(HEALTHBAR_OUTLINE_THICKNESS);
	outline.setSize(maxSize);

	bar.setFillColor(sf::Color::Yellow);
	bar.setSize(sf::Vector2f(0, maxSize.y));

	maxValue = maxVal;
	value = 0;

	switch (orientation)
	{
	case 0:
		outline.rotate(-90);
		outline.setPosition(outline.getOutlineThickness() + (barNum * outline.getOutlineThickness()) + posOffset,
							WIN_Y_LEN - outline.getOutlineThickness());
		
		bar.rotate(-90);
		bar.setPosition(outline.getOutlineThickness() + (barNum * outline.getOutlineThickness()) + posOffset,
						WIN_Y_LEN - outline.getOutlineThickness());
		
		break;
	case 1:
		outline.rotate(-90);
		outline.setPosition(WIN_X_LEN - (barNum * outline.getOutlineThickness()) - (bar.getSize().y + outline.getOutlineThickness()) - posOffset,
							WIN_Y_LEN - outline.getOutlineThickness());

		bar.rotate(-90);
		bar.setPosition(WIN_X_LEN - (barNum * outline.getOutlineThickness()) - (bar.getSize().y + outline.getOutlineThickness()) - posOffset,
						WIN_Y_LEN - outline.getOutlineThickness());
		break;
	}
}

void Healthbar::setValue(float val)
{
	value = val;
	bar.setSize(sf::Vector2f(value / maxValue * maxSize.x, maxSize.y));
	//bar.setFillColor(sf::Color(255, 255 * (1 - value / maxValue), 0));
	bar.setFillColor(sf::Color(minColor.r + (minColor.r - maxColor.r) * (1 - value / maxValue),
							   minColor.g + (minColor.g - maxColor.g) * (1 - value / maxValue),
							   minColor.b + (minColor.b - maxColor.b) * (1 - value / maxValue)));
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
