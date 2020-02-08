#pragma once

#include "GameSetting.h"
#include "Ant.h"
#include "VectorFunction.h"
#include "Animation.h"
#include "PheromoneSystem.h"
#include "PathBlocker.h"
#include "drawMapBoundary.h"

#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(GameSetting::windowWidth, GameSetting::windowHeight), "Colony");
	window.setFramerateLimit(GameSetting::FRAMERATE);
	sf::View view(sf::FloatRect(0.0f, 0.0f, GameSetting::windowWidth, GameSetting::windowHeight));
	sf::Clock gameclock;

	float timeElapsed{ 0.0f };
	float randTime{ 0.0f };

	//======CREATE PHEROMONE SYSTEM=====
	PheromoneSystem pheromones(10000); //fixed 10k particles. beyond that game will crash

	//======LOAD TEXTURE========//
	sf::Texture skin;
	if (!skin.loadFromFile("walk.png"))
		std::cout << "unable to load skin. \n";

	//======PATH BLOCKERS========//
	std::vector<PathBlocker> pblock_system;
	PathBlocker pblock1(sf::Vector2f(100.0f, 100.0f), sf::Color::Red, 30.0f);
	pblock_system.push_back(pblock1);

	drawMapBoundary(&pblock_system);

	
	//======CREATE ANTS========//
	Ant ant1(&pblock_system);
	ant1.initAnt(0.1, sf::Vector2f(GameSetting::windowHeight / 2, GameSetting::windowWidth / 2), skin);

	//======ANIMATION=========//

	Animation walk_animation(&skin, sf::Vector2u(8, 8), 62, 0.01);

	
	////=====WINDOW=======//
	while (window.isOpen())
	{
		float randT = static_cast<float>(rand() / static_cast<float>(RAND_MAX) * 3);
		if (randTime > randT)
		{
			float randLoc = static_cast<float>(rand() / static_cast<float>(RAND_MAX)* GameSetting::windowHeight);
			float randLoc2 = static_cast<float>(rand() / static_cast<float>(RAND_MAX)* GameSetting::windowHeight);
			//ant1.issue_move_command(sf::Vector2f(randLoc, randLoc2));
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				view.reset(sf::FloatRect(0.0f, 0.0f, GameSetting::windowWidth, GameSetting::windowHeight));

			// issue move command
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				
				sf::Vector2i ClickPos = sf::Mouse::getPosition(window);
				pheromones.createParticle(sf::Vector2f(ClickPos.x, ClickPos.y), Activity::GATHERING);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{

				sf::Vector2i ClickPos = sf::Mouse::getPosition(window);
				ant1.issue_move_command(sf::Vector2f(ClickPos.x,ClickPos.y));

			}

		}

		// update frame	
		timeElapsed = gameclock.restart().asSeconds() * GameSetting::GAMESPEED;

		ant1.updateMovement(timeElapsed);
		ant1.secretPheromon(timeElapsed, &pheromones);
		ant1.m_sensory_input.detectPheromone(&pheromones);
		
		walk_animation.Update(timeElapsed);
		ant1.setTextureRect(walk_animation.m_uvRect);
		pheromones.decay(timeElapsed);

		//clear window 
		window.setView(view);
		window.clear(sf::Color::White);

		//draw
		window.draw(ant1); 
		ant1.drawSensoryCircle(&window);
		window.draw(pheromones);
		for (auto n : pblock_system)
			window.draw(n);
		window.display();
		
		sf::View currentView = window.getView();
		randTime += timeElapsed;

	}

	return 0;
}