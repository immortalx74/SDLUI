// Usage
bool SDLUI_Window(SDLUI_Control_Window *wnd)
{
    if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED || SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_NONE)
    {
        wnd->is_dragged = false;
    }
    
    if(wnd == SDLUI_Base.active_window)
    {
        i32 mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Rect r = {wnd->x + wnd->w - 30, wnd->y, 30, 30};
        
        if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED && SDLUI_PointCollision(r, mx, my))
        {
            wnd->visible = false;
        }
        
        r = {wnd->x,wnd->y,wnd->w - 30, 30};
        
        if(SDLUI_PointCollision(r, mx, my))
        {
            if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED && !wnd->is_dragged)
            {
                wnd->is_dragged = true;
                wnd->drag_x = mx - wnd->x;
                wnd->drag_y = my - wnd->y;
            }
        }
        
        if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD && wnd->is_dragged)
        {
            SDL_SetCursor(SDLUI_Base.cursor_arrow);
            i32 old_x = wnd->x;
            i32 old_y = wnd->y;
            wnd->x = mx - wnd->drag_x;
            wnd->y = my - wnd->drag_y;
            
            for (int i = 0; i < wnd->children.size; ++i)
            {
                wnd->children.data[i]->x += wnd->x - old_x;
                wnd->children.data[i]->y += wnd->y - old_y;
            }
        }
        
        return false;
    }
    
    return false;
}

bool SDLUI_Button(SDLUI_Control_Button *btn)
{
    if(btn->parent == SDLUI_Base.active_window)
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
    return false;
}

bool SDLUI_SliderInt(SDLUI_Control_SliderInt *si)
{
    if(si->parent == SDLUI_Base.active_window)
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
    return false;
}

bool SDLUI_CheckBox(SDLUI_Control_CheckBox *chk)
{
    if(chk->parent == SDLUI_Base.active_window)
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
    return false;
}

bool SDLUI_ToggleButton(SDLUI_Control_ToggleButton *tb)
{
    if(tb->parent == SDLUI_Base.active_window)
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
    return false;
}

bool SDLUI_RadioButton(SDLUI_Control_RadioButton *rb)
{
    if(rb->parent == SDLUI_Base.active_window)
    {
        i32 mx, my;
        SDL_GetMouseState(&mx, &my);
        
        SDL_Rect r = {rb->x,rb->y,rb->w,rb->h};
        if(SDLUI_PointCollision(r, mx, my))
        {
            if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
            {
                SDLUI_Control_RadioButton *cur;
                
                for (int i = 0; i < rb->group->size; ++i)
                {
                    cur = (SDLUI_Control_RadioButton*)rb->group->data[i];
                    cur->checked = false;
                }
                
                rb->checked = true;
                
                return true;
            }
        }
        
        return false;
    }
    return false;
}

bool SDLUI_Text(SDLUI_Control_Text *txt)
{
    if(txt->parent == SDLUI_Base.active_window)
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
    return false;
}