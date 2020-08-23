void SDLUI_EventHandler(SDL_Event const &ev)
{
    if(ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP)
    {
        SDLUI_MouseStateSet(ev.type, ev.button.button);
    }

    if(ev.type == SDL_MOUSEWHEEL)
    {
        SDLUI_Base.mouse_wheel_y = ev.wheel.y;
        SDLUI_Base.mouse_wheel_x = ev.wheel.x;
    }
    else
    {
       SDLUI_Base.mouse_wheel_y = 0;
       SDLUI_Base.mouse_wheel_x = 0;
   }

   if(ev.type == SDL_WINDOWEVENT)
   {
    if(ev.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        SDLUI_ForceReDraw();
    }
}
}