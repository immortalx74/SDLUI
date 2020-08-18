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
        
        SDL_Rect r = {wnd->x, wnd->y, wnd->w, wnd->h};
        
        if(SDLUI_PointCollision(r, mx, my))
        {
            wnd->is_hovered = true;
        }
        else
        {
            wnd->is_hovered = false;
        }
        
        r = {wnd->x + wnd->w - 30, wnd->y, 30, 30};
        
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
    btn->do_process = true;
    
    if(btn->visible && btn->parent == SDLUI_Base.active_window && SDLUI_Base.active_window->is_hovered)
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
    
    btn->state = SDLUI_BUTTON_STATE_NORMAL;
    return false;
}

bool SDLUI_SliderInt(SDLUI_Control_SliderInt *si)
{
    si->do_process = true;

    if(si->visible && si->parent == SDLUI_Base.active_window && SDLUI_Base.active_window->is_hovered)
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
    
    si->ischanging = false;
    return false;
}

bool SDLUI_CheckBox(SDLUI_Control_CheckBox *chk)
{
    chk->do_process = true;

    if(chk->visible && chk->parent == SDLUI_Base.active_window && SDLUI_Base.active_window->is_hovered)
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
    tb->do_process = true;

    if(tb->visible && tb->parent == SDLUI_Base.active_window && SDLUI_Base.active_window->is_hovered)
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
    rb->do_process = true;

    if(rb->visible && rb->parent == SDLUI_Base.active_window && SDLUI_Base.active_window->is_hovered)
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
    txt->do_process = true;

    if(txt->visible && txt->parent == SDLUI_Base.active_window && SDLUI_Base.active_window->is_hovered)
    {
        return true;
    }
    return false;
}

bool SDLUI_TabContainer(SDLUI_Control_TabContainer *tbc)
{
    tbc->do_process = true;
    
    if(tbc->visible && tbc->parent == SDLUI_Base.active_window && SDLUI_Base.active_window->is_hovered)
    {
        i32 mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_Rect r = {tbc->x,tbc->y,tbc->w,tbc->bar_height};
        SDL_Rect tab_r;
        i32 offset = 0;
        SDLUI_Control_Tab *tab;
        
        if(SDLUI_PointCollision(r, mx, my))
        {
            if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
            {
                for (int i = 0; i < tbc->tabs.size; ++i)
                {
                    tab = (SDLUI_Control_Tab*)tbc->tabs.data[i];
                    tab_r = {tbc->x + offset, tbc->y, tab->w + SDLUI_MARGIN, 30};
                    
                    if(SDLUI_PointCollision(tab_r, mx, my))
                    {
                        for (int j = 0; j < tbc->active_tab->children.size; ++j)
                        {
                            SDLUI_Control *ctrl = tbc->active_tab->children.data[j];
                            ctrl->visible = false;
                        }
                        tbc->active_tab = tab;
                    }
                    
                    offset += SDLUI_MARGIN + tab->w;
                }
            }
        }
    }
    return false;
}