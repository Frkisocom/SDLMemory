#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <vector>

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void update();
	void render();
	void clean();
	void handleEvents();
	void assign(int i);

	bool running();
	static SDL_Event event;

private:
	bool isRunning;
	enum possibleStates
	{
		menu, round, leaderboard
	};
	possibleStates state = menu;
	SDL_Window* window;
	SDL_Renderer* renderer;
	void initMenu();
	void initRound(unsigned int w, unsigned int h, unsigned int nplay);
	void deleteRound();
};