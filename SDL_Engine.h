#pragma once
#ifndef SDL_ENGINE_H
#define SDL_ENGINE_H

#include <stdio.h>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>
class SDL_Engine
{

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	//Interval time between two frames (ms)
	float frameInterval;

	//const unsigned int keysCode[KEY_COUNT] =
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

public:
	bool exit = false;

	//Create delegate for key press or up
	typedef void (*KeyPressEvent) (int scanCode,int keyCode, bool isPress);
	KeyPressEvent keyPressEventHandler;

	//Create delegate for joystick position change
	//int joystickEventHandler;

	bool Init(int width, int height, bool fullScreen=true,int fps=60)
	{
		//Initialization flag
		int flag = 0;

		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			return false;
		}

		//Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);
		if (!window)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}

		if (!SDL_GL_SetSwapInterval(-1))
		{
			printf("Freesync not supported or enabled. Renabling Lock FPS again for that reason.\n");
			flag = SDL_RENDERER_PRESENTVSYNC;
		}

		renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED | flag);
		if (!renderer)
		{
			printf("Can't create renderer\n");
			return false;
		}
		SDL_RenderSetLogicalSize(renderer, width, height);

		// Clear everything on screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);

		//Calculate interval time
		frameInterval = 1000.0 / (float)fps;

		SDL_GetWindowSize(window, &width, &height);
		printf("Video OK ! Width %d, Heigth %d\n", width, height);

		return true;
	}

	int Render()
	{
		Uint64 start;
		Uint64 delay;
		start = SDL_GetTicks64();

		//Rendering
		SDL_RenderPresent(renderer);

		//Calculate inter-frame delay
		delay = SDL_GetTicks() - start;

		//Lock FPS to setting
		if (SDL_GetTicks64() - start < frameInterval)
		{
			SDL_Delay(frameInterval - (SDL_GetTicks() - start));
		}

		return (int)delay;
	}

	void Destory()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void PollEvent()
	{
		SDL_Event event;
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		//Handle events on queue
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
				{
					exit = true;
					break;
				}
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					//Call delegate
					keyPressEventHandler(
						event.key.keysym.scancode, 
						event.key.keysym.sym, 
						event.type == SDL_KEYDOWN);

					break;
				}
			}
		}
	}
};
#endif
