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
    SDLUI_CONTROL_TYPE_SLIDER_INT
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
    SDL_Texture *t_text;
    SDL_Texture *t_back;
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

struct SDLUI_Control
{
    SDLUI_CONTROL_TYPE type;
    
    union
    {
        SDLUI_Control_Button button;
        SDLUI_Control_SliderInt slider_int;
    };
};

struct SDLUI_Font
{
    TTF_Font *handle;
    i32 size = 16;
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

void SDLUI_Render()
{
    for (int i = 0; i < SDLUI_Base.count; ++i)
    {
        if(SDLUI_Base.pool[i].type == SDLUI_CONTROL_TYPE_BUTTON)
        {
            SDLUI_Control_Button *btn = &SDLUI_Base.pool[i].button;
            
            if(btn->render)
            {
                SDL_Rect r = {btn->x+1, btn->y+1, btn->w-2, btn->h-2};
                SDL_RenderCopy(SDLUI_Base.renderer, btn->t_back, NULL, &r);
                
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
        else if(SDLUI_Base.pool[i].type == SDLUI_CONTROL_TYPE_SLIDER_INT)
        {
            SDLUI_Control_SliderInt *si = &SDLUI_Base.pool[i].slider_int;
            
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
    }
    
    SDLUI_MouseStateReset();
}

// Controls ---------------------------------------------------

SDLUI_Control_Button *SDLUI_CreateButton(char * caption, i32 x, i32 y)
{
    if(SDLUI_Base.count < SDLUI_MAX_CONTROLS)
    {
        SDLUI_Control *ctrl = &SDLUI_Base.pool[SDLUI_Base.count];
        SDLUI_Base.count++;
        
        ctrl->type = SDLUI_CONTROL_TYPE::SDLUI_CONTROL_TYPE_BUTTON;
        ctrl->button.caption = caption;
        ctrl->button.x = x;
        ctrl->button.y = y;
        ctrl->button.w = 160;
        ctrl->button.h = 30;
        ctrl->button.render = false;
        ctrl->button.align = SDLUI_ALIGN_CENTER;
        
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Base.font.handle,caption, c);
        ctrl->button.t_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        ctrl->button.t_back = SDL_CreateTexture(SDLUI_Base.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctrl->button.w - 2, ctrl->button.h - 2);
        
        SDLUI_GradientToTexture(ctrl->button.t_back, SDLUI_Base.theme.col_base, ctrl->button.w-2, ctrl->button.h-2, (ctrl->button.h-2)/12);
        
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
            SDL_SetTextureColorMod(btn->t_back, 100, 100, 100);
            return true;
        }
        if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD)
        {
            SDL_SetTextureColorMod(btn->t_back, 100, 100, 100);
        }
        else
        {
            SDL_SetTextureColorMod(btn->t_back, 180, 180, 255);
        }
        
        return false;
    }
    
    SDL_SetTextureColorMod(btn->t_back, 255, 255, 255);
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
        ctrl->slider_int.thumb_size = 10;
        ctrl->slider_int.render = false;
        ctrl->slider_int.ischanging = false;
        ctrl->slider_int.orientation = orientation;
        
        if(ctrl->slider_int.orientation == SDLUI_ORIENTATION_HORIZONTAL)
        {
            ctrl->slider_int.w = 100;
            ctrl->slider_int.h = 10;
        }
        else
        {
            ctrl->slider_int.w = 10;
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
            return true;
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
        
        return true;
    }
    
    if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED)
    {
        si->ischanging = false;
        return false;
    }
    
    return false;
}