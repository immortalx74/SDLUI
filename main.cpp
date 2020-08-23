#include "SDL.h"
#include "sdlui.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("SDLUI app", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	// Initializes SDLUI
	SDLUI_Init(renderer, window);

	bool quit = false;
	SDL_Event e;

	// Each SDLUI_Create_xxx function returns a pointer to a control. These are used as "handles"
	// to call a control's logic, or to modify its properties.
	#include "sdlui_demo_controls_create.cpp"

	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if(e.type == SDL_KEYDOWN)
			{
				if(e.key.keysym.sym == SDLK_RETURN)
				{
					// txt1->text.modify("Button");
				}
			}
			if(e.type == SDL_WINDOWEVENT)
			{
				if(e.window.event == SDL_WINDOWEVENT_MINIMIZED || e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
				{
					while(SDL_WaitEvent(&e))
					{
						if(e.window.event == SDL_WINDOWEVENT_RESTORED || e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
						{
							break;
						}
					}
				}
			}

			// Needs to be called inside the event loop, to capture mouse input and window events.
			SDLUI_EventHandler(e);
		}

		// Manages SDLUI windows and their child controls
		SDLUI_WindowHandler();

		// Logic handling is similar to that of immediate mode GUIs. Example: if(SDLUI_Button(btn1)) {// do something}
		#include "sdlui_demo_controls_usage.cpp"

		SDL_SetRenderDrawColor(renderer, 30, 100, 140, 255);
		SDL_RenderClear(renderer);

		// Draws the UI. Regular SDL drawing should take place before this, for the UI to appear on top.
		SDLUI_Render();
		SDL_RenderPresent(renderer);
	}

	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}