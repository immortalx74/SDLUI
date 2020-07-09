#include "SDL.h"
#include "ui.h"
#include <iostream>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDLUI app", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDLUI_Init(renderer, window);
	bool quit = false;
	
    SDLUI_Control_Button *btn = SDLUI_CreateButton("PushButton", 20, 200);
    SDLUI_Control_Button *btn2 = SDLUI_CreateButton("Hello!", 280, 10);
    SDLUI_Control_SliderInt *sl1 = SDLUI_CreateSliderInt(20, 20, 0, 100, 50, SDLUI_ORIENTATION_VERTICAL);
    SDLUI_Control_SliderInt *sl2 = SDLUI_CreateSliderInt(80, 20, 0, 100, 50);
    SDLUI_Control_CheckBox *chk1 = SDLUI_CreateCheckBox(200, 100, true);
    SDLUI_Control_ToggleButton *tb = SDLUI_CreateToggleButton(200, 150, true);
    SDLUI_Control_TabContainer *tbc = SDLUI_CreateTabContainer(200, 200);
    SDLUI_CreateTab(tbc, "First");
    SDLUI_CreateTab(tbc, "Second");
    SDLUI_CreateTab(tbc, "Third");
    
    //SDLUI_Control *arr[4];
    //arr[0] = (SDLUI_Control*)sl1;
    //arr[1] = (SDLUI_Control*)btn;
    
    SDL_Event e;
    
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
        
        if(SDLUI_Button(btn2))
        {
            
        }
        
        if(SDLUI_SliderInt(sl1))
        {
            
        }
        
        if(SDLUI_SliderInt(sl2))
        {
            
        }
        
        if(SDLUI_CheckBox(chk1))
        {
            
        }
        
        if(SDLUI_ToggleButton(tb))
        {
            
        }
        
        if(SDLUI_TabContainer(tbc))
        {
            
        }
        
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderClear(renderer);
        SDLUI_Render();
        SDL_RenderPresent(renderer);
    }
    
    free(SDLUI_Base.pool);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}