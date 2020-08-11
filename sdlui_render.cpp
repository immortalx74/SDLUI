// Render ---------------------------------------------------

void SDLUI_Render_Button(SDLUI_Control_Button *btn)
{
    if(btn->visible)
    {
        i32 xx = btn->x - btn->parent->x;
        i32 yy = btn->y - btn->parent->y;
        
        SDL_Rect r = {xx+1, yy+1, btn->w-2, btn->h-2};
        
        switch (btn->state)
        {
            case SDLUI_BUTTON_STATE_NORMAL:
            SDL_RenderCopy(SDLUI_Base.renderer, btn->tex_back_normal, NULL, &r);
            break;
            
            case SDLUI_BUTTON_STATE_HOVER:
            SDL_RenderCopy(SDLUI_Base.renderer, btn->tex_back_hover, NULL, &r);
            break;
            
            case SDLUI_BUTTON_STATE_CLICK:
            SDL_RenderCopy(SDLUI_Base.renderer, btn->tex_back_click, NULL, &r);
            break;
        }
        
        r = {xx, yy, btn->w, btn->h};
        SDLUI_SetColor(SDLUI_Base.theme.col_border);
        SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
        
        i32 tex_w, tex_h;
        SDL_QueryTexture(btn->tex_text, NULL, NULL, &tex_w, &tex_h);
        
        i32 diff = btn->w - tex_w;
        i32 offset = diff * btn->align / 2;
        
        if(btn->align == SDLUI_ALIGN_LEFT)
        {
            offset += SDLUI_MARGIN;
        }
        else if(btn->align == SDLUI_ALIGN_RIGHT)
        {
            offset -= SDLUI_MARGIN;
        }
        
        r = {xx + offset, yy + SDLUI_MARGIN, tex_w, tex_h};
        SDL_RenderCopy(SDLUI_Base.renderer, btn->tex_text, NULL, &r);
    }
}

void SDLUI_Render_SliderInt(SDLUI_Control_SliderInt *si)
{
    if(si->visible)
    {
        if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
        {
            i32 xx = si->x - si->parent->x;
            i32 yy = si->y - si->parent->y;
            
            i32 fill = (i32)SDLUI_Map(si->min, si->max, 0, si->w, si->value);
            i32 empty = si->w - fill;
            
            SDLUI_SetColor(SDLUI_Base.theme.col_highlight);
            SDL_RenderDrawLine(SDLUI_Base.renderer, xx, yy + 8, xx + fill, yy + 8);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_border);
            SDL_RenderDrawLine(SDLUI_Base.renderer, xx + fill, yy + 8, xx + si->w, yy + 8);
            
            SDL_Rect r = {xx + fill-8, yy, 16, 16};
            r.x = SDLUI_Clamp(r.x, xx, xx + si->w - 14);
            SDLUI_Colorize(SDLUI_Base.tex_circle_fill_1, SDLUI_Base.theme.col_thumb);
            SDL_RenderCopy(SDLUI_Base.renderer, SDLUI_Base.tex_circle_fill_1, NULL, &r);
            SDLUI_Colorize(SDLUI_Base.tex_circle, SDLUI_Base.theme.col_white);
            SDL_RenderCopy(SDLUI_Base.renderer, SDLUI_Base.tex_circle, NULL, &r);
        }
        else
        {
            //NOTE: Below is old code. Should copy horizontal code and tweak it.
            
            //i32 fill = (i32)SDLUI_Map(si->min, si->max, 0, si->h, si->value);
            //i32 empty = si->h - fill;
            //
            //SDLUI_SetColor(SDLUI_Base.theme.col_border);
            //SDL_Rect r = {si->x, si->y, si->w, empty};
            //SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            //
            //SDLUI_SetColor(SDLUI_Base.theme.col_highlight);
            //r = {si->x, si->y + empty, si->w, fill};
            //SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            //
            //SDLUI_SetColor(SDLUI_Base.theme.col_thumb);
            //r = {si->x, si->y + empty, si->w, si->thumb_size};
            //r.y = SDLUI_Clamp(r.y, si->y, si->y + si->h - si->thumb_size);
            //SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            //
            //SDLUI_SetColor(SDLUI_Base.theme.col_border);
            //SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
        }
    }
}

