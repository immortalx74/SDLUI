#pragma once

#include <iostream>
#include "SDL_ttf.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define SDLUI_MAX_CONTROLS 50

// Enums ----------------------------------------------

enum SDLUI_BUTTON_STATE
{
    SDLUI_BUTTON_STATE_NORMAL,
    SDLUI_BUTTON_STATE_HOVER,
    SDLUI_BUTTON_STATE_CLICK
};

enum SDLUI_ALIGN
{
    SDLUI_ALIGN_LEFT = 0,
    SDLUI_ALIGN_CENTER = 1,
    SDLUI_ALIGN_RIGHT = 2
};

enum SDLUI_ORIENTATION
{
    SDLUI_ORIENTATION_HORIZONTAL,
    SDLUI_ORIENTATION_VERTICAL
};

enum SDLUI_MOUSEBUTTON
{
    SDLUI_MOUSEBUTTON_NONE,
    SDLUI_MOUSEBUTTON_PRESSED,
    SDLUI_MOUSEBUTTON_HELD,
    SDLUI_MOUSEBUTTON_RELEASED
};

enum SDLUI_CONTROL_TYPE
{
    SDLUI_CONTROL_TYPE_BUTTON,
    SDLUI_CONTROL_TYPE_SLIDER_INT,
    SDLUI_CONTROL_TYPE_CHECKBOX,
    SDLUI_CONTROL_TYPE_TOGGLEBUTTON,
    SDLUI_CONTROL_TYPE_TAB_CONTAINER
};


// Structs --------------------------------------------

struct SDLUI_Theme
{
    SDL_Color col_base = {75, 75, 75, 255};
    SDL_Color col_border = {38, 38, 38, 255};
    SDL_Color col_hover = {80, 80, 80, 255};
    SDL_Color col_click = {40, 40, 40, 255};
    SDL_Color col_highlight = {65, 105, 225, 255};
    SDL_Color col_test = {255, 0, 225, 255};
    SDL_Color col_thumb = {66, 66, 66, 255};
    SDL_Color col_white = {255, 255, 255, 255};
    SDL_Color col_grey = {127, 127, 127, 255};
};

struct SDLUI_Control_Button
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    char *caption;
    bool render;
    SDLUI_ALIGN align;
    SDLUI_BUTTON_STATE state;
    SDL_Texture *t_text;
    SDL_Texture *t_back_normal;
    SDL_Texture *t_back_hover;
    SDL_Texture *t_back_click;
};

struct SDLUI_Control_SliderInt
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    i32 min;
    i32 max;
    i32 value;
    SDLUI_ORIENTATION orientation;
    i32 thumb_size;
    bool render;
    bool ischanging;
};

struct SDLUI_Control_CheckBox
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    bool render;
    bool checked;
};

struct SDLUI_Control_ToggleButton
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    bool render;
    bool checked;
};

struct SDLUI_Control_Tab
{
    char *caption;
    i32 caption_length;
    i32 index;
    SDL_Texture *t_text;
};

struct SDLUI_Control_TabContainer
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    i32 tab_height;
    i32 num_tabs;
    i32 active_tab;
    bool render;
    SDLUI_Control_Tab *tabs;
};

struct SDLUI_Control
{
    union
    {
        SDLUI_Control_Button button;
        SDLUI_Control_SliderInt slider_int;
        SDLUI_Control_CheckBox checkbox;
        SDLUI_Control_ToggleButton toggle_button;
        SDLUI_Control_TabContainer tab_container;
    };
    
    SDLUI_CONTROL_TYPE type;
};

struct SDLUI_Font
{
    TTF_Font *handle;
    i32 size = 13;
    i32 width;
    i32 height;
};

struct __SDLUI_Base
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    i32 window_width;
    i32 window_height;
    
    i32 count = 0;
    SDLUI_Control *pool;
    
    u8 mouse_current_frame[5] = {0};
    u8 mouse_last_frame[5] = {0};
    SDLUI_Theme theme;
    SDLUI_Font font;
    
}SDLUI_Base;


// Functions ----------------------------------------------

