#include "GameObject.h"
#include "TextureManager.h"
#include "Game.h"
#include <string>

//extern int broj;

GameObject::GameObject(unsigned int id, unsigned int num, SDL_Renderer* rend)
	:flipped(false), paired(0), inAnimation(0), animFrames(0)
{
	srcRect.h = 1173;
	srcRect.w = 914;
	srcRect.x = 0;
	srcRect.y = 0;
	this->renderer = rend;
	if (id >= num / 2)
		this->pairID = id - num / 2;
	else
		this->pairID = id;
	std::string path = "res/textures/";
	path.append(std::to_string(pairID));
	path.append(".jpg");
	this->objTexture = TextureManager::LoadTexture(path.c_str(), renderer);
}
GameObject::~GameObject()
{}

void GameObject::place(unsigned int x, unsigned int y, unsigned int ratio, int wid)
{
	destRect.h = srcRect.h / 1.5 / ratio;
	destRect.w = srcRect.w / 1.5 / ratio;
	maxW = destRect.w;
	//pozicija
	int uoff=30, padding= 60 / ratio;
	destRect.x = (padding+destRect.w)*x+wid;
	destRect.y = uoff+(padding+destRect.h)*y;
}
void GameObject::ClickedOn()
{
	inAnimation = true;
}
void GameObject::Update()
{
	if (inAnimation)
	{
		if (animFrames <= 30) {
			destRect.x = destRect.x + maxW / 60;
			destRect.w = destRect.w - maxW / 30;
			if (animFrames == 30)
			{
				flipped = !flipped;
			}
			animFrames++;
		}
		else if (animFrames <= 60)
		{
			destRect.x = destRect.x - maxW / 60;
			destRect.w = destRect.w + maxW / 30;
			animFrames++;
		}
		else
		{
			//destRect.w = maxW;
			inAnimation = false;
			animFrames = 1;
		}
	}
}
void GameObject::Render()
{
	SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}
void GameObject::Render(SDL_Texture* tex)
{
	SDL_RenderCopy(renderer, tex, &srcRect, &destRect);
}