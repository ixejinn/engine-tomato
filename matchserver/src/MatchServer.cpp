#include "MatchServer.h"

#include <iostream>

MatchServer::MatchServer(){};

void MatchServer::Run()
{

	while (isRunning_)
	{
		std::cout << "run" << '\n';
	}
};