float SDLUI_Map(float in_min, float in_max, float out_min, float out_max, float value)
{
    float slope = (out_max - out_min) / (in_max - in_min);
    return out_min + (slope * (value - in_min));
}

float SDLUI_Clamp(float n, float min, float max)
{
    if (n < min) n = min;
    if (n > max) n = max;
    return n;
}

bool SDLUI_PointCollision(SDL_Rect rect, i32 x, i32 y)
{
    if(x >= rect.x && x <= rect.x + rect.w &&
       y >= rect.y && y <= rect.y +rect.h)
    {
        return true;
    }
    return false;
}

void SDLUI_Init(SDL_Renderer *r, SDL_Window *w)
{
    TTF_Init();
    SDLUI_Base.font.handle = TTF_OpenFont("liberation-mono.ttf", SDLUI_Base.font.size);
    SDLUI_Base.font.height = TTF_FontHeight(SDLUI_Base.font.handle);
    TTF_SizeText(SDLUI_Base.font.handle, "0", &SDLUI_Base.font.width, &SDLUI_Base.font.height);
    
    
    SDLUI_Base.renderer = r;
    SDLUI_Base.window = w;
    SDL_GetWindowSize(SDLUI_Base.window, &SDLUI_Base.window_width, &SDLUI_Base.window_height);
    SDLUI_Base.pool = (SDLUI_Control*)malloc(SDLUI_MAX_CONTROLS * sizeof(SDLUI_Control));
}

void SDLUI_MouseStateReset()
{
    for (int i = 0; i < 5; ++i)
    {
        SDLUI_Base.mouse_last_frame[i] = SDLUI_Base.mouse_current_frame[i];
    }
}

void SDLUI_MouseStateSet(i32 event_type, i32 mbutton)
{
    //if(event_type == SDL_MOUSEBUTTONDOWN)
    //{
    //SDLUI_Base.mouse_current_frame[mbutton] = 1;
    //}
    //else
    //{
    //SDLUI_Base.mouse_current_frame[mbutton] = 0;
    //}
    SDLUI_Base.mouse_current_frame[mbutton] = (event_type == SDL_MOUSEBUTTONDOWN || event_type == SDL_MOUSEMOTION);
}

SDLUI_MOUSEBUTTON SDLUI_MouseButton(i32 mbutton)
{
    if(SDLUI_Base.mouse_current_frame[mbutton] && !SDLUI_Base.mouse_last_frame[mbutton])
    {
        return SDLUI_MOUSEBUTTON_PRESSED;
    }
    else if(SDLUI_Base.mouse_current_frame[mbutton] && SDLUI_Base.mouse_last_frame[mbutton])
    {
        return SDLUI_MOUSEBUTTON_HELD;
    }
    else if(!SDLUI_Base.mouse_current_frame[mbutton] && SDLUI_Base.mouse_last_frame[mbutton])
    {
        return SDLUI_MOUSEBUTTON_RELEASED;
    }
    
    return SDLUI_MOUSEBUTTON_NONE;
}

void SDLUI_GradientToTexture(SDL_Texture *t, SDL_Color c, i32 width, i32 height, i32 interv)
{
    SDL_SetRenderTarget(SDLUI_Base.renderer, t);
    
    i32 x = 0, y = 0;
    
    for (int i = 0; i < height; ++i)
    {
        SDL_SetRenderDrawColor(SDLUI_Base.renderer, c.r, c.g, c.b, 255);
        SDL_RenderDrawLine(SDLUI_Base.renderer, x, y + i, width, y + i);
        
        if(i % interv == 0)
        {
            c.r -= interv;
            c.g -= interv;
            c.b -= interv;
        }
    }
    
    SDL_SetRenderTarget(SDLUI_Base.renderer, NULL);
}

void SDLUI_SetColor(SDL_Color c)
{
    SDL_SetRenderDrawColor(SDLUI_Base.renderer, c.r, c.g, c.b, c.a);
}

