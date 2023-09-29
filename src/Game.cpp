#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "uiText.h"

//#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

SDL_Event Game::event;
std::vector<GameObject*> AllCards;
std::vector<UIText*> AllTexts;
std::vector<UIText*> AllPlayers;
std::vector<UIText*> AllScores;


UIText* rowT;
UIText* colT;
UIText* playerCount;
UIText* congrats;
UIText* home;
UIText* err;
UIText* replay;
bool canClick = true;
bool flag = false;
bool flip = 0;
SDL_Rect table;
SDL_Texture* defaultTexture;	//backface
int maxCards = 0;					//maxCards ukupno kartica
int playerScores[9];					//lista s max 9 igraca
int nplay = 1, currentPlayer = 0;	//odabrani broj igrača, trenutni igrač na potezu
int x, y;						//koordinate miša
int wid;
int row = 4, col = 4, selected = 3;
int activeID[] = { -1, -1 };
unsigned int delay = 110;
unsigned int ctr = 0;

bool collision(SDL_Rect rec) {
	return (rec.x<x && rec.x + rec.w>x && rec.y<y && rec.y + rec.h>y);
}

void pair()
{
	if (delay == 60) { //produzi iskljucenje inputa igraca dok se kartica potpuno okrene
		ctr = 0;
		canClick = false;
	}
	if (delay == 0) {
		if (AllCards[activeID[0]]->pairID == AllCards[activeID[1]]->pairID)
		{
			AllCards[activeID[0]]->paired = true;
			AllCards[activeID[1]]->paired = true;
			playerScores[currentPlayer]++;
			AllScores[currentPlayer]->UpdateTexture(std::to_string(playerScores[currentPlayer]).c_str(), 38);
		}
		else
		{
			AllCards[activeID[0]]->ClickedOn();
			AllCards[activeID[1]]->ClickedOn();
		}
		activeID[0] = -1;
		activeID[1] = -1;
		delay = 120;
		currentPlayer++;
		if (currentPlayer == nplay)
			currentPlayer = 0;
	}
	else
		delay--;
}

void Game::assign(int i)
{
	//max igraca, redova i stupaca je 9
	if (i < 1 || i > 9) { 
		return;
	}
	switch (selected) {
	case 0:
		row = i;
		rowT->UpdateTexture(std::to_string(i).c_str(), 40);
		break;
	case 1:
		col = i;
		colT->UpdateTexture(std::to_string(i).c_str(), 40);
		break;
	case 2:
		nplay = i;
		playerCount->UpdateTexture(std::to_string(i).c_str(), 40);
		break;
	}
}

void Game::deleteRound() {
	std::vector<GameObject*>().swap(AllCards);
	std::vector<UIText*>().swap(AllPlayers);
	std::vector<UIText*>().swap(AllScores);
}

Game::Game() {

}

Game::~Game() {

}

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
	initMenu();
}

void Game::initMenu()
{
	err = new UIText(renderer, "Umnozak brojeva mora biti paran!", 500, 520, 20);
	replay = new UIText(renderer, "Pokreni s istim podacima", 630, 810, 30);
	rowT = new UIText(renderer, "4", 1030, 320, 40);
	colT = new UIText(renderer, "4", 1030, 450, 40);
	playerCount = new UIText(renderer, "1", 1030, 580, 40);
	congrats = new UIText(renderer, "Cestitamo, zavrsili ste igru!", 600, 200, 40);
	AllTexts.push_back(new UIText(renderer, "Memory igra - glavni meni", 400, 100, 72));
	AllTexts.push_back(new UIText(renderer, "Napravio Fran Milisavljevic - Daygames projektni zadatak", 1050, 850, 12));
	AllTexts.push_back(new UIText(renderer, "Pokreni!!", 700, 800, 50));
	AllTexts.push_back(new UIText(renderer, "Broj kartica vodoravno", 500, 320, 40));
	AllTexts.push_back(new UIText(renderer, "Broj kartica vertikalno", 500, 450, 40));
	AllTexts.push_back(new UIText(renderer, "Broj igraca", 500, 580, 40));
	AllTexts.push_back(new UIText(renderer, "Izlaz", 25, 35, 20));
}

void Game::initRound(unsigned int cardsW, unsigned int cardsH, unsigned int nplay)
{

	maxCards = cardsW * cardsH;
	currentPlayer = 0;
	for (int i = 0; i < 9; i++)
		playerScores[i] = 0;
	float ratio;
	if (2 * cardsH < cardsW)
		ratio = float(cardsW)/2.0f;
	else
		ratio = cardsH;
	for (unsigned int i = 0; i < maxCards; i++)
	{
		AllCards.push_back(new GameObject(i, maxCards, renderer));
	}
	srand(time(NULL));
	std::random_shuffle(&AllCards[0], &AllCards[maxCards - 1]);
	int maxw = (60 / ratio + 914 / 1.5 / ratio) * cardsW - 60 / ratio;
	int bid = wid / 2 - maxw / 2;
	for (unsigned int i = 0; i < maxCards; i++)
	{
		AllCards[i]->place(i % cardsW, i / cardsW, ratio, bid);
	}
	defaultTexture = TextureManager::LoadTexture("res/textures/default.jpg", renderer);
	home = new UIText(renderer, "Menu", 25, 35, 20);
	if (nplay <= 4) {
		AllPlayers.push_back(new UIText(renderer, "PLAYER 1:", 740, 800, 38));
		AllScores.push_back(new UIText(renderer, "0", 930, 800, 38));
		AllPlayers.push_back(new UIText(renderer, "PLAYER 2:", 1350, 400, 38));
		AllScores.push_back(new UIText(renderer, "0", 1540, 400, 38));
		AllPlayers.push_back(new UIText(renderer, "PLAYER 3:", 700, 50, 38));
		AllScores.push_back(new UIText(renderer, "0", 890, 50, 38));
		AllPlayers.push_back(new UIText(renderer, "PLAYER 4:", 20, 400, 38));
		AllScores.push_back(new UIText(renderer, "0", 210, 400, 38));

	}
	else {
		for (size_t i = 0; i < nplay; i++)
		{
			std::string ime = "PLAYER ";
			ime = ime + std::to_string(i+1)+":";
			AllPlayers.push_back(new UIText(renderer, ime.c_str(), 1350, 200 + i * 40, 38));
			AllScores.push_back(new UIText(renderer, "0", 1550, 200 + i * 40, 38));
		}
	}
	table.w = maxw+20;
	table.h = AllCards[maxCards - 1]->gdr().h + AllCards[maxCards - 1]->gdr().y+10-60/ratio;
	table.x = bid-10;
	table.y = 20;
}


