#pragma once

#include "Button.h"

#include <SFML/Graphics.hpp>
#include <iostream>

class MenuBar
{

	// menu bar with 1 button
	// ====|SCORE|=====|TIMER|==============|MENU|====

public:

	Button button1;

public:

	MenuBar(sf::Vector2f pos, sf::Vector2f size, sf::Color color, sf::Font& font, sf::Vector2f buttonSize)
	{
		background.setSize(size);
		background.setFillColor(color);
		background.setPosition(pos);

		scoreText.setFont(font);
		timeText.setFont(font);

		float topPadding;
		float rightPadding;
		float leftPadding;

		float posX;
		float posY;


		//Button
		topPadding = (size.y - buttonSize.y) / 2;
		rightPadding = 10;

		posX = pos.x + size.x - rightPadding - buttonSize.x;
		posY = pos.y + topPadding;


		button1 = Button("Menu", font, 15, buttonSize, { posX, posY }, sf::Color::White, sf::Color::Black);

		// Score text
		scoreText.setFont(font);
		scoreText.setString("score: 000");
		scoreText.setCharacterSize(15);

		topPadding = (size.y - scoreText.getGlobalBounds().height) / 2;
		leftPadding = 15;

		posX = pos.x + leftPadding;
		posY = pos.y + topPadding;

		scoreText.setPosition({ posX, posY });


		// timer text
		timeText.setFont(font);
		timeText.setString("Time: 000");
		timeText.setCharacterSize(15);

		topPadding = (size.y - timeText.getGlobalBounds().height) / 2;
		leftPadding = 15;

		posX = pos.x + scoreText.getGlobalBounds().width + 3*leftPadding;
		posY = pos.y + topPadding;

		timeText.setPosition({ posX, posY });

	};
	MenuBar() {};
	~MenuBar() {};

	void updateScore(int score, int time)
	{
		std::string str;
		str = "Score: " + std::to_string(score);
		scoreText.setString(str);

		str = "Time: " + std::to_string(time);
		timeText.setString(str);
	}

	void drawTo(sf::RenderWindow& window)
	{
		window.draw(background);
		window.draw(scoreText);
		window.draw(timeText);
		button1.drawTo(window);

	}

	sf::Vector2f getSize()
	{
		return background.getSize();
	}

private:
	
	sf::RectangleShape background;



	int score;
	sf::Text scoreText;

	int timer;
	sf::Text timeText;

};

