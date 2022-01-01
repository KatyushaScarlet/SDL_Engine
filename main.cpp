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
SDL_Engine engine;
SFX_Data* sfx;

void KeyPress(int scanCode, int keyCode, bool isPress);
int main(int argc, char* argv[])
{
	if (!engine.Init(640,480,false,60))
	{
		printf("Failed to create window, exit\n");
		return 0;
	}
	
	//Bind key press event
	engine.keyPressEventHandler = &KeyPress;

	//Load image
	Image_Data* image = engine.LoadImage("./assests/th06_covor.jpg");

	//Load background music and play (loops)
	engine.LoadMusic("./assests/th06_01.mp3");
	engine.PlayMusic(true);

	//Load SFX and play
	sfx = engine.LoadSFX("./assests/pause.wav");
	engine.PlaySFX(sfx);

	bool quit = false;
	while (!quit)
	{
		//Display
		engine.DisplayImage(image, 0, 0);

		engine.PollEvent();
		if (engine.exit)
		{
			quit = true;
			break;
		}

		int delay = engine.Render();
		//printf("Inter-frame delay=%dms\n", delay);
	}

	//Unload assests
	engine.UnloadImage(image);
	engine.UnloadMusic();
	engine.UnloadSFX(sfx);

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