SDL_Rect SelectorButtons[] = {  { 1000, 300, 80, 80 } ,
								{ 1000, 430, 80, 80 } ,
								{ 1000, 560, 80, 80 } };
SDL_Rect start = { 600, 780, 400, 100 };
SDL_Rect btnMenu = { 10, 10, 75, 75 };
SDL_Rect pointer = { 10, 10, 210, 10 };
int winCtr = 120;
void Game::update()
{
	if (!canClick)
	{
		ctr++;
		if (ctr == 60)
		{
			canClick = true;
			ctr = 0;
		}
	}
	if (state == round)
	{
		for (unsigned int i = 0; i < maxCards; i++)
		{
			AllCards[i]->Update();
		}
		if (activeID[0] != -1 && activeID[1] != -1) {
			pair();
		}
		int nPaired = 0;
		for (int i = 0; i < maxCards; i++)
		{
			if (AllCards[i]->paired)
				nPaired++;
		}
		if (nPaired == maxCards)
		{
			canClick = false;
			winCtr--;
		}
		if (winCtr == 1) {
			winCtr = 120;
			canClick = true;
			state = leaderboard;
		}
	}

}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 115, 167, 250, 255);
	SDL_RenderClear(renderer);
	switch (state)
	{
	case menu:
		SDL_SetRenderDrawColor(renderer, 66, 75, 245, 255);
		SDL_RenderFillRects(renderer, &start, 1);
		SDL_RenderFillRects(renderer, &btnMenu, 1);
		for (int i = 0; i < 3; i++)
		{
			if (selected == i) {
				SDL_SetRenderDrawColor(renderer, 36, 45, 245, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 66, 75, 245, 255);
			}
			SDL_RenderFillRects(renderer, &SelectorButtons[i], 1);
		}
		for (UIText* x : AllTexts)
			x->Render();
		rowT->Render();
		colT->Render();
		playerCount->Render();
		if(flag)
			err->Render();
		break;
	case round:
		SDL_SetRenderDrawColor(renderer, 140, 245, 180, 255);
		SDL_RenderFillRect(renderer, &table);
		SDL_SetRenderDrawColor(renderer, 36, 45, 245, 255);
		SDL_RenderFillRect(renderer, &btnMenu);
		home->Render();
		for (unsigned int i = 0; i < maxCards; i++)
		{
			if (AllCards[i]->flipped)
				AllCards[i]->Render();
			else
				AllCards[i]->Render(defaultTexture);
		}
		for (size_t i = 0; i < nplay; i++) {
			AllPlayers[i]->Render();
			AllScores[i]->Render();
		}
		pointer.x = AllPlayers[currentPlayer]->destRect.x;
		pointer.y = AllPlayers[currentPlayer]->destRect.y + 42;
		SDL_RenderFillRect(renderer, &pointer);
		break;
	case leaderboard:
		SDL_SetRenderDrawColor(renderer, 36, 45, 245, 255);
		SDL_RenderFillRect(renderer, &btnMenu);
		SDL_RenderFillRect(renderer, &start);
		home->Render();
		congrats->Render();
		replay->Render();
		for (size_t i = 0; i < nplay; i++) {
			AllPlayers[i]->destRect.x = 700;
			AllPlayers[i]->destRect.y = 260+i*44;
			AllPlayers[i]->Render();
			AllScores[i]->destRect.x = 900;
			AllScores[i]->destRect.y = 260 + i * 44;
			AllScores[i]->Render();
		}
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
		case menu:
			if (collision(start))
			{
				std::cout << row*col << std::endl;
				if (row * col % 2 == 1)
					flag = true;
				else {
					flag = false;
					initRound(row, col, nplay);
					state = round;
				}
				break;
			}
			for (int i = 0; i < 3; i++)
			{
				if (collision(SelectorButtons[i]))
					selected = i;
			}
			if (collision(btnMenu))
				isRunning = false;
			break;
		case round:
			if (canClick) {
				for (unsigned int i = 0; i < maxCards; i++)
				{
					if (AllCards[i]->paired || AllCards[i]->flipped)
						continue;
					if (collision(AllCards[i]->gdr()))
					{
						activeID[flip] = i;
						flip=!flip;
						AllCards[i]->ClickedOn();
						canClick = false;
					}
				}
				if (collision(btnMenu)) {
					deleteRound();
					state = menu;
				}
			}
			break;
		case leaderboard:
			if (collision(start))
			{
				deleteRound();
				initRound(row, col, nplay);
				state = round;
			}
			if (collision(btnMenu)) {
				deleteRound();
				state = menu;
			}
			break;
		default:
			break;
		}
	case SDL_KEYDOWN:
		if(state==menu)
			assign(static_cast<char>(event.key.keysym.sym)-'0');
		break;
	default:
		break;
	}
}

bool Game::running()
{
	return isRunning;
}

