#include <SDL2/SDL.h>
#include "memory.hpp"

const int scr_height = 160;
const int scr_width = 144; 

namespace gbemu {
class GBLCD{
public:
	GBLCD(GBMEM*);
	~GBLCD();
	void render();
	void draw(int, int, int, int);
private:
	GBMEM *mem;
	SDL_Window* gb_mainwin;
	SDL_Surface* gb_screen;
	SDL_Renderer* gb_renderer;
};
}

