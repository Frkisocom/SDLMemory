#pragma once
#include "Game.h"

class GameObject {
public:
	GameObject(unsigned int id, unsigned int broj, SDL_Renderer* rend);
	~GameObject();

	void place(unsigned int x, unsigned int y, float ratio, int wid);
	void Update();
	void Render();
	void Render(SDL_Texture* tex);
	unsigned int pairID;
	bool flipped, paired, inAnimation;
	SDL_Rect gdr() { return destRect; }
	void ClickedOn();

private:
	//int xpos, ypos;
	int maxW;
	unsigned int animFrames;
	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;
};