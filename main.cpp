#include <stdio.h>

#include "SDL_Engine.h"
int main(int argc, char* argv[])
{
	SDL_Engine engine;
	if (!engine.Init(640,480,false,60))
	{
		printf("Failed to create window, exit\n");
		return 0;
	}

	bool quit = false;
	while (!quit)
	{
		engine.PollEvent();
		if (engine.exit)
		{
			quit = true;
			break;
		}

		int delay = engine.Render();
		//printf("Inter-frame delay=%dms\n", delay);
	}

	engine.Destory();
	return 0;
}