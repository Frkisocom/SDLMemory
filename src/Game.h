#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>

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

	SDL_Renderer* getRenderer();
	bool running();
	static SDL_Event event;

private:
	int state = 0;
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
	void initState1();
	void initState2(unsigned int w, unsigned int h, unsigned int nplay);

};