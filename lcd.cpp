#include "lcd.hpp"
#include <iostream>

namespace gbemu{
GBLCD::GBLCD(GBMEM* mem) : gb_mainwin(nullptr), gb_screen(nullptr), mem(mem) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		; // couldn't init SDL 
	}
	else { 
		gb_mainwin = SDL_CreateWindow("gbemu",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				scr_width,
				scr_height,
				SDL_WINDOW_SHOWN);
		gb_screen = SDL_GetWindowSurface(gb_mainwin);
		gb_renderer = SDL_CreateRenderer(gb_mainwin, -1, SDL_RENDERER_SOFTWARE);
	}
}
GBLCD::~GBLCD(){
	SDL_DestroyWindow(gb_mainwin);
	SDL_Quit();
}
void GBLCD::render(){
	SDL_LockSurface(gb_screen);
	SDL_memset(gb_screen->pixels, 0, gb_screen->h * gb_screen->pitch);
	SDL_UpdateWindowSurface(gb_mainwin);
	SDL_UnlockSurface(gb_screen);
}
};
