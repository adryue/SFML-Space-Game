#include "Button.h"
#include <iostream>

const float BUTTON_PADDING = 30.f;

Button::Button(std::string txt, sf::Vector2f orient) : text(txt, orient)
{
	orientation = orient;

	hitbox.setFillColor(sf::Color(30, 30, 30, 128));
	hitbox.setOutlineColor(sf::Color::Cyan);
	hitbox.setSize(sf::Vector2f(text.getLocalBounds().width + text.getLocalBounds().left + BUTTON_PADDING,
								text.getLocalBounds().height + text.getLocalBounds().top + BUTTON_PADDING));
	hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);
	hitbox.setPosition(WIN_X_LEN * orientation.x, WIN_Y_LEN * orientation.y);
}

void Button::resetPosition()
{
	text.resetPosition();
	hitbox.setPosition(WIN_X_LEN * orientation.x, WIN_Y_LEN * orientation.y);
}

void Button::setSelected(bool selected)
{
	if (selected)
	{
		hitbox.setOutlineThickness(5.0);
	}
	else
	{
		hitbox.setOutlineThickness(0);
	}
}

void Button::draw(sf::RenderWindow& window)
{
	window.draw(hitbox);
	window.draw(text);

	/*sf::RectangleShape temp(sf::Vector2f(4, 4));
	temp.setFillColor(sf::Color::Red);
	temp.setOrigin(2, 2);
	temp.setPosition(WIN_X_LEN / 2, WIN_Y_LEN / 2);
	window.draw(temp);*/
}