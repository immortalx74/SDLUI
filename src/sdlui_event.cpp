void SDLUI_EventHandler(SDL_Event const &ev)
{
	if(ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP)
	{
		SDLUI_MouseStateSet(ev.type, ev.button.button);
	}

	if(ev.type == SDL_MOUSEWHEEL)
	{
		SDLUI_Core.mouse_wheel_y = ev.wheel.y;
		SDLUI_Core.mouse_wheel_x = ev.wheel.x;
		if(ev.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
		{
    			SDLUI_Core.mouse_wheel_x *= -1;
		}
	}
	else
	{
		SDLUI_Core.mouse_wheel_y = 0;
		SDLUI_Core.mouse_wheel_x = 0;
	}

	if(ev.type == SDL_WINDOWEVENT)
	{
		if(ev.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			SDLUI_ForceReDraw();
		}
	}

	if(ev.type > 0)
	{
		SDLUI_Core.e = ev;
	}
}
