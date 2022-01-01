#include "SDL_Engine.h"

bool SDL_Engine::Init(int width, int height, bool fullScreen, std::string title, int fps)
{
	//Initialization flag
	int flag = 0;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("SDL_Init: SDL could not initialize!\n");
		printf("Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("SDL_CreateWindow: Window could not be created!\n");
		printf("Error: %s\n", SDL_GetError());
		return false;
	}

	//Maybe not necessary?
	//if (SDL_GL_SetSwapInterval(-1))
	//{
	//	printf("SDL_GL_SetSwapInterval: Freesync not supported or enabled. Renabling Lock FPS again for that reason.\n");
	//	printf("Error: %s\n", SDL_GetError());
	//	flag = SDL_RENDERER_PRESENTVSYNC;
	//}

	renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED | flag);
	if (!renderer)
	{
		printf("SDL_CreateRenderer: Can't create renderer\n");
		printf("Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_RenderSetLogicalSize(renderer, width, height);

	//Clear everything on screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	//Init Image for JPG and PNG
	int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(imgFlags);
	if ((initted & imgFlags) != imgFlags)
	{
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("Error: %s\n", IMG_GetError());
	}

	//Init Mix
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048))
	{
		printf("Mix_OpenAudio: SDL_mixer could not initialize!\n");
		printf("Error: %s\n", Mix_GetError());
	}

	//Init TTF
	if (TTF_Init())
	{
		printf("TTF_Init: SDL_ttf could not initialize!\n");
		printf("Error: %s\n", TTF_GetError());
	}

	//Calculate interval time
	frameInterval = 1000.0 / (double)fps;

	SDL_GetWindowSize(window, &width, &height);
	printf("Video OK!\nWidth=%d, Heigth=%d, FPS=%d\n", width, height, fps);

	return true;
}

int SDL_Engine::Render()
{
	Uint64 start = SDL_GetTicks64();
	int delay;

	//Rendering
	SDL_RenderPresent(renderer);

	//Calculate inter-frame delay
	delay = SDL_GetTicks() - start;

	//Lock FPS to setting
	if (SDL_GetTicks64() - start < frameInterval)
	{
		SDL_Delay(frameInterval - (SDL_GetTicks() - start));
	}

	return delay;
}

void SDL_Engine::Destory()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	//Quit subsystem
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void SDL_Engine::PollEvent()
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

Font_Data* SDL_Engine::LoadFont(std::string path, int size)
{
	Font_Data* font = TTF_OpenFont(path.c_str(), size);
	if (!font) {
		printf("TTF_OpenFont: Fail to load font from %s\n", path.c_str());
		printf("Error: %s\n", TTF_GetError());
	}
	return font;
}

void SDL_Engine::UnloadFont(Font_Data* font)
{
	TTF_CloseFont(font);
}

Image_Data* SDL_Engine::LoadText(Font_Data* font, std::string text, Color_Map* fontColor)
{
	Image_Data* texture;
	SDL_Surface* surface;

	//Set default Color to Black
	if (!fontColor)
	{
		fontColor = new Color_Map{ 0,0,0 };
	}

	surface = TTF_RenderText_Solid(font, text.c_str(), *fontColor);
	if (!surface)
	{
		printf("TTF_RenderText_Solid: Could not render text: %s.\n", text.c_str());
		printf("Error: %s\n", TTF_GetError());
		return nullptr;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}

Image_Data* SDL_Engine::LoadImage(std::string path, Color_Map* transparentColor)
{
	Image_Data* texture;
	SDL_Surface* surface;

	surface = IMG_Load(path.c_str());
	if (!surface)
	{
		printf("IMG_Load: Could not load image file from %s.\n", path.c_str());
		printf("Error: %s\n", SDL_GetError());
		return nullptr;
	}

	//If set transparent color
	if (transparentColor)
	{
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, transparentColor->r, transparentColor->g, transparentColor->b));
	}

	////(Bug?)stretchSurface is always NULL
	//if (width >= 0 || height >= 0)
	//{
	//	//Set stretch destination
	//	SDL_Surface* stretchSurface = new SDL_Surface();
	//	SDL_Rect stretchRect;
	//	stretchRect.x = 0;
	//	stretchRect.y = 0;
	//	stretchRect.w = (width >= 0) ? width : surface->w;
	//	stretchRect.h = (height >= 0) ? height : surface->h;
	//	int flag = SDL_BlitScaled(surface, nullptr, stretchSurface, &stretchRect);

	//	SDL_FreeSurface(surface);
	//	surface = stretchSurface;
	//}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}

void SDL_Engine::DisplayImage(Image_Data* image, int x, int y)
{
	SDL_Rect rect;
	int width, height;
	int result;
	result = SDL_QueryTexture(image, nullptr, nullptr, &width, &height);
	if (result)
	{
		printf("SDL_QueryTexture: Could not query the attributes of the texture.\n");
		printf("Error: %s\n", SDL_GetError());
	}

	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	result = SDL_RenderCopy(renderer, image, nullptr, &rect);
	if (result)
	{
		printf("SDL_RenderCopy: Could not copy the portion of the texture to the current rendering target.\n");
		printf("Error: %s\n", SDL_GetError());
	}
}

void SDL_Engine::UnloadImage(Image_Data* image)
{
	if (image)
	{
		SDL_DestroyTexture(image);
	}
}

bool SDL_Engine::LoadMusic(std::string path)
{
	//Only one Mix_Music can be played at the time
	if (music)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path.c_str());
	if (!music)
	{
		printf("Mix_LoadMUS: Failed to load music from %s.\n", path.c_str());
		printf("Error: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

bool SDL_Engine::PlayMusic(bool loop)
{
	int result = Mix_PlayMusic(music, loop ? -1 : 0);
	if (result)
	{
		printf("Mix_PlayMusic: Failed to play music!\n");
		printf("Error: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

void SDL_Engine::PauseMusic()
{
	Mix_PauseMusic();
}

void SDL_Engine::ResumeMusic()
{
	Mix_ResumeMusic();
}

void SDL_Engine::UnloadMusic()
{
	Mix_HaltMusic();
	Mix_FreeMusic(music);
}

SFX_Data* SDL_Engine::LoadSFX(std::string path)
{
	SFX_Data* SFX = Mix_LoadWAV(path.c_str());
	if (!SFX)
	{
		printf("Mix_LoadWAV: Failed to load SFX from %s.\n", path.c_str());
		printf("Error: %s\n", Mix_GetError());
		return nullptr;
	}
	return SFX;
}

bool SDL_Engine::PlaySFX(SFX_Data* SFX, int playback)
{
	int result = Mix_PlayChannel(-1, SFX, playback);
	if (result)
	{
		printf("Mix_PlayChannel: Failed to play SFX!\n");
		printf("Error: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

void SDL_Engine::UnloadSFX(SFX_Data* SFX)
{
	Mix_FreeChunk(SFX);
}