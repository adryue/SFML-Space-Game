#include "end.h"
#include <vector>

ScreenName endScreen(sf::RenderWindow& window)
{
	Text titleText("PLAYER " + std::to_string(winner + 1) + " WINS", sf::Vector2f(0.5, 1.f / 3.f));

	std::vector<Button> buttons;
	int buttonIndex = 0; //index of the currently selected button
	buttons.push_back(Button("PLAY AGAIN", sf::Vector2f(0.3, 2.f / 3.f)));
	buttons.push_back(Button("HOME", sf::Vector2f(0.7, 2.f / 3.f)));
	buttons[0].setSelected(true);

	window.setView(sf::View(sf::FloatRect(0.0, 0.0, WIN_X_LEN, WIN_Y_LEN)));
	titleText.resetPosition();
	for (Button& b : buttons)
	{
		b.resetPosition();
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				return ScreenName::none;
				break;

			case sf::Event::Resized:
				WIN_X_LEN = event.size.width;
				WIN_Y_LEN = event.size.height;
				window.setView(sf::View(sf::FloatRect(0.0, 0.0, WIN_X_LEN, WIN_Y_LEN)));
				titleText.resetPosition();
				for (Button& b : buttons)
				{
					b.resetPosition();
				}
				break;

			case sf::Event::JoystickButtonPressed:
				//left and right change the selected button
				buttons[buttonIndex].setSelected(false);
				if (event.joystickButton.button == 15) //left
				{
					buttonIndex--;
				}
				else if (event.joystickButton.button == 13) //right
				{
					buttonIndex++;
				}
				buttonIndex = (buttonIndex + buttons.size()) % buttons.size();
				buttons[buttonIndex].setSelected(true);

				//START presses the selected button
				if (event.joystickButton.button == 9) //START
				{
					switch (buttonIndex)
					{
					case 0:
						return ScreenName::play;
						break;
					case 1:
						return ScreenName::home;
						break;
					default:
						break;
					}
				}
				break;
			default:
				break;
			}
		}
		window.clear();

		//---draw everything---
		window.draw(titleText);

		for (Button& b : buttons)
		{
			b.draw(window);
		}

		window.display();
	}
}