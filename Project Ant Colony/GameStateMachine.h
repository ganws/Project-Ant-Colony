#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>	
#include <list>

class GameStateMachine
{
public:

	GameStateMachine();
	~GameStateMachine();

	void updateStat();
	void checkUserInput(sf::Event& event);

private:

	// trype of 
	enum class OBJECT_TO_PLACE
	{
		ANT,
		PHEROMONE,
		PBLOCK,
		FOOD
	};

	enum class GAMESTAT
	{
		MENU,
		OBJ_PLACEMENT,
		PAUSE,
		BACK,
		QUIT
	};

	std::list<GAMESTAT> stateStack;
};