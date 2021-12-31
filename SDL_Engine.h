#pragma once
#ifndef SDL_ENGINE_H
#define SDL_ENGINE_H

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

class SDL_Engine
{

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	//Interval time between two frames (ms)
	float frameInterval;

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
			printf("SDL_Init: SDL could not initialize!\n");
			printf("SDL_Init: %s\n", SDL_GetError());
			return false;
		}

		//Create window
		window = SDL_CreateWindow("SDL Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);
		if (!window)
		{
			printf("SDL_CreateWindow: Window could not be created!\n");
			printf("SDL_CreateWindow: %s\n", SDL_GetError());
			return false;
		}

		if (!SDL_GL_SetSwapInterval(-1))
		{
			printf("SDL_GL_SetSwapInterval: Freesync not supported or enabled. Renabling Lock FPS again for that reason.\n");
			printf("SDL_GL_SetSwapInterval: %s\n", SDL_GetError());
			flag = SDL_RENDERER_PRESENTVSYNC;
		}

		renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED | flag);
		if (!renderer)
		{
			printf("SDL_CreateRenderer: Can't create renderer\n");
			printf("SDL_CreateRenderer: %s\n", SDL_GetError());
			return false;
		}
		SDL_RenderSetLogicalSize(renderer, width, height);

		//Clear everything on screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);

		//Init JPG and PNG
		int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
		int initted = IMG_Init(imgFlags);
		if ((initted & imgFlags) != imgFlags) 
		{
			printf("IMG_Init: Failed to init required jpg and png support!\n");
			printf("IMG_Init: %s\n", IMG_GetError());
		}

		//Init sound
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			printf("Mix_OpenAudio: SDL_mixer could not initialize!\n");
			printf("Mix_OpenAudio: %s\n", Mix_GetError());
		}

		//Calculate interval time
		frameInterval = 1000.0 / (float)fps;

		SDL_GetWindowSize(window, &width, &height);
		printf("Video OK!\nWidth=%d, Heigth=%d, fps=%d\n", width, height, fps);

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

	SDL_Texture* LoadImage(std::string filePath)
	{
		SDL_Texture* texture;
		SDL_Surface* surface;

		surface = IMG_Load(filePath.c_str());
		if (!surface)
		{
			printf("IMG_Load: Could not load image file.\n");
			printf("IMG_Load: %s\n", SDL_GetError());
			return nullptr;
		}

		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		return texture;
	}

	void DisplayImage(SDL_Texture* image,int x,int y)
	{
		SDL_Rect rect;
		int width, height;
		int result;
		result = SDL_QueryTexture(image, nullptr, nullptr, &width, &height);
		if (result)
		{
			printf("SDL_QueryTexture: Could not query the attributes of the texture.\n");
			printf("SDL_QueryTexture: %s\n", SDL_GetError());
		}

		rect.x = x;
		rect.y = y;
		rect.w = width;
		rect.h = height;
		result = SDL_RenderCopy(renderer, image, nullptr, &rect);
		if (result)
		{
			printf("SDL_RenderCopy: Could not copy the portion of the texture to the current rendering target.\n");
			printf("SDL_RenderCopy: %s\n", SDL_GetError());
		}
	}

	void UnloadImage(SDL_Texture* image)
	{
		if (image)
		{
			SDL_DestroyTexture(image);
			delete image;
		}
	}
};

#endif