void SDLUI_Render_CheckBox(SDLUI_Control_CheckBox *chk)
{
    if(chk->visible)
    {
        i32 xx = chk->x - chk->parent->x;
        i32 yy = chk->y - chk->parent->y;
        
        SDLUI_SetColor(SDLUI_Base.theme.col_border);
        
        SDL_Rect r = {xx, yy, chk->w, chk->h};
        SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
        
        if(chk->checked)
        {
            SDL_RenderCopy(SDLUI_Base.renderer, SDLUI_Base.tex_tick, NULL, &r);
        }
    }
}

void SDLUI_Render_ToggleButton(SDLUI_Control_ToggleButton *tb)
{
    if(tb->visible)
    {
        i32 xx = tb->x - tb->parent->x;
        i32 yy = tb->y - tb->parent->y;
        
        SDL_Rect r = {xx, yy, tb->w, tb->h};
        SDL_RenderCopy(SDLUI_Base.renderer, SDLUI_Base.tex_toggle, NULL, &r);
        
        if(tb->checked)
        {
            r = {xx + 16, yy, tb->w - 16, tb->h};
            SDLUI_Colorize(SDLUI_Base.tex_circle_fill_2, SDLUI_Base.theme.col_highlight);
            SDL_RenderCopy(SDLUI_Base.renderer, SDLUI_Base.tex_circle_fill_2, NULL, &r);
        }
        else
        {
            r = {xx, yy, tb->w - 16, tb->h};
            SDLUI_Colorize(SDLUI_Base.tex_circle_fill_2, SDLUI_Base.theme.col_white);
            SDL_RenderCopy(SDLUI_Base.renderer, SDLUI_Base.tex_circle_fill_2, NULL, &r);
        }
    }
}

void SDLUI_Render_RadioButton(SDLUI_Control_RadioButton *rb)
{
    if(rb->visible)
    {
        i32 xx = rb->x - rb->parent->x;
        i32 yy = rb->y - rb->parent->y;
        
        SDLUI_Colorize(SDLUI_Base.tex_circle, SDLUI_Base.theme.col_white);
        SDL_Rect r = {xx, yy, rb->w, rb->h};
        SDL_RenderCopy(SDLUI_Base.renderer, SDLUI_Base.tex_circle, NULL, &r);
        
        if(rb->checked)
        {
            SDLUI_Colorize(SDLUI_Base.tex_circle_fill_2, SDLUI_Base.theme.col_white);
            SDL_RenderCopy(SDLUI_Base.renderer, SDLUI_Base.tex_circle_fill_2, NULL, &r);
        }
    }
}

void SDLUI_Render_Tabcontainer(SDLUI_Control_TabContainer *tbc)
{
    if(tbc->visible)
    {
        //i32 tab_width = tbc->w / tbc->num_tabs;
        i32 tab_width = 0;
        SDL_Rect r = {tbc->x, tbc->y, 0, tbc->bar_height};
        
        for (int i = 0; i < tbc->num_tabs; ++i)
        {
            r.w = tbc->tabs[i].width;
            
            if(i == tbc->active_tab)
            {
                SDLUI_SetColor(SDLUI_Base.theme.col_highlight);
            }
            else
            {
                SDLUI_SetColor(SDLUI_Base.theme.col_thumb);
            }
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_border);
            SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
            
            i32 tex_w, tex_h;
            SDL_QueryTexture(tbc->tabs[i].tex_text, NULL, NULL, &tex_w, &tex_h);
            SDL_Rect dst = {r.x + SDLUI_MARGIN, r.y + SDLUI_MARGIN, tex_w, tex_h};
            SDL_RenderCopy(SDLUI_Base.renderer, tbc->tabs[i].tex_text, NULL, &dst);
            r.x += r.w;
        }
        
        SDLUI_SetColor(SDLUI_Base.theme.col_border);
        r = {tbc->x, tbc->y + tbc->bar_height, tbc->w, tbc->h - tbc->bar_height};
        SDL_RenderFillRect(SDLUI_Base.renderer, &r);
    }
}

