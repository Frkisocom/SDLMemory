#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "uiText.h"

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

SDL_Event Game::event;
std::vector<GameObject*> SveKartice;
std::vector<UIText*> SviTekstovi;
std::vector<UIText*> SviPlayeri;
std::vector<UIText*> SviScoreovi;


UIText* rowT;
UIText* colT;
UIText* players;
UIText* congrats;
UIText* home;
UIText* err;
UIText* replay;
bool fleg = false;
SDL_Rect stol;
int broj = 0;					//broj ukupno kartica
int igraci[9];					//lista s max 9 igraca
int nplay = 1, currentPlayer = 0;
SDL_Texture* defaultTexture;	//backface
int x, y;						//koordinate miša
bool radiMis = true;
unsigned int delay = 110;
unsigned int ctr = 0;
int activeID[] = { -1, -1 };
int brojK = 0;

int row = 4, col = 4, selected = 0;
//Non member funkcije
bool collision(SDL_Rect rec) {
	return (rec.x<x && rec.x + rec.w>x && rec.y<y && rec.y + rec.h>y);
}

int spari()
{
	if (delay == 60) { //produzi iskljucenje inputa igraca dok se kartica potpuno okrene
		ctr = 0;
		radiMis = false;
	}
	if (delay == 0) {
		if (SveKartice[activeID[0]]->pairID == SveKartice[activeID[1]]->pairID)
		{
			SveKartice[activeID[0]]->sparen = true;
			SveKartice[activeID[1]]->sparen = true;
			activeID[0] = -1;
			activeID[1] = -1;
			delay = 120;
			//updateat score
			igraci[currentPlayer]++;
			SviScoreovi[currentPlayer]->UpdateTexture(std::to_string(igraci[currentPlayer]).c_str(), 38);
			currentPlayer++;
			if (currentPlayer == nplay)
				currentPlayer = 0;
		}
		else
		{
			SveKartice[activeID[0]]->Kliknut();
			SveKartice[activeID[1]]->Kliknut();
			activeID[0] = -1;
			activeID[1] = -1;
			delay = 120;
			currentPlayer++;
			if (currentPlayer == nplay)
				currentPlayer = 0;
			return 2;
		}

	}
	else
		delay--;
	return 0;
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
	else if (selected == 3)
	{
		nplay = i;
		players->UpdateTexture(std::to_string(i).c_str(), 40);
	}
}

Game::Game() {

}

Game::~Game() {

}

int wid;
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	wid = width;
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
	initState1();
}

void Game::initState1()
{
	err = new UIText(renderer, "Umnozak brojeva mora biti paran!", 500, 520, 20);
	replay = new UIText(renderer, "Pokreni s istim podacima", 630, 810, 30);
	rowT = new UIText(renderer, "4", 1030, 320, 40);
	colT = new UIText(renderer, "4", 1030, 450, 40);
	players = new UIText(renderer, "1", 1030, 580, 40);
	congrats = new UIText(renderer, "Cestitamo, zavrsili ste igru!", 600, 200, 40);
	SviTekstovi.push_back(new UIText(renderer, "Memory igra - glavni meni", 400, 100, 72));
	SviTekstovi.push_back(new UIText(renderer, "Napravio Fran Milisavljevic - Daygames projektni zadatak", 1050, 850, 12));
	SviTekstovi.push_back(new UIText(renderer, "Pokreni!!", 700, 800, 50));
	SviTekstovi.push_back(new UIText(renderer, "Broj kartica vodoravno", 500, 320, 40));
	SviTekstovi.push_back(new UIText(renderer, "Broj kartica vertikalno", 500, 450, 40));
	SviTekstovi.push_back(new UIText(renderer, "Broj igraca", 500, 580, 40));
}

void Game::initState2(unsigned int brojw, unsigned int brojh, unsigned int nplay)
{

	broj = brojw * brojh;
	float ratio;
	if (2 * brojh < brojw)
		ratio = float(brojw)/2.0f;
	else
		ratio = brojh;
	for (unsigned int i = 0; i < broj; i++)
	{
		SveKartice.push_back(new GameObject(i, broj, renderer));
	}
	srand(time(NULL));
	std::random_shuffle(&SveKartice[0], &SveKartice[broj - 1]);
	int maxw = (60 / ratio + 914 / 1.5 / ratio) * brojw - 60 / ratio;
	int bid = wid / 2 - maxw / 2;
	for (unsigned int i = 0; i < broj; i++)
	{
		SveKartice[i]->plasiraj(i % brojw, i / brojw, ratio, bid);
	}
	defaultTexture = TextureManager::LoadTexture("res/textures/default.jpg", renderer);
	home = new UIText(renderer, "Menu", 25, 40, 20);
	if (nplay <= 4) {
		SviPlayeri.push_back(new UIText(renderer, "PLAYER 1:", 740, 800, 38));
		SviScoreovi.push_back(new UIText(renderer, "0", 930, 800, 38));
		SviPlayeri.push_back(new UIText(renderer, "PLAYER 2:", 1350, 400, 38));
		SviScoreovi.push_back(new UIText(renderer, "0", 1540, 400, 38));
		SviPlayeri.push_back(new UIText(renderer, "PLAYER 3:", 700, 50, 38));
		SviScoreovi.push_back(new UIText(renderer, "0", 890, 50, 38));
		SviPlayeri.push_back(new UIText(renderer, "PLAYER 4:", 20, 400, 38));
		SviScoreovi.push_back(new UIText(renderer, "0", 210, 400, 38));

	}
	else {
		for (size_t i = 0; i < nplay; i++)
		{
			std::string ime = "PLAYER ";
			ime = ime + std::to_string(i+1)+":";
			SviPlayeri.push_back(new UIText(renderer, ime.c_str(), 1350, 200 + i * 40, 38));
			SviScoreovi.push_back(new UIText(renderer, "0", 1550, 200 + i * 40, 38));
		}
	}
	stol.w = maxw+20;
	stol.h = SveKartice[broj - 1]->gdr().h + SveKartice[broj - 1]->gdr().y+10-60/ratio;
	stol.x = bid-10;
	stol.y = 20;
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
	if (state == 2)
	{
		for (unsigned int i = 0; i < broj; i++)
		{
			SveKartice[i]->Update();
		}
		if (activeID[0] != -1 && activeID[1] != -1) {
			spari();
		}
		int yoink = 0;
		for (int i = 0; i < broj; i++)
		{
			if (SveKartice[i]->sparen)
				yoink++;
		}
		if (yoink == broj)
			state = 3;
	}

}

