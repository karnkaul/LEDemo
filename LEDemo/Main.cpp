#include "Game/GameLoop.h"
#include "Gameplay/GameInit.h"

using namespace LittleEngine;

int main(int argc, char** argv)
{
	if (!GameInit::Init(argc, argv))
	{
		LOG_E("Fatal Error initialising GameLoop!");
		return 1;
	}

	GameLoop::g_createWorlds = &GameInit::CreateWorlds;
	GameLoop::g_loadShaders = &GameInit::LoadShaders;
	return GameLoop::Run();
}