void SDLUI_Render_Label(SDLUI_Control_Label *lbl)
{
    if(lbl->visible)
    {
        SDLUI_SetColor(SDLUI_Base.theme.col_white);
        SDL_Rect r = {lbl->x, lbl->y, lbl->w, lbl->h};
        SDL_RenderCopy(SDLUI_Base.renderer, lbl->tex_text, NULL, &r);
    }
}

void SDLUI_Render_Text(SDLUI_Control_Text *txt)
{
    if(txt->visible)
    {
        SDLUI_SetColor(SDLUI_Base.theme.col_white);
        SDL_Rect r = {txt->x, txt->y, txt->w, txt->h};
        SDL_RenderCopy(SDLUI_Base.renderer, txt->tex_text, NULL, &r);
    }
}

void SDLUI_RenderChild(SDLUI_CONTROL_TYPE type, SDLUI_Control *ctrl, SDL_Texture &tex)
{
    switch (type)
    {
        case SDLUI_CONTROL_TYPE_BUTTON:
        {
            SDLUI_Render_Button((SDLUI_Control_Button*)ctrl);
        }
        break;
        
        case SDLUI_CONTROL_TYPE_SLIDER_INT:
        {
            SDLUI_Render_SliderInt((SDLUI_Control_SliderInt*)ctrl);
        }
        break;
        
        case SDLUI_CONTROL_TYPE_CHECKBOX:
        {
            SDLUI_Render_CheckBox((SDLUI_Control_CheckBox*)ctrl);
        }
        break;
        
        case SDLUI_CONTROL_TYPE_TOGGLE_BUTTON:
        {
            SDLUI_Render_ToggleButton((SDLUI_Control_ToggleButton*)ctrl);
        }
        break;
        
        case SDLUI_CONTROL_TYPE_RADIO_BUTTON:
        {
            SDLUI_Render_RadioButton((SDLUI_Control_RadioButton*)ctrl);
        }
        break;
    }
}

