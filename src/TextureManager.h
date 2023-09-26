#pragma once
#include "Game.h"

struct pliz {
	SDL_Texture* tex;
	SDL_Rect dRect;
};

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* rend);
	static pliz LoadFont(const char* text, SDL_Renderer* rend, unsigned int size);
};
