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
Font_Data* globalFont;

void KeyPress(int scanCode, int keyCode, bool isPress);
int main(int argc, char* argv[])
{
	if (!engine.Init(640,480,false,"Demo",60))
	{
		printf("Failed to create window, exit\n");
		return 0;
	}
	
	//Bind key press event
	engine.keyPressEventHandler = &KeyPress;

	//Load image

	//Load front layer with transparent color
	Color_Map black = { 0,0,0 };
	Image_Data* frontLayer = engine.LoadImage("./assests/front.bmp", &black);
	//Load bottom layer
	Image_Data* bottomLayer = engine.LoadImage("./assests/bottom.bmp");

	//Load background music and play (loops)
	//engine.LoadMusic("./assests/th06_01.mp3");
	//engine.PlayMusic(true);

	//Load SFX and play
	//sfx = engine.LoadSFX("./assests/pause.wav");
	//engine.PlaySFX(sfx);

	//Load Global Font
	globalFont = engine.LoadFont("./assests/zpix.ttf", 16);
	//Load Text
	Image_Data* textHello = engine.LoadText(globalFont, "Hello World!");
	Image_Data* textFPS = engine.LoadText(globalFont, "Inter-frame delay=-1ms");

	bool quit = false;
	while (!quit)
	{
		//Display Image
		engine.DisplayImage(bottomLayer, 0, 0);
		engine.DisplayImage(frontLayer, 160, 120);

		//Display Text
		engine.DisplayImage(textHello, 16, 16);
		engine.DisplayImage(textFPS,16,32);


		engine.PollEvent();
		if (engine.exit)
		{
			quit = true;
			break;
		}

		int delay = engine.Render();
		//printf("Inter-frame delay=%dms\n", delay);
		engine.UnloadText(textFPS);
		textFPS = engine.LoadText(globalFont, "Inter-frame delay=" + std::to_string(delay) + "ms");
	}

	//Unload assests
	engine.UnloadImage(frontLayer);
	engine.UnloadImage(bottomLayer);
	engine.UnloadText(textHello);
	engine.UnloadText(textFPS);
	engine.UnloadFont(globalFont);
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