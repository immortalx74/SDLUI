#include "SDL.h"
#include "ui.h"
#include <iostream>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("ui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ui_Init(renderer, window);
	bool quit = false;
	
    BUTTON *btn = ui_CreateButton("PushButton", 20, 100);
    SLIDER_INT *si = ui_CreateSliderInt(30, 30, 0, 100, 50);
    
    SDL_Event e;
    
    while (!quit)
    {
        while (SDL_PollEvent(&e))
	    {
            if (e.type == SDL_QUIT)
	        {
	            quit = true;
	        }
            
            ui_MouseStateSet(e.type, e.button.button);
        }
        
        
        if(ui_Button(btn))
        {
            std::cout << "here" << std::endl;
        }
        
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderClear(renderer);
        ui_Render();
        SDL_RenderPresent(renderer);
    }
    
    free(ui_base.pool);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}