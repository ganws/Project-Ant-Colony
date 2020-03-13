#include "Hashit.h"

TEXT_COMMAND Parse(std::string const& StringIn)
{
	TEXT_COMMAND command = TEXT_COMMAND::ERROR; //default

	if (StringIn == "-addant")
		command = TEXT_COMMAND::CREATE_ANT;
	if (StringIn == "-exit")
		command =  TEXT_COMMAND::EXIT;
	if (StringIn == "-h")
		command = TEXT_COMMAND::HELP;
	if (StringIn == "-antnum")
		command = TEXT_COMMAND::ANTNUM;
	if (StringIn == "-skin")
		command = TEXT_COMMAND::ANTSKIN;

	return command;
}