#include "SDL.h"
#include "ui.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDLUI app", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDLUI_Init(renderer, window);
	bool quit = false;
    SDL_Event e;
    
    SDLUI_Control_Button *btn = SDLUI_CreateButton(20, 10, "PushButton");
    SDLUI_Control_SliderInt *si = SDLUI_CreateSliderInt(20, 60, 0, 100, 50);
    SDLUI_Control_CheckBox *chk = SDLUI_CreateCheckBox(20, 90, true);
    SDLUI_Control_ToggleButton *tb = SDLUI_CreateToggleButton(20, 120, true);
    
    while (!quit)
    {
        while (SDL_PollEvent(&e))
	    {
            if (e.type == SDL_QUIT)
	        {
	            quit = true;
	        }
            
            if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
            {
                SDLUI_MouseStateSet(e.type, e.button.button);
            }
        }
        
        if(SDLUI_Button(btn))
        {
            
        }
        if(SDLUI_SliderInt(si))
        {
            
        }
        if(SDLUI_CheckBox(chk))
        {
            
        }
        if(SDLUI_ToggleButton(tb))
        {
            
        }
        
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
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