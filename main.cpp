#include "SDL.h"
#include "ui.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDLUI app", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDLUI_Init(renderer, window);
	bool quit = false;
    SDL_Event e;
    
    SDLUI_Control_Window *wnd = SDLUI_CreateWindow(10, 10, 400, 400, "Demo Window (Enter to toggle visibility)");
    SDLUI_Control_Button *btn = SDLUI_CreateButton(30, 60, "PushButton");
    SDLUI_Control_SliderInt *si = SDLUI_CreateSliderInt(30, 110, 0, 100, 50);
    SDLUI_Control_CheckBox *chk = SDLUI_CreateCheckBox(30, 140, true);
    SDLUI_Control_ToggleButton *tb = SDLUI_CreateToggleButton(30, 180, false);
    SDLUI_Control_RadioButton *rb1 = SDLUI_CreateRadioButton(30, 220, true);
    SDLUI_Control_RadioButton *rb2 = SDLUI_CreateRadioButton(30, 250, false);
    SDLUI_Control_RadioButton *rb3 = SDLUI_CreateRadioButton(30, 280, false);
    
    SDLUI_Control_RadioButton *rb4 = SDLUI_CreateRadioButton(90, 220, false, 1);
    SDLUI_Control_RadioButton *rb5 = SDLUI_CreateRadioButton(90, 250, true, 1);
    SDLUI_Control_RadioButton *rb6 = SDLUI_CreateRadioButton(90, 280, false, 1);
    
    btn->parent_to(wnd);
    si->parent_to(wnd);
    chk->parent_to(wnd);
    tb->parent_to(wnd);
    rb1->parent_to(wnd);
    rb2->parent_to(wnd);
    rb3->parent_to(wnd);
    
    rb4->parent_to(wnd);
    rb5->parent_to(wnd);
    rb6->parent_to(wnd);
    
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
                    wnd->visible = !wnd->visible;
                }
            }
        }
        
        if(SDLUI_Window(wnd))
        {
            
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
        
        SDLUI_RadioButton(rb1);
        SDLUI_RadioButton(rb2);
        SDLUI_RadioButton(rb3);
        
        SDLUI_RadioButton(rb4);
        SDLUI_RadioButton(rb5);
        SDLUI_RadioButton(rb6);
        
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