void SDLUI_Render_Button(SDLUI_Control_Button *btn)
{
    if(btn->render)
    {
        SDL_Rect r = {btn->x+1, btn->y+1, btn->w-2, btn->h-2};
        
        switch (btn->state)
        {
            case SDLUI_BUTTON_STATE_NORMAL:
            SDL_RenderCopy(SDLUI_Base.renderer, btn->t_back_normal, NULL, &r);
            break;
            
            case SDLUI_BUTTON_STATE_HOVER:
            SDL_RenderCopy(SDLUI_Base.renderer, btn->t_back_hover, NULL, &r);
            break;
            
            case SDLUI_BUTTON_STATE_CLICK:
            SDL_RenderCopy(SDLUI_Base.renderer, btn->t_back_click, NULL, &r);
            break;
        }
        
        r = {btn->x, btn->y, btn->w, btn->h};
        SDLUI_SetColor(SDLUI_Base.theme.col_border);
        SDL_RenderDrawRect(SDLUI_Base.renderer, &r);
        
        i32 w = SDLUI_Base.font.width * strlen(btn->caption);
        i32 diffx = btn->w - w;
        i32 diffy = (btn->h - SDLUI_Base.font.height)/2 + 1;
        
        r = {btn->x + btn->align * diffx/2, btn->y + diffy, w, SDLUI_Base.font.height};
        SDL_RenderCopy(SDLUI_Base.renderer, btn->t_text, NULL, &r);
    }
}

void SDLUI_Render_SliderInt(SDLUI_Control_SliderInt *si)
{
    if(si->render)
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
    if(chk->render)
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
    if(tb->render)
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
    if(tbc->render)
    {
        i32 tab_width = tbc->w / tbc->num_tabs;
        SDL_Rect r = {tbc->x, tbc->y, tab_width, tbc->tab_height};
        
        for (int i = 0; i < tbc->num_tabs; ++i)
        {
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
            
            i32 w = SDLUI_Base.font.width * tbc->tabs[i].caption_length;
            i32 h = SDLUI_Base.font.height;
            SDL_Rect t = {r.x+8, r.y+8, w, h};
            SDL_RenderCopy(SDLUI_Base.renderer, tbc->tabs[i].t_text, NULL, &t);
            r.x += tab_width;
            t.x += tab_width;
        }
        
        SDLUI_SetColor(SDLUI_Base.theme.col_border);
        r = {tbc->x, tbc->y + tbc->tab_height + 1, tbc->w, tbc->h - tbc->tab_height - 1};
        SDL_RenderFillRect(SDLUI_Base.renderer, &r);
    }
}

void SDLUI_Render()
{
    for (int i = 0; i < SDLUI_Base.count; ++i)
    {
        switch (SDLUI_Base.pool[i].type)
        {
            case SDLUI_CONTROL_TYPE_BUTTON:
            {
                SDLUI_Control_Button *btn = &SDLUI_Base.pool[i].button;
                SDLUI_Render_Button(btn);
            }
            break;
            
            case SDLUI_CONTROL_TYPE_SLIDER_INT:
            {
                SDLUI_Control_SliderInt *si = &SDLUI_Base.pool[i].slider_int;
                SDLUI_Render_SliderInt(si);
            }
            break;
            
            case SDLUI_CONTROL_TYPE_CHECKBOX:
            {
                SDLUI_Control_CheckBox *chk = &SDLUI_Base.pool[i].checkbox;
                SDLUI_Render_CheckBox(chk);
            }
            break;
            
            case SDLUI_CONTROL_TYPE_TOGGLEBUTTON:
            {
                SDLUI_Control_ToggleButton *tb = &SDLUI_Base.pool[i].toggle_button;
                SDLUI_Render_ToggleButton(tb);
            }
            break;
            
            case SDLUI_CONTROL_TYPE_TAB_CONTAINER:
            {
                SDLUI_Control_TabContainer *tbc = &SDLUI_Base.pool[i].tab_container;
                SDLUI_Render_Tabcontainer(tbc);
            }
            break;
        }
    }
    
    SDLUI_MouseStateReset();
}

