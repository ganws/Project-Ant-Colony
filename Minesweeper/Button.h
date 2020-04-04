#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Button
{
public:

	Button() {};

	Button(std::string t, sf::Font &font, int charSize, sf::Vector2f size , sf::Vector2f pos, sf::Color bgColor, sf::Color textColor)
	{
		button.setSize(size);
		button.setFillColor(bgColor);
		m_bgColor = bgColor;

		text.setFont(font);
		text.setString(t);
		text.setFillColor(textColor);
		text.setCharacterSize(charSize);

		setPosition(pos);
	}

	Button(const Button& copybtn)
	{
		button.setSize(copybtn.button.getSize());
		button.setFillColor(copybtn.m_bgColor);
		this->m_bgColor = copybtn.m_bgColor;

		text.setFont(*copybtn.text.getFont());
		text.setString(copybtn.text.getString());
		text.setFillColor(copybtn.text.getFillColor());
		text.setCharacterSize(copybtn.text.getCharacterSize());

		setPosition(copybtn.button.getPosition());
	}

	void setPosition(sf::Vector2f pos)
	{

		float button_width = button.getSize().x;
		float button_height = button.getSize().y;

		float font_width = text.getGlobalBounds().width;
		float font_height = text.getGlobalBounds().height;

		button.setPosition(pos);
		text.setOrigin({ font_width / 2, font_height / 2 });
		text.setPosition({ button.getGlobalBounds().left + button_width / 2 , button.getGlobalBounds().top + button_height / 2 });
	}

	void setFont(sf::Font &font)
	{
		text.setFont(font);
	}

	void drawTo(sf::RenderWindow& window)
	{
		window.draw(button);
		window.draw(text);
	}

	~Button() {};

	bool isMouseOver(sf::RenderWindow &window)
	{
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		float topX = button.getGlobalBounds().left;
		float topY = button.getGlobalBounds().top;

		float botX = button.getGlobalBounds().left + button.getGlobalBounds().width;
		float botY = button.getGlobalBounds().top +button.getGlobalBounds().height;

		if (mousePos.x > topX && mousePos.x < botX && mousePos.y > topY && mousePos.y < botY)
		{
			m_isMouseOver = true;
			button.setFillColor(sf::Color(180,180,180));
		}
		else
		{
			m_isMouseOver = false;
			button.setFillColor(m_bgColor);
		}

		return m_isMouseOver;
	}

	bool isMouseOver_ABS(sf::RenderWindow& window)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);

		float topX = button.getGlobalBounds().left;
		float topY = button.getGlobalBounds().top;

		float botX = button.getGlobalBounds().left + button.getGlobalBounds().width;
		float botY = button.getGlobalBounds().top + button.getGlobalBounds().height;

		if (mousePos.x > topX && mousePos.x < botX && mousePos.y > topY && mousePos.y < botY)
		{
			m_isMouseOver = true;
			button.setFillColor(sf::Color(180, 180, 180));
		}
		else
		{
			m_isMouseOver = false;
			button.setFillColor(m_bgColor);
		}

		return m_isMouseOver;
	}

	void setText(std::string str, int fontSize)
	{
		text.setString(str);
		text.setCharacterSize(fontSize);
		setPosition(button.getPosition());
	}

	sf::Vector2f getPosition()
	{
		return button.getPosition();
	}

private:

	sf::RectangleShape button;
	sf::Text text;

	sf::Color m_bgColor;

	bool m_isMouseOver{false};

};

