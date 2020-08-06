#include "SDL.h"
#include "ui.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDLUI app", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDLUI_Init(renderer, window);
	bool quit = false;
    SDL_Event e;
    
    SDLUI_Control_Window *wnd1 = SDLUI_CreateWindow(10, 10, 350, 400, "test window1");
    
    SDLUI_Control_Window *wnd2 = SDLUI_CreateWindow(380, 10, 450, 350, "test window2");
    
    SDLUI_Control_Window *wnd3 = SDLUI_CreateWindow(250, 400, 450, 350, "test window3");
    
    SDLUI_Control_Button *btn1 = SDLUI_CreateButton(wnd1, 10, 40, "PushButton");
    SDLUI_Control_Button *btn2 = SDLUI_CreateButton(wnd2, 10, 40, "AButton");
    
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
            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_RETURN)
                {
                    wnd1->visible = !wnd1->visible;
                }
            }
        }
        
        SDLUI_WindowHandler();
        SDLUI_Window(wnd1);
        SDLUI_Window(wnd2);
        SDLUI_Window(wnd3);
        if(SDLUI_Button(btn1))
        {
            
        }
        if(SDLUI_Button(btn2))
        {
            std::cout << "yutu" << std::endl;
        }
        
        
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