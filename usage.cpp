// Usage
bool SDLUI_Button(SDLUI_Control_Button *btn)
{
    i32 mx, my;
    SDL_GetMouseState(&mx, &my);
    
    SDL_Rect r = {btn->x,btn->y,btn->w,btn->h};
    if(SDLUI_PointCollision(r, mx, my))
    {
        if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
        {
            btn->state = SDLUI_BUTTON_STATE_CLICK;
            return true;
        }
        if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD)
        {
            btn->state = SDLUI_BUTTON_STATE_CLICK;
        }
        else
        {
            btn->state = SDLUI_BUTTON_STATE_HOVER;
        }
        
        return false;
    }
    
    btn->state = SDLUI_BUTTON_STATE_NORMAL;
    return false;
}

bool SDLUI_SliderInt(SDLUI_Control_SliderInt *si)
{
    i32 mx, my;
    SDL_GetMouseState(&mx, &my);
    
    SDL_Rect r = {si->x,si->y,si->w,si->h};
    if(SDLUI_PointCollision(r, mx, my))
    {
        if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
        {
            if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
            {
                si->value = SDLUI_Map(si->x, si->x + si->w, si->min, si->max, mx);
            }
            else
            {
                si->value = SDLUI_Map(si->y + si->h, si->y, si->min, si->max, my);
            }
            
            si->ischanging = true;
        }
    }
    
    if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD && si->ischanging)
    {
        if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
        {
            si->value = SDLUI_Map(si->x, si->x + si->w, si->min, si->max, mx);
            si->value = SDLUI_Clamp(si->value, si->min, si->max);
        }
        else
        {
            si->value = SDLUI_Map(si->y + si->h, si->y, si->min, si->max, my);
            si->value = SDLUI_Clamp(si->value, si->min, si->max);
        }
    }
    
    if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED && si->ischanging)
    {
        si->ischanging = false;
        // NOTE: Returns true only on release. Is this the right way to do it?
        return true;
    }
    
    return false;
}

bool SDLUI_CheckBox(SDLUI_Control_CheckBox *chk)
{
    
    i32 mx, my;
    SDL_GetMouseState(&mx, &my);
    
    SDL_Rect r = {chk->x,chk->y,chk->w,chk->h};
    if(SDLUI_PointCollision(r, mx, my))
    {
        if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
        {
            chk->checked = !chk->checked;
            return true;
        }
    }
    return false;
}

bool SDLUI_ToggleButton(SDLUI_Control_ToggleButton *tb)
{
    i32 mx, my;
    SDL_GetMouseState(&mx, &my);
    
    SDL_Rect r = {tb->x,tb->y,tb->w,tb->h};
    if(SDLUI_PointCollision(r, mx, my))
    {
        if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
        {
            tb->checked = !tb->checked;
            return true;
        }
    }
    
    return false;
}

bool SDLUI_Text(SDLUI_Control_Text *txt)
{
    if(txt->text.modified)
    {
        txt->w = txt->text.length * SDLUI_Font.width;
        
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,txt->text.data, c);
        txt->tex_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        SDL_FreeSurface(s);
        
        txt->text.modified = false;
    }
    
    return true;
}