SDL_Rect gumb1 = { 600, 780, 400, 100 };
SDL_Rect gumb2 = { 1000, 300, 80, 80 };
SDL_Rect gumb3 = { 1000, 430, 80, 80 };
SDL_Rect gumb4 = { 1000, 560, 80, 80 };
SDL_Rect gumbHome = { 10, 10, 75, 75 };
SDL_Rect pointer = { 10, 10, 200, 10 };

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
			SDL_RenderFillRects(renderer, &gumb4, 1);
		}
		else if (selected == 2)
		{
			SDL_SetRenderDrawColor(renderer, 36, 45, 245, 255);
			SDL_RenderFillRects(renderer, &gumb3, 1);
			SDL_SetRenderDrawColor(renderer, 66, 75, 245, 255);
			SDL_RenderFillRects(renderer, &gumb2, 1);
			SDL_RenderFillRects(renderer, &gumb4, 1);
		}
		else if (selected == 3)
		{
			SDL_SetRenderDrawColor(renderer, 36, 45, 245, 255);
			SDL_RenderFillRects(renderer, &gumb4, 1);
			SDL_SetRenderDrawColor(renderer, 66, 75, 245, 255);
			SDL_RenderFillRects(renderer, &gumb2, 1);
			SDL_RenderFillRects(renderer, &gumb3, 1);
		}
		else
		{
			SDL_RenderFillRects(renderer, &gumb2, 1);
			SDL_RenderFillRects(renderer, &gumb3, 1);
			SDL_RenderFillRects(renderer, &gumb4, 1);
		}
		for (UIText* x : SviTekstovi)
			x->Render();
		rowT->Render();
		colT->Render();
		players->Render();
		if(fleg)
			err->Render();
		break;
	case 2:
		SDL_SetRenderDrawColor(renderer, 140, 245, 180, 255);
		SDL_RenderFillRect(renderer, &stol);
		SDL_SetRenderDrawColor(renderer, 36, 45, 245, 255);
		SDL_RenderFillRect(renderer, &gumbHome);
		for (unsigned int i = 0; i < broj; i++)
		{
			if (SveKartice[i]->okrenut)
				SveKartice[i]->Render();
			else
				SveKartice[i]->Render(defaultTexture);
		}
		home->Render();
		for (size_t i = 0; i < nplay; i++) {
			SviPlayeri[i]->Render();
			SviScoreovi[i]->Render();
		}
		pointer.x = SviPlayeri[currentPlayer]->destRect.x;
		pointer.y = SviPlayeri[currentPlayer]->destRect.y + 42;
		SDL_RenderFillRect(renderer, &pointer);
		SDL_SetRenderDrawColor(renderer, 66, 75, 245, 255);
		break;
	case 3:
		SDL_SetRenderDrawColor(renderer, 36, 45, 245, 255);
		SDL_RenderFillRect(renderer, &gumbHome);
		SDL_RenderFillRect(renderer, &gumb1);
		SDL_SetRenderDrawColor(renderer, 66, 75, 245, 255);
		home->Render();
		congrats->Render();
		replay->Render();
		for (size_t i = 0; i < nplay; i++) {
			SviPlayeri[i]->destRect.x = 700;
			SviPlayeri[i]->destRect.y = 260+i*44;
			SviPlayeri[i]->Render();
			SviScoreovi[i]->destRect.x = 900;
			SviScoreovi[i]->destRect.y = 260 + i * 44;
			SviScoreovi[i]->Render();
		}

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
				std::cout << row*col << std::endl;
				if (row * col % 2 == 1)
					fleg = true;
				else {
					fleg = false;
					initState2(row, col, nplay);
					state = 2;
				}
				break;
			}
			if (collision(gumb2))
			{
				selected = 1;
				break;
			}
			if (collision(gumb3))
			{
				selected = 2;
				break;
			}
			if (collision(gumb4))
			{
				selected = 3;
				break;
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
				if (collision(gumbHome)) {
					//ne znam zasto ne radi, obrise vektor i sve u njemu, no ne oslobodi direkt memoriju iz procesora, pa kada ponovo pozovem mi zauzima novu memoriju. Nezz popravit
					for (auto& obj : SveKartice) {
						delete obj;
					}
					SveKartice.clear();
					SveKartice.shrink_to_fit();
					std::vector<GameObject*>().swap(SveKartice);
					state = 0;
				}
			}
			break;
		case 3:
			if (collision(gumb1))
			{
				for (auto& obj : SveKartice) {
					delete obj;
				}
				SveKartice.clear();
				SveKartice.shrink_to_fit();
				std::vector<GameObject*>().swap(SveKartice);
				initState2(row, col, nplay);
				state = 2;
			}
			if (collision(gumbHome)) {
				state = 0;
			}
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
