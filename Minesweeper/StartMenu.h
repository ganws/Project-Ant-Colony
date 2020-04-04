#pragma once

#include "Button.h"

#include <SFML/Graphics.hpp>
#include <iostream>


//===========================
//=====choose difficulty=====
//===========================
//=======|EASY|==============
//=======|MEDIUM|============
//=======|HARD|==============
//===========================

class StartMenu
{
public:

	Button button1;
	Button button2;
	Button button3;

public:

	StartMenu(sf::Vector2f menuSize, sf::Vector2f pos, sf::Color bgColor, sf::Vector2f btnSize, sf::Color btnColor, sf::Font& font)
	{

		//area ratio in the menu
		button_ratio = 7;
		text_ratio = 1;
		setFont(font);

		background.setSize(menuSize);
		background.setPosition(pos);
		background.setFillColor(bgColor);

		sf::Vector2f maxSize{ menuSize.x / 2 , menuSize.y / 2 };
		//if (btnSize.x > maxSize.x) btnSize.x = maxSize.x;
		//if (btnSize.y > maxSize.y) btnSize.y = maxSize.y;

		float menu_width = background.getSize().x;
		float menu_height = background.getSize().y;

		float ratio = float(button_ratio) / float(button_ratio + text_ratio);

		float leftPadding = (menu_width - btnSize.x) / 2;
		float topPadding = (ratio * menu_height - btnNum * btnSize.y) / (btnNum + 1);

		float posX;
		float posY;

		//button1
		posX = background.getGlobalBounds().left + leftPadding;
		posY = background.getGlobalBounds().top + (1 - ratio) * menu_height + topPadding;
		button1 = Button("buton1", font, 15, btnSize, { posX,posY }, btnColor, sf::Color::Black);

		//button2
		posY = button1.getPosition().y + btnSize.y + topPadding;
		button2 = Button("button2", font, 15, btnSize, { posX,posY }, btnColor, sf::Color::Black);

		//button3
		posY = button2.getPosition().y + +btnSize.y + topPadding;
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

	StartMenu() {};

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

