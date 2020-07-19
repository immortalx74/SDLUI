#include "SDL.h"
#include "ui.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("SDLUI app", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        
    }
    SDLUI_Init(renderer, window);
	bool quit = false;
	
    SDLUI_Control_Button *btn = SDLUI_CreateButton("PushButton", 20, 20);
    SDLUI_Control_Button *btn2 = SDLUI_CreateButton("Hello!", 20, 90);
    SDLUI_Control_SliderInt *sl1 = SDLUI_CreateSliderInt(170, 20, 0, 100, 50, SDLUI_ORIENTATION_VERTICAL);
    SDLUI_Control_SliderInt *sl2 = SDLUI_CreateSliderInt(20, 160, 0, 100, 50);
    SDLUI_Control_Label *lbl = SDLUI_CreateLabel(20, 200, "Some text");
    SDLUI_Control_CheckBox *chk1 = SDLUI_CreateCheckBox(20, 240, true);
    SDLUI_Control_ToggleButton *tb = SDLUI_CreateToggleButton(80, 240, true);
    SDLUI_Control_Text *txt = SDLUI_CreateText(130, 290, "Changing text");
    SDLUI_Control_Text *txtonoff = SDLUI_CreateText(130, 244, "Off");
    SDLUI_Control_RadioButton *rdb = SDLUI_CreateRadioButton(350, 450, 1);
    
    SDLUI_Control_TabContainer *tbc = SDLUI_CreateTabContainer(350, 100, 350, 350);
    SDLUI_CreateTab(tbc, "first");
    SDLUI_CreateTab(tbc, "second");
    SDLUI_CreateTab(tbc, "third");
    
    
    //SDLUI_Control **ch = (SDLUI_Control**)malloc(2 * sizeof(SDLUI_Control*));
    //ch[0] = (SDLUI_Control*)sl1;
    //ch[1] = (SDLUI_Control*)sl2;
    
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
            txt->string.modify("new one");
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
            if(tb->checked)
            {
                txtonoff->string.modify("On");
            }
            else
            {
                txtonoff->string.modify("Off");
            }
        }
        
        if(SDLUI_RadioButton(rdb))
        {
            
        }
        
        if(SDLUI_TabContainer(tbc))
        {
            
        }
        
        SDLUI_Label(lbl);
        SDLUI_Text(txt);
        SDLUI_Text(txtonoff);
        
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