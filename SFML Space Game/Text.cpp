#include "Text.h"

Text::Text(std::string txt, sf::Vector2f orient)
{
	orientation = orient;

	setFont(FONT);
	setFillColor(sf::Color::White);
	setString(txt);
	setOrigin((getLocalBounds().width + getLocalBounds().left) / 2, getLocalBounds().height);
	setPosition(WIN_X_LEN * orientation.x, WIN_Y_LEN * orientation.y);
}

void Text::resetPosition()
{
	setPosition(WIN_X_LEN * orientation.x, WIN_Y_LEN * orientation.y);
}
