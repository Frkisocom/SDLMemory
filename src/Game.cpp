#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "uiText.h"
#include <vector>
#include <algorithm>
#include <string>

SDL_Event Game::event;
std::vector<GameObject*> SveKartice;
std::vector<UIText*> SviTekstovi;
UIText* rowT;
UIText* colT;
int broj = 0;
SDL_Texture* defaultTexture;
//koordinate miša
int x, y;
bool radiMis = true;
unsigned int delay = 110;
unsigned int ctr = 0;
int activeID[] = { -1, -1 };
int brojK = 0;
int state = 0;
int row = 4, col = 4, selected = 0;
//Non member funkcije
bool collision(SDL_Rect rec) {
	return (rec.x<x && rec.x + rec.w>x && rec.y<y && rec.y + rec.h>y);
}

void spari()
{
	if (activeID[1] != -1)
	{
		if (delay == 60) {
			ctr = 0;
			radiMis = false;
		}
		if (delay == 0) {
			if (SveKartice[activeID[0]]->pairID == SveKartice[activeID[1]]->pairID)
			{
				SveKartice[activeID[0]]->sparen = true;
				SveKartice[activeID[1]]->sparen = true;
			}
			else
			{
				SveKartice[activeID[0]]->Kliknut();
				SveKartice[activeID[1]]->Kliknut();
			}
			activeID[0] = -1;
			activeID[1] = -1;
			delay = 120;
		}
		else
			delay--;

	}
}

void Game::assign(int i)
{
	if (selected == 1)
	{
		row = i;
		rowT->UpdateTexture(std::to_string(i).c_str(), 40);
	}
	else if (selected == 2)
	{
		col = i;
		colT->UpdateTexture(std::to_string(i).c_str(), 40);
	}
}

Game::Game() {

}

Game::~Game() {

}


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems initialised!..." << std::endl;
		TTF_Init();
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
			std::cout << "Window created!..." << std::endl;
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 115, 167, 250, 255);
			std::cout << "Renderer created!..." << std::endl;
		}
		isRunning = true;
	}
	else
		isRunning = false;

	//Main Menu inicijalizacija
	rowT = new UIText(renderer, "4", 830, 320, 40);
	colT = new UIText(renderer, "4", 830, 450, 40);
	SviTekstovi.push_back(new UIText(renderer, "Memory igra - glavni meni", 200, 100, 72));
	SviTekstovi.push_back(new UIText(renderer, "Napravio Fran Milisavljevic - Daygames projektni zadatak", 800, 850, 12));
	SviTekstovi.push_back(new UIText(renderer, "Pokreni!!", 500, 620, 50));
	SviTekstovi.push_back(new UIText(renderer, "Broj kartica vodoravno", 300, 320, 40));
	SviTekstovi.push_back(new UIText(renderer, "Broj kartica vertikalno", 300, 450, 40));
}
void Game::initGame(unsigned int brojw, unsigned int brojh)
{
	broj = brojw * brojh;

	for (unsigned int i = 0; i < broj; i++)
	{
		SveKartice.push_back(new GameObject(i, broj, renderer));
	}
	srand(time(NULL));
	std::random_shuffle(&SveKartice[0], &SveKartice[broj - 1]);
	for (unsigned int i = 0; i < broj; i++)
	{
		SveKartice[i]->plasiraj(i % brojw, i / brojw);
	}
	defaultTexture = TextureManager::LoadTexture("res/textures/default.jpg", renderer);

}

void Game::update()
{
	if (!radiMis)
	{
		ctr++;
		if (ctr == 60)
		{
			radiMis = true;
			ctr = 0;
		}
	}
	for (unsigned int i = 0; i < broj; i++)
	{
		SveKartice[i]->Update();
	}
	if (state == 2)
	{
		spari();
	}

}

SDL_Rect gumb1 = { 400, 600, 400, 100 };
SDL_Rect gumb2 = { 800, 300, 80, 80 };
SDL_Rect gumb3 = { 800, 430, 80, 80 };

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 115, 167, 250, 255);
	SDL_RenderClear(renderer);
	switch (state)
	{
	case 0:
		SDL_SetRenderDrawColor(renderer, 66, 75, 245, 255);
		SDL_RenderFillRects(renderer, &gumb1, 1);
		if (selected == 1)
		{
			SDL_SetRenderDrawColor(renderer, 36, 45, 245, 255);
			SDL_RenderFillRects(renderer, &gumb2, 1);
			SDL_SetRenderDrawColor(renderer, 66, 75, 245, 255);
			SDL_RenderFillRects(renderer, &gumb3, 1);
		}
		else if (selected == 2)
		{
			SDL_SetRenderDrawColor(renderer, 36, 45, 245, 255);
			SDL_RenderFillRects(renderer, &gumb3, 1);
			SDL_SetRenderDrawColor(renderer, 66, 75, 245, 255);
			SDL_RenderFillRects(renderer, &gumb2, 1);
		}
		else
		{
			SDL_RenderFillRects(renderer, &gumb2, 1);
			SDL_RenderFillRects(renderer, &gumb3, 1);
		}
		for (UIText* x : SviTekstovi)
			x->Render();
		rowT->Render();
		colT->Render();
		break;
	case 1:

		break;
	case 2:
		for (unsigned int i = 0; i < broj; i++)
		{
			if (SveKartice[i]->okrenut)
				SveKartice[i]->Render();
			else
				SveKartice[i]->Render(defaultTexture);
		}
		break;
	case 3:

		break;
	default:
		break;
	}
	SDL_RenderPresent(renderer);
}
void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	SDL_Quit();
	std::cout << "Game cleaned!.." << std::endl;
}

void Game::handleEvents()
{

	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
	case SDL_MOUSEMOTION:
		SDL_GetMouseState(&x, &y);
		break;
	case SDL_MOUSEBUTTONDOWN:
		switch (state)
		{
		case 0:
			if (collision(gumb1))
			{
				initGame(row, col);
				state = 2;
			}
			if (collision(gumb2))
			{
				selected = 1;
			}
			if (collision(gumb3))
			{
				selected = 2;
			}
			break;
		case 1:

			break;
		case 2:
			if (radiMis) {
				for (unsigned int i = 0; i < broj; i++)
				{
					if (SveKartice[i]->sparen)
						continue;
					if (collision(SveKartice[i]->gdr()))
					{
						activeID[brojK % 2] = i;
						brojK++;
						SveKartice[i]->Kliknut();
						radiMis = false;
					}
				}
			}
			break;
		case 3:

			break;
		default:
			break;
		}
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_1:
			assign(1);
			break;
		case SDLK_2:
			assign(2);
			break;
		case SDLK_3:
			assign(3);
			break;
		case SDLK_4:
			assign(4);
			break;
		case SDLK_5:
			assign(5);
			break;
		case SDLK_6:
			assign(6);
			break;
		case SDLK_7:
			assign(7);
			break;
		case SDLK_8:
			assign(8);
			break;
		case SDLK_9:
			assign(9);
		}
	default:
		break;
	}
}

bool Game::running()
{
	return isRunning;
}

SDL_Renderer* Game::getRenderer()
{
	return this->renderer;
}
