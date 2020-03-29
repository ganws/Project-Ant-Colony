#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class GameStateMachine
{
public:

	GameStateMachine();
	~GameStateMachine();

	void updateStat();
	void checkUserInput(sf::Event& event);

private:

	enum class GAMESTAT
	{
		MENU,
		OBJ_PLACEMENT,
		PAUSE,
		BACK,
		QUIT,
	};

};

