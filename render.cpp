// Render ---------------------------------------------------
void SDLUI_Render_Button(SDLUI_Control_Button *btn)
{
    if(btn->visible)
    {
        SDL_Rect r = {btn->x+1, btn->y+1, btn->w-2, btn->h-2};
        
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
        
        r = {btn->x, btn->y, btn->w, btn->h};
        SDLUI_SetColor(SDLUI_Base.theme.col_border);
        SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
        
        i32 w = SDLUI_Font.width * btn->text.length;
        i32 diffx = btn->w - w;
        i32 diffy = (btn->h - SDLUI_Font.height)/2 + 1;
        
        r = {btn->x + btn->align * diffx/2, btn->y + diffy, w, SDLUI_Font.height};
        SDL_RenderCopy(SDLUI_Base.renderer, btn->tex_text, NULL, &r);
    }
}

void SDLUI_Render_SliderInt(SDLUI_Control_SliderInt *si)
{
    if(si->visible)
    {
        if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
        {
            i32 fill = (i32)SDLUI_Map(si->min, si->max, 0, si->w, si->value);
            i32 empty = si->w - fill;
            
            SDLUI_SetColor(SDLUI_Base.theme.col_highlight);
            SDL_Rect r = {si->x, si->y, fill, si->h};
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_border);
            r = {si->x + fill, si->y, empty, si->h};
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_thumb);
            r = {si->x + fill, si->y, si->thumb_size, si->h};
            r.x = SDLUI_Clamp(r.x, si->x, si->x + si->w - si->thumb_size);
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_border);
            SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
        }
        else
        {
            i32 fill = (i32)SDLUI_Map(si->min, si->max, 0, si->h, si->value);
            i32 empty = si->h - fill;
            
            SDLUI_SetColor(SDLUI_Base.theme.col_border);
            SDL_Rect r = {si->x, si->y, si->w, empty};
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_highlight);
            r = {si->x, si->y + empty, si->w, fill};
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_thumb);
            r = {si->x, si->y + empty, si->w, si->thumb_size};
            r.y = SDLUI_Clamp(r.y, si->y, si->y + si->h - si->thumb_size);
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_border);
            SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
        }
    }
}

void SDLUI_Render_CheckBox(SDLUI_Control_CheckBox *chk)
{
    if(chk->visible)
    {
        if(chk->checked)
        {
            SDLUI_SetColor(SDLUI_Base.theme.col_border);
            SDL_Rect r = {chk->x, chk->y, chk->w, chk->h};
            SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_highlight);
            r.x += 5; r.y += 5; r.w -= 10; r.h -= 10;
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
        }
        else
        {
            SDLUI_SetColor(SDLUI_Base.theme.col_border);
            SDL_Rect r = {chk->x, chk->y, chk->w, chk->h};
            SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
        }
    }
}

void SDLUI_Render_ToggleButton(SDLUI_Control_ToggleButton *tb)
{
    if(tb->visible)
    {
        if(tb->checked)
        {
            SDLUI_SetColor(SDLUI_Base.theme.col_border);
            SDL_Rect r = {tb->x, tb->y, tb->w, tb->h};
            SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
            
            SDLUI_SetColor(SDLUI_Base.theme.col_highlight);
            r.x += (r.w/2) + 3; r.y += 3; r.w = (r.w/2) - 6; r.h -= 6;
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
        }
        else
        {
            SDLUI_SetColor(SDLUI_Base.theme.col_border);
            SDL_Rect r = {tb->x, tb->y, tb->w, tb->h};
            SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
            
            r.x += 3; r.y += 3; r.w = (r.w/2) - 6; r.h -= 6;
            SDL_RenderFillRect(SDLUI_Base.renderer, &r);
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

void SDLUI_Render_RadioButton(SDLUI_Control_RadioButton *rdb)
{
    if(rdb->visible)
    {
        SDLUI_SetColor(SDLUI_Base.theme.col_white);
        SDL_Rect src = {rdb->checked * 48, 0, 48, 48};
        SDL_Rect r = {rdb->x, rdb->y, rdb->w, rdb->h};
        SDL_RenderCopy(SDLUI_Base.renderer, rdb->tex_img, &src, &r);
    }
}

void SDLUI_Render()
{
    SDLUI_CONTROL_TYPE type;
    SDLUI_Control *ptr;
    
    for (int i = 0; i < SDLUI_Collection.used; ++i)
    {
        type = SDLUI_Collection.elements[i]->type;
        ptr = SDLUI_Collection.elements[i];
        
        switch (type)
        {
            case SDLUI_CONTROL_TYPE_BUTTON:
            {
                SDLUI_Render_Button((SDLUI_Control_Button*)ptr);
            }
            break;
            
            case SDLUI_CONTROL_TYPE_TEXT:
            {
                SDLUI_Render_Text((SDLUI_Control_Text*)ptr);
            }
            break;
            
            case SDLUI_CONTROL_TYPE_SLIDER_INT:
            {
                SDLUI_Render_SliderInt((SDLUI_Control_SliderInt*)ptr);
            }
            break;
            
            case SDLUI_CONTROL_TYPE_CHECKBOX:
            {
                SDLUI_Render_CheckBox((SDLUI_Control_CheckBox*)ptr);
            }
            break;
            
            case SDLUI_CONTROL_TYPE_TOGGLE_BUTTON:
            {
                SDLUI_Render_ToggleButton((SDLUI_Control_ToggleButton*)ptr);
            }
            break;
        }
    }
    
    SDLUI_MouseStateReset();
}
