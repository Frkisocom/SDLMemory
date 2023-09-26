#include "GameObject.h"
#include "TextureManager.h"
#include "Game.h"
#include <string>

//extern int broj;

GameObject::GameObject(unsigned int id, unsigned int broj, SDL_Renderer* rend)
	:okrenut(0), sparen(0), uAnimaciji(0), animFrames(0)
{
	this->renderer = rend;
	if (id >= broj / 2)
		this->pairID = id - broj / 2;
	else
		this->pairID = id;
	std::string path = "res/textures/";
	path.append(std::to_string(pairID));
	path.append(".jpg");
	this->objTexture = TextureManager::LoadTexture(path.c_str(), renderer);
}
GameObject::~GameObject()
{}

void GameObject::plasiraj(unsigned int x, unsigned int y)
{
	//koordinate teksture
	srcRect.h = 1173;
	srcRect.w = 914;
	srcRect.x = 0;
	srcRect.y = 0;
	//velicina
	destRect.h = srcRect.h / 6;
	destRect.w = srcRect.w / 6;
	maxW = destRect.w;
	//pozicija
	int loff=100, uoff=30, padding=20;
	destRect.x = loff+(padding+destRect.w)*x;
	destRect.y = uoff+(padding+destRect.h)*y;
}
void GameObject::Kliknut()
{
	uAnimaciji = true;
}
void GameObject::Update()
{
	if (uAnimaciji)
	{
		if (animFrames <= 30) {
			destRect.x = destRect.x + maxW / 60;
			destRect.w = destRect.w - maxW / 30;
			if (animFrames == 30)
			{
				okrenut = !okrenut;
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
			uAnimaciji = false;
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