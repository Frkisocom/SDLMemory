#include "SDL.h"
#include "Game.h"
#include "GameObject.h"

#include <sstream>
#include <fstream>

Game* game = nullptr;



int main(int args, char* argv[]) 
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	std::string item_name;
	std::ifstream nameFileout;
	int ww, wh;
	nameFileout.open("config.txt");
	nameFileout >> item_name;
	ww = stoi(item_name);
	nameFileout >> item_name;
	wh = stoi(item_name);
	nameFileout.close();

	game = new Game();
	game->init("Memori", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, false);
	//***********************************************************************************
	//kreiranje kartica
	//************************************************************************************
	while (game->running()) 
	{

		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);

	}
	game->clean();
	return 0;
}