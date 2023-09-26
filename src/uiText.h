#pragma once
#include "Game.h"
#include "TextureManager.h"


class UIText {
public:
	UIText(SDL_Renderer* rend, const char* content, int x, int y, unsigned int size);
	~UIText();
	void Update();
	void Render();
	void Kliknut();
	void UpdateTexture(const char* content, unsigned int size);
private:
	SDL_Texture* objTexture;
	SDL_Rect destRect;
	SDL_Renderer* renderer;
};