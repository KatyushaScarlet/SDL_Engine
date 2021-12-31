#include <iostream>

#include "SDL_Engine.h"

//const unsigned int keysCode[15] =
//{
//	SDL_SCANCODE_UP,
//	SDL_SCANCODE_DOWN,
//	SDL_SCANCODE_LEFT,
//	SDL_SCANCODE_RIGHT,
//	SDL_SCANCODE_LCTRL,//A
//	SDL_SCANCODE_LALT,//B
//	SDL_SCANCODE_LSHIFT,//Y
//	SDL_SCANCODE_SPACE,//X
//	SDL_SCANCODE_TAB,//L1
//	SDL_SCANCODE_BACKSPACE,//R1
//	SDL_SCANCODE_RETURN,//START
//	SDL_SCANCODE_ESCAPE,//SELECT
//	SDL_SCANCODE_PAGEUP,//L2
//	SDL_SCANCODE_PAGEDOWN//R2
//};

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

	SDL_Texture* sample = engine.LoadImage("./hero.png");

	bool quit = false;
	while (!quit)
	{
		engine.DisplayImage(sample, 0, 0);

		engine.PollEvent();
		if (engine.exit)
		{
			quit = true;
			break;
		}

		int delay = engine.Render();
		//printf("Inter-frame delay=%dms\n", delay);
	}

	engine.UnloadImage(sample);
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