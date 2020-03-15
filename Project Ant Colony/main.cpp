#pragma once

#include "GameSetting.h"
#include "Ant.h"
#include "VectorFunction.h"
#include "Animation.h"
#include "PheromoneSystem.h"
#include "PathBlocker.h"
#include "drawMapBoundary.h"
#include "Colony.h"
#include "Hashit.h"
#include "PheroMatrix.h"

#include<fstream>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/OpenGL.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(GameSetting::windowWidth, GameSetting::windowHeight), "Colony");
	window.setFramerateLimit(GameSetting::FRAMERATE);
	sf::View view(sf::FloatRect(0.0f, 0.0f, GameSetting::windowWidth, GameSetting::windowHeight));
	sf::Clock gameclock;

	std::cout << static_cast<bool>(1200%7 + 1200%300) << "\n";


	float timeElapsed{ 0.0f };
	float randTime{ 0.0f };
	std::string USER_INPUT;
	//=====LOAD COMMAND LIST=======//
	std::string line;
	std::string command_list;
	std::ifstream command_list_file("command_list.md");
	if (command_list_file.is_open())
	{
		while (std::getline(command_list_file, line))
		{
			command_list.append(line);
			command_list.push_back('\n');
		}
		command_list_file.close();
			std::cout << command_list;
	}

	//======CREATE PHEROMONE SYSTEM=====
	PheromoneSystem pheromones(10000); //fixed 10k particles. beyond that game will crash
	PheroMatrix PheroTiles;

	//======CREATE PHEROMONE SYSTEM 2=====
	PheroTiles.initPheroMatrix(GameSetting::windowWidth, GameSetting::windowHeight, sf::Vector2u(300, 300)); //gamesetting: 1200x1200	

	//======LOAD TEXTURE========//
	sf::Texture skin;
	std::cout << &skin << "\n";
	if (!skin.loadFromFile("walk.png"))
		std::cout << "unable to load skin. \n";

	//======PATH BLOCKERS========//
	std::vector<PathBlocker> pblock_system;
	PathBlocker pblock1(sf::Vector2f(100.0f, 100.0f), sf::Color::Red, 30.0f);
	pblock_system.push_back(pblock1);

	drawMapBoundary(&pblock_system);

	//======CREATE COLONY==========//
	Colony Colony1;
	Colony1.initColony(&pblock_system, &skin, &pheromones, &PheroTiles);

	////=====WINDOW=======//
	while (window.isOpen())
	{
		float randT = static_cast<float>(rand() / static_cast<float>(RAND_MAX) * 3);
		if (randTime > randT)
		{
			float randLoc = static_cast<float>(rand() / static_cast<float>(RAND_MAX)* GameSetting::windowHeight);
			float randLoc2 = static_cast<float>(rand() / static_cast<float>(RAND_MAX)* GameSetting::windowHeight);
			randTime = 0.0f;
		}

	

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//move screen

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				view.move(0.0f, 4.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				view.move(0.0f, -4.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				view.move(-4.0f, 0.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				view.move(4.0f, 0.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				view.reset(sf::FloatRect(0.0f, 0.0f, GameSetting::windowWidth, GameSetting::windowHeight));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				Colony1.addAnt(sf::Vector2f(GameSetting::windowWidth / 2, GameSetting::windowHeight / 2));
				

			// issue move command
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				
				sf::Vector2i ClickPos = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(ClickPos);
				//pheromones.createParticle(sf::Vector2f(worldPos.x, worldPos.y), Activity::GATHERING);
				PheroTiles.addStrength(worldPos, 100.0f);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{

				sf::Vector2i ClickPos = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(ClickPos);

			}
			// DETECT STRING COMMAND
			if (event.type == sf::Event::TextEntered)	
			{
				if (event.text.unicode < 128 && event.text.unicode!=13 && event.text.unicode != 8)
				{
					char tmpChar{static_cast<char>(event.text.unicode)};
					USER_INPUT.push_back(tmpChar);
					std::cout << tmpChar;
				}
			}

			if ((!USER_INPUT.empty())&&(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)))
			{
				USER_INPUT.pop_back();
				std::cout << "\b \b";
			}
			// ENTER COMMAND
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))

			{
				if (!USER_INPUT.empty()) 
				{
					std::cout << "\n";
					switch (Parse(USER_INPUT))
					{
					case TEXT_COMMAND::EXIT:
						std::cout << "COMMAND: Exit";
						break;

					case TEXT_COMMAND::CREATE_ANT:
						std::cout << "COMMAND: Create ant";
						Colony1.addAnt(sf::Vector2f(GameSetting::windowWidth / 2, GameSetting::windowHeight/ 2));
						break;

					case TEXT_COMMAND::HELP:
						std::cout << command_list;
						break;

					case TEXT_COMMAND::ANTNUM:
						std::cout << Colony1.getAntNum() << "\n";
						break;

					case TEXT_COMMAND::ANTSKIN:
						std::cout << Colony1.ant_skin << "\n";
						break;

					default:
						std::cout << "COMMAND: Unknown";
					}
					std::cout << "\n";
					USER_INPUT.clear();

				}
			}
		}

		// update frame	
		timeElapsed = gameclock.restart().asSeconds() * GameSetting::GAMESPEED;
		Colony1.computeAntMove(timeElapsed);
		PheroTiles.pheromoneDecay(timeElapsed);
		pheromones.decay(timeElapsed);

		//clear window 
		window.setView(view);
		window.clear(sf::Color::White);

		//draw
		window.draw(PheroTiles);
		Colony1.drawColony(&window);
		//window.draw(pheromones);

		for (auto &n : pblock_system)
			window.draw(n);
		window.display();
		
		sf::View currentView = window.getView();
		randTime += timeElapsed;


	}


	return 0;
}