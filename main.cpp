#include "SDL.h"
#include "sdlui.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDLUI app", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDLUI_Init(renderer, window);
	bool quit = false;
    SDL_Event e;
    
#include "sdlui_demo_controls_create.cpp"
    
    while (!quit)
    {
        while (SDL_PollEvent(&e))
	    {
            if (e.type == SDL_QUIT)
	        {
	            quit = true;
	        }
            
            SDLUI_EventHandler(e);
        }
        
        SDLUI_WindowHandler();
        
#include "sdlui_demo_controls_usage.cpp"
        
        SDL_SetRenderDrawColor(renderer, 30, 100, 140, 255);
        SDL_RenderClear(renderer);
        SDLUI_Render();
        SDL_RenderPresent(renderer);
    }
    
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}