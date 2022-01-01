#pragma once
#ifndef SDL_ENGINE_H
#define SDL_ENGINE_H

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

//Class
#define Image_Data SDL_Texture
#define SFX_Data Mix_Chunk
#define Font_Data TTF_Font
#define Color_Map SDL_Color
#define Direction SDL_Rect

//Function
#define DisplayText DisplayImage
#define UnloadText UnloadImage

class SDL_Engine
{

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Mix_Music* music;

	//Interval time between two frames (ms)
	double frameInterval;

public:
	bool exit = false;

	//Create delegate for key press or up
	typedef void (*KeyPressEvent) (int scanCode,int keyCode, bool isPress);
	KeyPressEvent keyPressEventHandler;

	//Create delegate for joystick position change
	//int joystickEventHandler;

	//Baisc
	bool Init(int width, int height, bool fullScreen = true, std::string title = "SDL Engine", int fps = 60);
	int Render();
	void Destory();
	void PollEvent();

	//TTF
	Font_Data* LoadFont(std::string path, int size);
	void UnloadFont(Font_Data* font);
	Image_Data* LoadText(Font_Data* font, std::string text, Color_Map* fontColor = nullptr);

	//Image
	Image_Data* LoadImage(std::string path, Color_Map* transparentColor = nullptr);
	void DisplayImage(Image_Data* image, int left, int top);
	void UnloadImage(Image_Data* image);

	//Music
	bool LoadMusic(std::string path);
	bool PlayMusic(bool loop);
	void PauseMusic();
	void ResumeMusic();
	void UnloadMusic();

	//SFX
	SFX_Data* LoadSFX(std::string path);
	bool PlaySFX(SFX_Data* SFX, int playback = 0);
	void UnloadSFX(SFX_Data* SFX);

	//Geometry Rendering
	void DrawRectangle(int left, int top, int width, int height, Color_Map* color = nullptr, bool fill = true);
	void DrawLine(int left1, int top1, int left2, int top2, Color_Map* color = nullptr);
	void DrawDot(int left, int top, Color_Map* color = nullptr);
	void DrawCircle(int centerLeft, int centreTop, int radius, Color_Map* color = nullptr, bool fill = true);
};

#endif
