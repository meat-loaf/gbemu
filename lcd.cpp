#include "lcd.hpp"

namespace gbemu{
GBLCD::GBLCD() : gb_mainwin(nullptr), gb_screen(nullptr) {
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
		
	}
}
GBLCD::~GBLCD(){
	SDL_DestroyWindow(gb_mainwin);
	SDL_Quit();
}
};
