#include<iostream>

enum class TEXT_COMMAND
{
	EXIT = 0,
	CREATE_ANT = 1,
	ERROR = 2,
	HELP = 3,
	ANTSIZE = 4,
	ANTSKIN = 5,
	CLEARPHERO = 6
};

enum class MOUSE_INPUT_MODE
{
	EMPTY = 0,
	ADDANT = 1,
	PHEROMONE = 2,
	PBLOCK = 3,
	FOOD = 4
};

TEXT_COMMAND Parse(std::string const & StringIn);
