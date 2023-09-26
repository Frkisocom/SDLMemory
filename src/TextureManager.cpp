#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName, SDL_Renderer* rend) 
{
	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

pliz TextureManager::LoadFont(const char* text, SDL_Renderer* rend, unsigned int size)
{
	TTF_Font* font = TTF_OpenFont("res/fonts/arial.ttf", size);
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface* tempSurface = TTF_RenderText_Solid(font, text, color);
	SDL_Rect dRect;
	dRect.w = tempSurface->w;
	dRect.h = tempSurface->h;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, tempSurface);
	TTF_CloseFont(font);
	SDL_FreeSurface(tempSurface);
	pliz paket;
	paket.tex = tex;
	paket.dRect = dRect;
	return paket;
}