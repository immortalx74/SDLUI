void SDLUI_EventHandler(SDL_Event &ev)
{
    if(ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP)
    {
        SDLUI_MouseStateSet(ev.type, ev.button.button);
    }
    if(ev.type == SDL_WINDOWEVENT)
    {
        if(ev.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            SDLUI_ForceReDraw();
        }
    }
}