// Controls ---------------------------------------------------
void SDLUI_Destroy_Control(void  *ctrl)
{
    i32 offset = (u8*)ctrl - (u8*)SDLUI_Base.pool;
    i32 index = offset / sizeof(SDLUI_Control);
    
    std::cout << offset << std::endl;
}

SDLUI_Control_Button *SDLUI_CreateButton(char * caption, i32 x, i32 y)
{
    if(SDLUI_Base.count < SDLUI_MAX_CONTROLS)
    {
        SDLUI_Control *ctrl = &SDLUI_Base.pool[SDLUI_Base.count];
        SDLUI_Base.count++;
        
        ctrl->type = SDLUI_CONTROL_TYPE_BUTTON;
        ctrl->button.caption = caption;
        ctrl->button.x = x;
        ctrl->button.y = y;
        ctrl->button.w = 100;
        ctrl->button.h = 30;
        ctrl->button.render = false;
        ctrl->button.align = SDLUI_ALIGN_CENTER;
        ctrl->button.state = SDLUI_BUTTON_STATE_NORMAL;
        
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Base.font.handle,caption, c);
        ctrl->button.t_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        ctrl->button.t_back_normal = SDL_CreateTexture(SDLUI_Base.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctrl->button.w - 2, ctrl->button.h - 2);
        ctrl->button.t_back_hover = SDL_CreateTexture(SDLUI_Base.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctrl->button.w - 2, ctrl->button.h - 2);
        ctrl->button.t_back_click = SDL_CreateTexture(SDLUI_Base.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctrl->button.w - 2, ctrl->button.h - 2);
        
        SDLUI_GradientToTexture(ctrl->button.t_back_normal, SDLUI_Base.theme.col_base, ctrl->button.w-2, ctrl->button.h-2, (ctrl->button.h-2)/12);
        SDLUI_GradientToTexture(ctrl->button.t_back_hover, SDLUI_Base.theme.col_highlight, ctrl->button.w-2, ctrl->button.h-2, (ctrl->button.h-2)/12);
        SDLUI_GradientToTexture(ctrl->button.t_back_click, SDLUI_Base.theme.col_border, ctrl->button.w-2, ctrl->button.h-2, (ctrl->button.h-2)/12);
        
        return &ctrl->button;
    }
    
    return NULL;
}

