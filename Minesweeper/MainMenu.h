#pragma once

#include "Button.h"

#include <SFML/Graphics.hpp>
#include <iostream>


// 3-button menu

class MainMenu
{
public:

	Button button1;
	Button button2;
	Button button3;

public:

	MainMenu(sf::Vector2f menuSize, sf::Vector2f pos, sf::Color bgColor, sf::Vector2f btnSize, sf::Color btnColor, sf::Font& font)
	{

		//area ratio in the menu
		button_ratio = 2;
		text_ratio = 1;
		setFont(font);

		background.setSize(menuSize);
		background.setPosition(pos);
		background.setFillColor(bgColor);

		float menu_width = background.getSize().x;
		float menu_height = background.getSize().y;

		float ratio = float(button_ratio) / float(button_ratio + text_ratio);

		float leftPadding = (menu_width - btnNum * btnSize.x) / (btnNum + 1);
		float topPadding = (ratio * menu_height - btnSize.y) / 2;

		float posX;
		float posY = background.getGlobalBounds().top + (1 - ratio) * menu_height + topPadding;

		//button1
		posX = background.getGlobalBounds().left + leftPadding;
		button1 = Button("buton1", font, 15, btnSize, { posX,posY }, btnColor, sf::Color::Black);

		//button2
		posX = background.getGlobalBounds().left + btnSize.x + 2 * leftPadding;
		button2 = Button("button2", font, 15, btnSize, { posX,posY }, btnColor, sf::Color::Black);

		//button3
		posX = background.getGlobalBounds().left + btnSize.x * 2 + 3 * leftPadding;
		button3 = Button("button3", font, 15, btnSize, { posX,posY }, btnColor, sf::Color::Black);
	}

	void setMessage(std::string str)
	{
		textOnMenu.setString(str);
		adjustTextPosition();
	}

	void adjustTextPosition()
	{
		float menu_width = background.getGlobalBounds().width;
		float menu_height = background.getGlobalBounds().height;

		float text_width = textOnMenu.getGlobalBounds().width;
		float text_height = textOnMenu.getGlobalBounds().height;

		float ratio = float(text_ratio) / float(text_ratio + button_ratio);

		float leftPadding = (menu_width - text_width) / 2;
		float topPadding = (ratio * menu_height - text_height) / 2;

		float posX = background.getGlobalBounds().left + leftPadding;
		float posY = background.getGlobalBounds().top + topPadding;

		textOnMenu.setPosition(posX, posY);

	}

	void setTextOnMenu(std::string str, int charSize, sf::Color color)
	{
		textOnMenu.setString(str);
		textOnMenu.setCharacterSize(charSize);
		textOnMenu.setFillColor(color);
		adjustTextPosition();
	}

	MainMenu() {};

	void setFont(sf::Font& font)
	{
		textOnMenu.setFont(font);
		button1.setFont(font);
		button2.setFont(font);
	}


	void drawTo(sf::RenderWindow& window)
	{
		window.draw(background);
		window.draw(textOnMenu);
		button1.drawTo(window);
		button2.drawTo(window);
		button3.drawTo(window);
	}


private:

	int btnNum{ 3 };

	int button_ratio;
	int text_ratio;

	sf::Text textOnMenu;
	sf::RectangleShape background;

};