void SDLUI_Render_Window(SDLUI_Control_Window *wnd)
{
    if(wnd->visible != wnd->visible_last_frame)
    {
        for (int i = 0; i < wnd->children.size; ++i)
        {
            wnd->children.data[i]->visible = wnd->visible;
            wnd->visible_last_frame = wnd->visible;
        }
        
    }
    
    if(wnd->enabled != wnd->enabled_last_frame)
    {
        for (int i = 0; i < wnd->children.size; ++i)
        {
            wnd->children.data[i]->enabled = wnd->enabled;
            wnd->enabled_last_frame = wnd->enabled;
        }
        
    }
    
    if(wnd->visible)
    {
        if(wnd == SDLUI_Base.active_window)
        {
            SDL_SetRenderTarget(SDLUI_Base.renderer, wnd->tex_rect);
            
            SDL_Rect r = {0, 0, wnd->w, 30};
            if(wnd == SDLUI_Base.active_window)
            {
                SDLUI_SetColor(SDLUI_Base.theme.col_active_window_bar);
            }
            else
            {
                SDLUI_SetColor(SDLUI_Base.theme.col_inactive_window_bar);
            }
            
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_white);
            i32 tex_w, tex_h;
            SDL_QueryTexture(wnd->tex_title, NULL, NULL, &tex_w, &tex_h);
            r = {SDLUI_MARGIN, SDLUI_MARGIN, tex_w, tex_h};
            SDL_RenderCopy(SDLUI_Base.renderer, wnd->tex_title, NULL, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_base);
            r = {0, 0 + 30, wnd->w, wnd->h - 30};
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_grey);
            r = {0, 0, wnd->w, wnd->h};
            SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
            
            i32 mx, my;
            SDL_GetMouseState(&mx, &my);
            r = {wnd->x + wnd->w - 30, wnd->y, 30, 30};
            
            if(wnd == SDLUI_Base.active_window && SDLUI_PointCollision(r, mx, my) && !wnd->is_resized)
            {
                SDLUI_SetColor(SDLUI_Base.theme.col_red);
                r = {wnd->w - 29, 1, 28, 28};
                SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            }
            else
            {
                SDLUI_SetColor(SDLUI_Base.theme.col_active_window_bar);
                r = {wnd->w - 29, 1, 28, 28};
                SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            }
            
            r = {0 + wnd->w - 30, 0, 30, 30};
            SDL_RenderCopy(SDLUI_Base.renderer, SDLUI_Base.tex_close, NULL, &r);
            
            
            SDLUI_CONTROL_TYPE type;
            SDLUI_Control *ctrl;
            
            for (int j = 0; j < wnd->children.size; ++j)
            {
                type = wnd->children.data[j]->type;
                ctrl = wnd->children.data[j];
                
                SDLUI_RenderChild(type, ctrl, *wnd->tex_rect);
            }
            
            r = {wnd->x, wnd->y, wnd->w, wnd->h};
            SDL_SetRenderTarget(SDLUI_Base.renderer, NULL);
            SDL_RenderCopy(SDLUI_Base.renderer, wnd->tex_rect, NULL, &r);
        }
        else
        {
            SDL_SetRenderTarget(SDLUI_Base.renderer, wnd->tex_rect);
            SDL_Rect r = {0, 0, wnd->w, 30};
            SDLUI_SetColor(SDLUI_Base.theme.col_inactive_window_bar);
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_grey);
            r = {0, 0, wnd->w, wnd->h};
            SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_white);
            i32 tex_w, tex_h;
            SDL_QueryTexture(wnd->tex_title, NULL, NULL, &tex_w, &tex_h);
            r = {SDLUI_MARGIN, SDLUI_MARGIN, tex_w, tex_h};
            SDL_RenderCopy(SDLUI_Base.renderer, wnd->tex_title, NULL, &r);
            
            r = {0 + wnd->w - 30, 0, 30, 30};
            SDL_RenderCopy(SDLUI_Base.renderer, SDLUI_Base.tex_close, NULL, &r);
            
            r = {wnd->x, wnd->y, wnd->w, wnd->h};
            SDL_SetRenderTarget(SDLUI_Base.renderer, NULL);
            SDL_RenderCopy(SDLUI_Base.renderer, wnd->tex_rect, NULL, &r);
        }
    }
}

void SDLUI_ForceReDraw()
{
    SDLUI_Control_Window *wnd;
    SDLUI_Control_Window *aw = SDLUI_Base.active_window;
    
    for (int i = 0; i < SDLUI_Window_Collection.size; ++i)
    {
        wnd = (SDLUI_Control_Window*)SDLUI_Window_Collection.data[i];
        SDLUI_Base.active_window = wnd;
        SDLUI_Render_Window(wnd);
    }
    
    SDLUI_Base.active_window = aw;
}

void SDLUI_Render()
{
    SDLUI_Control_Window *wnd;
    
    static bool once;
    if(!once)
    {
        SDLUI_ForceReDraw();
        once = true;
    }
    
    for (int i = 0; i < SDLUI_Window_Collection.size; ++i)
    {
        wnd = (SDLUI_Control_Window*)SDLUI_Window_Collection.data[i];
        SDLUI_Render_Window(wnd);
    }
    
    SDLUI_MouseStateReset();
}