bool SDLUI_Button(SDLUI_Control_Button *btn)
{
    btn->render = true;
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

SDLUI_Control_SliderInt *SDLUI_CreateSliderInt(i32 x, i32 y, i32 min, i32 max, i32 value, SDLUI_ORIENTATION orientation = SDLUI_ORIENTATION_HORIZONTAL)
{
    if(SDLUI_Base.count < SDLUI_MAX_CONTROLS)
    {
        SDLUI_Control *ctrl = &SDLUI_Base.pool[SDLUI_Base.count];
        SDLUI_Base.count++;
        
        ctrl->type = SDLUI_CONTROL_TYPE_SLIDER_INT;
        ctrl->slider_int.x = x;
        ctrl->slider_int.y = y;
        ctrl->slider_int.min = min;
        ctrl->slider_int.max = max;
        ctrl->slider_int.value = value;
        ctrl->slider_int.thumb_size = 12;
        ctrl->slider_int.render = false;
        ctrl->slider_int.ischanging = false;
        ctrl->slider_int.orientation = orientation;
        
        if(ctrl->slider_int.orientation == SDLUI_ORIENTATION_HORIZONTAL)
        {
            ctrl->slider_int.w = 100;
            ctrl->slider_int.h = 12;
        }
        else
        {
            ctrl->slider_int.w = 12;
            ctrl->slider_int.h = 100;
        }
        
        return &ctrl->slider_int;
    }
    
    return NULL;
}

bool SDLUI_SliderInt(SDLUI_Control_SliderInt *si)
{
    si->render = true;
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

SDLUI_Control_CheckBox *SDLUI_CreateCheckBox(i32 x, i32 y, bool checked)
{
    if(SDLUI_Base.count < SDLUI_MAX_CONTROLS)
    {
        SDLUI_Control *ctrl = &SDLUI_Base.pool[SDLUI_Base.count];
        SDLUI_Base.count++;
        
        ctrl->type = SDLUI_CONTROL_TYPE_CHECKBOX;
        ctrl->checkbox.x = x;
        ctrl->checkbox.y = y;
        ctrl->checkbox.w = 20;
        ctrl->checkbox.h = 20;
        ctrl->checkbox.render = false;
        ctrl->checkbox.checked = false;
        
        return &ctrl->checkbox;
    }
    
    return NULL;
}

bool SDLUI_CheckBox(SDLUI_Control_CheckBox *chk)
{
    chk->render = true;
    
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

SDLUI_Control_ToggleButton *SDLUI_CreateToggleButton(i32 x, i32 y, bool checked)
{
    if(SDLUI_Base.count < SDLUI_MAX_CONTROLS)
    {
        SDLUI_Control *ctrl = &SDLUI_Base.pool[SDLUI_Base.count];
        SDLUI_Base.count++;
        
        ctrl->type = SDLUI_CONTROL_TYPE_TOGGLEBUTTON;
        ctrl->toggle_button.x = x;
        ctrl->toggle_button.y = y;
        ctrl->toggle_button.w = 40;
        ctrl->toggle_button.h = 20;
        ctrl->toggle_button.render = false;
        ctrl->toggle_button.checked = false;
        
        return &ctrl->toggle_button;
    }
    
    return NULL;
}

bool SDLUI_ToggleButton(SDLUI_Control_ToggleButton *tb)
{
    tb->render = true;
    
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

SDLUI_Control_TabContainer *SDLUI_CreateTabContainer(i32 x, i32 y)
{
    if(SDLUI_Base.count < SDLUI_MAX_CONTROLS)
    {
        SDLUI_Control *ctrl = &SDLUI_Base.pool[SDLUI_Base.count];
        SDLUI_Base.count++;
        
        ctrl->type = SDLUI_CONTROL_TYPE_TAB_CONTAINER;
        ctrl->tab_container.x = x;
        ctrl->tab_container.y = y;
        ctrl->tab_container.w = 300;
        ctrl->tab_container.h = 200;
        ctrl->tab_container.tab_height = 30;
        ctrl->tab_container.num_tabs = 0;
        ctrl->tab_container.active_tab = 0;
        ctrl->tab_container.render = false;
        
        return &ctrl->tab_container;
    }
    
    return NULL;
}

void SDLUI_CreateTab(SDLUI_Control_TabContainer *tbc, char *caption)
{
    if(tbc->num_tabs == 0)
    {
        tbc->tabs = (SDLUI_Control_Tab*)malloc(1 * sizeof(SDLUI_Control_Tab));
    }
    else
    {
        i32 new_size = (tbc->num_tabs + 1) * sizeof(SDLUI_Control_Tab);
        tbc->tabs = (SDLUI_Control_Tab*)realloc((SDLUI_Control_Tab*)tbc->tabs, new_size);
    }
    
    tbc->num_tabs++;
    SDLUI_Control_Tab tab;
    tab.index = tbc->num_tabs - 1;
    tab.caption_length = strlen(caption);
    tab.caption = (char*)malloc(tab.caption_length + 1);
    memcpy(tab.caption, caption, tab.caption_length);
    tab.caption[tab.caption_length] = 0;
    SDL_Color c = {255, 255, 255, 255};
    SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Base.font.handle,caption, c);
    tab.t_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
    
    tbc->tabs[tab.index] = tab;
}

bool SDLUI_TabContainer(SDLUI_Control_TabContainer *tbc)
{
    tbc->render = true;
    
    i32 mx, my;
    SDL_GetMouseState(&mx, &my);
    
    SDL_Rect r = {tbc->x,tbc->y,tbc->w,tbc->tab_height};
    if(SDLUI_PointCollision(r, mx, my))
    {
        if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
        {
            tbc->active_tab = (mx - tbc->x) / (tbc->w / tbc->num_tabs);
            
            return true;
        }
    }
    
    return false;
}