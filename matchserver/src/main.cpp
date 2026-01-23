#include "MatchServer.h"
#include "MatchManager.h"
#include "SessionManager.h"

#include <iostream>
int main()
{
	//MatchServer server;
	//server.Run();

	MatchManager mm;
	SessionManager sm;

	//mm.AddMatchRequest(0);
	//mm.AddMatchRequest(1);
	//mm.AddMatchRequest(2);
	//mm.AddMatchRequest(3);
	//mm.AddMatchRequest(4);
	//mm.AddMatchRequest(5);
	//mm.AddMatchRequest(6);
	//mm.AddMatchRequest(7);
	//mm.AddMatchRequest(8);

	while (true)
	{
		mm.Update(0.016f);
		sm.Update();
	}

	return 0;
}