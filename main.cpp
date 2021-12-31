#include <stdio.h>

#include "SDL_Engine.h"

void KeyPress(int scanCode, int keyCode, bool isPress);
int main(int argc, char* argv[])
{
	SDL_Engine engine;
	if (!engine.Init(640,480,false,60))
	{
		printf("Failed to create window, exit\n");
		return 0;
	}

	engine.keyPressEventHandler = &KeyPress;

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

void KeyPress(int scanCode, int keyCode, bool isPress)
{
	if (isPress)
	{
		switch (scanCode)
		{
		case SDL_SCANCODE_UP:
			printf("up\n");
			break;
		case SDL_SCANCODE_DOWN:
			printf("down\n");
			break;
		case SDL_SCANCODE_LEFT:
			printf("left\n");
			break;
		case SDL_SCANCODE_RIGHT:
			printf("right\n");
			break;
		default:
			break;
		}
	}
}