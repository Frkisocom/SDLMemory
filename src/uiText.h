#pragma once
#include "Game.h"
#include "TextureManager.h"


class UIText {
public:
	UIText(SDL_Renderer* rend, const char* content, int x, int y, unsigned int size);
	~UIText();
	void Update();
	void Render();
	void ClickedOn();
	void UpdateTexture(const char* content, unsigned int size);
	SDL_Rect destRect;
private:
	SDL_Texture* objTexture;
	SDL_Renderer* renderer;
};