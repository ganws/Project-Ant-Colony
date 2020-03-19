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
#include "SpatialPartition.h"
#include "Food.h"

#include<fstream>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/OpenGL.hpp>
#include<iostream>
#include<SFML/System/Vector2.hpp>

int main()
{

	//////////////////////////////////////////////////////////////////////////////
	////////////////////////////   INITIALIZATION   //////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(GameSetting::windowWidth, GameSetting::windowHeight), "Colony");
	window.setFramerateLimit(GameSetting::FRAMERATE);
	sf::View view(sf::FloatRect(0.0f, 0.0f, GameSetting::windowWidth, GameSetting::windowHeight));
	sf::Clock gameclock;

	sf::Vector2f v1(2, 2);
	sf::Vector2f v2;
	v2 =  v1*3.0;

	printf("x = %f, y = %f\n", v2.x, v2.y);

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

	//===============temporary UI settings =================//
	bool block_placement{ false };
	MOUSE_INPUT_MODE placement_mode{ MOUSE_INPUT_MODE::EMPTY };
	bool show_pheromone{ true };
	bool show_sensor{ true };


	//////////////////////////////////////////////////////////////////////////////
	////////////////////////////   WORLD CREATION   //////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	//======CREATE PHEROMONE SYSTEM=====
	//PheromoneSystem pheromones(10000); //fixed 10k particles. beyond that game will crash
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
	PathBlocker pblock1(sf::Vector2f(100.0f, 100.0f), sf::Color::Blue, 30.0f);
	pblock_system.push_back(pblock1);

	drawMapBoundary(&pblock_system);

	//========LOAD SPATIAL PARTITIONING OBJECT===========//
	SpatialPartition partition;
	partition.initSpatialPartition(GameSetting::windowWidth, GameSetting::windowHeight, sf::Vector2u(20, 20));
	partition.updateCheckIndex(&pblock_system);

	//======FOOD OBJECT=============//
	std::vector<Food> food_system;

	//======CREATE COLONY==========//
	Colony Colony1;
	Colony1.initColony(&pblock_system, &skin, &PheroTiles, &partition, &food_system);

	//======COLONY HOLE===========//
	sf::CircleShape Chole;
	Chole.setFillColor(sf::Color::Magenta);
	Chole.setRadius(10.0);
	Chole.setOrigin(Chole.getRadius(), Chole.getRadius());
	Chole.setPosition(sf::Vector2f(1000, 1000));
	Colony1.setCholePos(Chole.getPosition());


	////=====WINDOW=======//
	while (window.isOpen())
	{

		//////////////////////////////////////////////////////////////////////////////
		////////////////////////////   USER INTERFACE   //////////////////////////////
		//////////////////////////////////////////////////////////////////////////////

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//move screen

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				view.move(0.0f, 8.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				view.move(0.0f, -8.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				view.move(-8.0f, 0.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				view.move(8.0f, 0.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				view.reset(sf::FloatRect(0.0f, 0.0f, GameSetting::windowWidth, GameSetting::windowHeight));

			//======TOGGLE PHEROMONE VISUAL==============//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Z))
			{
				show_pheromone = !show_pheromone;
				std::cout << "PHEROMONE VISUAL: " << show_pheromone << "\n";
			}

			//======TOGGLE SENSOR VISUAL==============//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
			{
				show_sensor = !show_sensor;
				std::cout << "SENSOR VISUAL: " << show_sensor << "\n";
			}

			//===============ADD ANT================//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Q))
			{
				float rand_x = static_cast<float>(rand() / static_cast<float>(RAND_MAX / GameSetting::windowWidth));
				float rand_y = static_cast<float>(rand() / static_cast<float>(RAND_MAX / GameSetting::windowHeight));

				////recalculate for padding

				while ((rand_x < 20.0) | (rand_x > (GameSetting::windowWidth - 20.0)))
					rand_x = static_cast<float>(rand() / static_cast<float>(RAND_MAX / GameSetting::windowWidth));
				while ((rand_y < 20.0) | (rand_y > (GameSetting::windowHeight - 20.0)))
					rand_y = static_cast<float>(rand() / static_cast<float>(RAND_MAX / GameSetting::windowHeight));

				Colony1.addAnt(sf::Vector2f(rand_x, rand_y));
				std::cout << "pressed Q!\n";
			}


			//////////////////MOUSE PLACEMENT///////////////////
			//===============PATH BLOCKER================//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P))
			{
				placement_mode = MOUSE_INPUT_MODE::PBLOCK;
				std::cout << "PATHBLOCK PLACEMENT: ON\n";
			}

			//===============ANT======================//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::A))
			{
				placement_mode = MOUSE_INPUT_MODE::ADDANT;
				std::cout << "ANT PLACEMENT: ON\n";
			}

			//===============PHEROMONE================//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::O))
			{
				placement_mode = MOUSE_INPUT_MODE::PHEROMONE;
				std::cout << "PHEROMONE PLACEMENT: ON\n";
			}

			//===============FOOD================//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::F))
			{
				placement_mode = MOUSE_INPUT_MODE::FOOD;
				std::cout << "FOOD PLACEMENT: ON\n";
			}


			//===============MOUSE OBJECT PLACEMENT================//
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2i ClickPos = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(ClickPos);

				//ANT
				switch (placement_mode)
				{
				case MOUSE_INPUT_MODE::EMPTY:
					break;
				case MOUSE_INPUT_MODE::ADDANT:
					Colony1.addAnt(worldPos);
					break;
				case MOUSE_INPUT_MODE::PHEROMONE:
					PheroTiles.addStrength(worldPos, 100.0f);
					break;
				case MOUSE_INPUT_MODE::PBLOCK:
					pblock_system.push_back(PathBlocker(worldPos, sf::Color::Blue, 30.0f));
					partition.updateCheckIndex(&pblock_system);
					break;
				case MOUSE_INPUT_MODE::FOOD:
					food_system.push_back(Food(worldPos, sf::Color::Green, 10));
					partition.addCheckIndex(food_system[food_system.size() - 1]);
					break;
				default:
					break;
				}
				{
				}


			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{

				sf::Vector2i ClickPos = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(ClickPos);

			}

			//===============STRING COMMAND================//
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode < 128 && event.text.unicode != 13 && event.text.unicode != 8)
				{
					char tmpChar{ static_cast<char>(event.text.unicode) };
					USER_INPUT.push_back(tmpChar);
					std::cout << tmpChar;
				}
			}

			if ((!USER_INPUT.empty()) && (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)))
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
						//Colony1.addAnt(sf::Vector2f(GameSetting::windowWidth / 2, GameSetting::windowHeight/ 2));
						break;

					case TEXT_COMMAND::HELP:
						std::cout << command_list;
						break;

					case TEXT_COMMAND::ANTSIZE:
						std::cout << Colony1.getAntNum() << "\n";
						break;

					case TEXT_COMMAND::ANTSKIN:
						std::cout << Colony1.ant_skin << "\n";
						break;
					case TEXT_COMMAND::CLEARPHERO:
						PheroTiles.resetPheromone();
						break;
					default:
						std::cout << "COMMAND: Unknown";
					}
					std::cout << "\n";
					USER_INPUT.clear();

				}
			}
		}

		//////////////////////////////////////////////////////////////////////////////
		/////////////////////////	     END OF UI      //////////////////////////////
		//////////////////////////////////////////////////////////////////////////////

	//==========update frame==============//	
		timeElapsed = gameclock.restart().asSeconds() * GameSetting::GAMESPEED;
		Colony1.computeAntMove(timeElapsed);
		PheroTiles.pheromoneDecay(timeElapsed);
		//pheromones.decay(timeElapsed);

		//spatial partitioning update
		partition.clearPartition(); //clear all partition of ant pointers

		for (int i = 0; i < Colony1.getAntNum(); i++)
		{
			Colony1.AntContainer[i].collision_check = false;
			partition.updatePartition(&Colony1.AntContainer[i], Colony1.AntContainer[i].getPosition()); //add ant pointer to partition
		}
		partition.updateAntStatus(); //update ant collision check bool status


		//=========clear window =================//
		window.setView(view);
		window.clear(sf::Color::White);

		//========= draw window =================//
		if (show_pheromone) window.draw(PheroTiles);
		Colony1.drawColony(&window, show_sensor);
		window.draw(Chole);
		for (auto& n : pblock_system)
			window.draw(n);
		for (auto& f : food_system)
			window.draw(f);

		window.display();

		sf::View currentView = window.getView();
		randTime += timeElapsed;

	}

	return 0;
}