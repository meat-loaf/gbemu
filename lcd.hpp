#include <SDL2/SDL.h>
const int scr_height = 160;
const int scr_width = 144; 

namespace gbemu {
class GBLCD{
public:
	GBLCD();
	~GBLCD();
private:
	SDL_Window* gb_mainwin;
	SDL_Surface* gb_screen;
};
}

