#include "uiText.h"

UIText::UIText(SDL_Renderer* rend, const char* content, int x, int y, unsigned int size)
{
	pliz paket = TextureManager::LoadFont(content, rend, size);
	objTexture = paket.tex;
	destRect = paket.dRect;
	destRect.x = x;
	destRect.y = y;
	renderer = rend;
}
UIText::~UIText()
{

}
void UIText::Update()
{

}
void UIText::UpdateTexture(const char* content, unsigned int size)
{
	pliz paket = TextureManager::LoadFont(content, renderer, size);
	objTexture = paket.tex;
	destRect.w = paket.dRect.w;
	destRect.h = paket.dRect.h;
}
void UIText::Render()
{
	SDL_RenderCopy(renderer, objTexture, NULL, &destRect);
}
void UIText::ClickedOn()
{

}