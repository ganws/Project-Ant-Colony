#pragma once

#include "GameSetting.h"
#include "Ant.h"
#include "VectorFunction.h"
#include "Animation.h"
#include "PathBlocker.h"
#include "drawMapBoundary.h"
#include "Colony.h"
#include "Hashit.h"
#include "PheroMatrix.h"
#include "SpatialPartition.h"
#include "Food.h"
#include "Astar.h"
#include "Terrain.h"

#include<fstream>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/OpenGL.hpp>
#include<iostream>
#include<SFML/System/Vector2.hpp>
#include<string>


int main()
{

	//////////////////////////////////////////////////////////////////////////////
	////////////////////////////   INITIALIZATION   //////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	sf::RenderWindow window(sf::VideoMode(GameSetting::windowWidth, GameSetting::windowHeight), "Colony");
	window.setFramerateLimit(GameSetting::FRAMERATE);
	sf::View view(sf::FloatRect(0.0f, 0.0f, float(GameSetting::windowWidth), float(GameSetting::windowHeight)));
	float zoom_factor{1.0};
	sf::Clock gameclock;

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
	bool ALT_PRESSED{ false };
	bool CTRL_PRESSED{ false };

	//=============Game state display========================//
	sf::Font fontArial;
	fontArial.loadFromFile("../Resource/arial.ttf");

	sf::Text stat_antnum;
	stat_antnum.setString("Colony Size:");
	stat_antnum.setFont(fontArial);
	stat_antnum.setFillColor(sf::Color::Black);
	stat_antnum.setCharacterSize(25);
	stat_antnum.setPosition(float(GameSetting::windowWidth - 500), float(10));

	sf::Text stat_resource;
	stat_resource.setString("Resource:");
	stat_resource.setFont(fontArial);
	stat_resource.setFillColor(sf::Color::Black);
	stat_resource.setCharacterSize(25);
	stat_resource.setPosition(float(GameSetting::windowWidth - 200), float(10));


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
	drawMapBoundary(&pblock_system);

	//=======TERRAIN SYSTEM========//
	Terrain terrain_system(GameSetting::windowWidth, GameSetting::windowHeight, sf::Vector2u(300,300));
	for (auto &pb : pblock_system)
		terrain_system.updateCoeff(pb);

	//========LOAD SPATIAL PARTITIONING OBJECT===========//
	SpatialPartition partition;
	partition.initSpatialPartition(GameSetting::windowWidth, GameSetting::windowHeight, sf::Vector2u(20, 20));
	partition.updateCheckIndex(&pblock_system);

	//========LOAD A* SYSTEM===========//
	
	Astar AstarSystem;
	sf::Vector2u res(40, 40);
	AstarSystem.initAstar(GameSetting::windowWidth, GameSetting::windowHeight, res, true);
	AstarSystem.updateObstacleNode(&pblock_system);
	Node* startNode = &AstarSystem.m_Nodes[0];
	Node* goalNode = &AstarSystem.m_Nodes[res.x * res.y - 1];
	std::list <Node*> shortestPath;
	shortestPath = AstarSystem.computePath(startNode, goalNode);

	Astar TilePath_Rough;
	sf::Vector2u res2(40, 40);
	TilePath_Rough.initAstar(GameSetting::windowWidth, GameSetting::windowHeight, res2, true);
	TilePath_Rough.updateObstacleNode(&pblock_system);
	Node* start_node_rough = &TilePath_Rough.m_Nodes[0];
	Node* goal_node_rough = &TilePath_Rough.m_Nodes[res2.x * res2.y - 1];
	std::list <Node*> shortestPath_rough;	
	shortestPath_rough = TilePath_Rough.computePath(start_node_rough, goal_node_rough);

	//======FOOD OBJECT=============//
	std::vector<Food> food_system;
	std::vector<sf::Vector2f> foodpath;
	Food* newfood_ptr{nullptr};
	//======CREATE COLONY==========//
	Colony Colony1;
	Colony1.initColony(&pblock_system, &skin, &PheroTiles, &partition, &food_system, &terrain_system);

	//======COLONY HOLE===========//
	sf::CircleShape Chole;
	Chole.setFillColor(sf::Color::Black);
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
				view.move(0.0f, 20.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				view.move(0.0f, -20.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				view.move(-20.0f, 0.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				view.move(20.0f, 0.0f);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				view.reset(sf::FloatRect(0.0f, 0.0f, float(GameSetting::windowWidth), float(GameSetting::windowHeight)));

			//======TOGGLE ALT==============//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::LAlt))
			{
				ALT_PRESSED = true;
				//std::cout << "ALT: " << ALT_PRESSED << "\n";
			}

			if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::LAlt))
			{
				ALT_PRESSED = false;
				//std::cout << "ALT: " << ALT_PRESSED << "\n";
			}

			//======TOGGLE CTRL==============//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::LControl))
			{
				CTRL_PRESSED = true;
				//std::cout << "CTRL: " << CTRL_PRESSED << "\n";
			}

			if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::LControl))
			{
				CTRL_PRESSED = false;
				//std::cout << "CTRL: " << CTRL_PRESSED << "\n";
			}

			//======TOGGLE PHEROMONE VISUAL==============//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Z))
			{
				show_pheromone = !show_pheromone;
				std::cout << "PHEROMONE VISUAL: " << show_pheromone << "\n";
			}

			//======TOGGLE SENSOR VISUAL==============//
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
			{
				Colony1.dispSensor = !Colony1.dispSensor;
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
				if (ALT_PRESSED)
					placement_mode = MOUSE_INPUT_MODE::S_NODE;
				if (CTRL_PRESSED)
					placement_mode = MOUSE_INPUT_MODE::E_NODE;

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
					terrain_system.updateCoeff(pblock_system.back());
					partition.updateCheckIndex(&pblock_system);
					AstarSystem.updateObstacleNode(&pblock_system);
					TilePath_Rough.updateObstacleNode(&pblock_system);
					shortestPath = AstarSystem.computePath(startNode, goalNode);
					break;

				case MOUSE_INPUT_MODE::FOOD:
					food_system.push_back(Food(worldPos, sf::Color::Green, 10));
					partition.addCheckIndex(food_system[food_system.size() - 1]);
					
					//compute path to colony hole
					//newfood_ptr = &food_system.back();
					//start_node_rough = TilePath_Rough.getNode(newfood_ptr->getPosition()); //node position of food
					//goal_node_rough = TilePath_Rough.getNode(Chole.getPosition()); //node position of colony
					//shortestPath_rough = TilePath_Rough.computePath(start_node_rough, goal_node_rough); //compute shortest path in Node position
					//foodpath = TilePath_Rough.getPathFromNode(shortestPath_rough);
					//newfood_ptr->storePath(foodpath);
					//
					//std::cout << "Food to colony path = \n";
					//for (auto path : foodpath)
					//	printf("[%f][%f]\n", path.x, path.y);
					break;
				//case MOUSE_INPUT_MODE::S_NODE:
				//	startNode = AstarSystem.getNode(worldPos);
				//	//printf("start = [%d][%d]\n", startNode->x, startNode->y);
				//	shortestPath = AstarSystem.computePath(startNode, goalNode);
				//	placement_mode = MOUSE_INPUT_MODE::EMPTY;
				//	break;
				//case MOUSE_INPUT_MODE::E_NODE:
				//	goalNode = AstarSystem.getNode(worldPos);
				//	shortestPath = AstarSystem.computePath(startNode, goalNode);
				//	//printf("goal = [%d][%d]\n", goalNode->x, goalNode->y);
				//	placement_mode = MOUSE_INPUT_MODE::EMPTY;
				//	break;
				default:
					break;
				}

			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{

				sf::Vector2i ClickPos = sf::Mouse::getPosition(window);
				sf::Vector2f worldPos = window.mapPixelToCoords(ClickPos);

			}

			//======ZOOM======//
			if (event.type == sf::Event::MouseWheelMoved)
			{
				float maxZoom = 2.0;
				float minZoom = -2.0;
				
				zoom_factor += static_cast<float>(event.mouseWheel.delta) * -0.2f;
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
		if (timeElapsed > 1 / 20.0f)
			timeElapsed = 1 / 20.0f;
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

		//game stat update
		partition.updateAntStatus(); //update ant collision check bool status
		std::string colonysize = "Colony size: " + std::to_string(Colony1.getAntNum());
		stat_antnum.setString(colonysize);

		std::string resourcesize = "Resource: " + std::to_string(Colony1.getResourceAmount());
		stat_resource.setString(resourcesize);


		//=========clear window =================//
		view.zoom(zoom_factor);
		window.setView(view);
		window.clear(sf::Color::White);
		zoom_factor = 1;

		//========= draw window =================//
		window.draw(stat_antnum);
		window.draw(stat_resource);
		if (show_pheromone) 
			window.draw(PheroTiles);
		Colony1.drawColony(window);
		window.draw(Chole);
		//window.draw(terrain_system);
		//window.draw(AstarSystem);
		//window.draw(TilePath_Rough);
		for (auto& n : pblock_system)
			window.draw(n);
		for (int i = 0; i < food_system.size(); i++)
		{
			window.draw(food_system[i]);
			if (food_system[i].depleted)
			{
				food_system.erase(food_system.begin()+i);
			}
		}

		window.display();

		sf::View currentView = window.getView();
		randTime += timeElapsed;

	}

	return 0;
}