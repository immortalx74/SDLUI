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

#define MAX_CONTROLS 50

enum ALIGN
{
    LEFT = 0,
    CENTER = 1,
    RIGHT = 2
};

enum MOUSE_STATE
{
    NONE,
    PRESSED,
    HELD,
    RELEASED
};

enum CONTROL_TYPE
{
    Button,
    SliderInt
};

struct THEME
{
    SDL_Color col_base = {75, 75, 75, 255};
    SDL_Color col_border = {38, 38, 38, 255};
    SDL_Color col_hover = {80, 80, 80, 255};
    SDL_Color col_click = {40, 40, 40, 255};
};

struct BUTTON
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    char *caption;
    bool render;
    ALIGN align;
    SDL_Texture *t_text;
    SDL_Texture *t_back;
};

struct SLIDER_INT
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    i32 min;
    i32 max;
    i32 value;
};

struct CONTROL
{
    CONTROL_TYPE type;
    
    union
    {
        BUTTON button;
        SLIDER_INT slider_int;
    };
};

struct UI_FONT
{
    TTF_Font *handle;
    i32 size = 16;
    i32 width;
    i32 height;
};

struct UI_BASE
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    i32 window_width;
    i32 window_height;
    
    i32 count = 0;
    CONTROL *pool;
    
    u8 mouse_current_frame[5] = {0};
    u8 mouse_last_frame[5] = {0};
    THEME theme;
    UI_FONT font;
    
}ui_base;

bool ui_PointCollision(SDL_Rect rect, i32 x, i32 y)
{
    if(x >= rect.x && x <= rect.x + rect.w &&
       y >= rect.y && y <= rect.y +rect.h)
    {
        return true;
    }
    return false;
}

void ui_Init(SDL_Renderer *r, SDL_Window *w)
{
    TTF_Init();
    ui_base.font.handle = TTF_OpenFont("liberation-mono.ttf", ui_base.font.size);
    ui_base.font.height = TTF_FontHeight(ui_base.font.handle);
    TTF_SizeText(ui_base.font.handle, "0", &ui_base.font.width, &ui_base.font.height);
    
    
    ui_base.renderer = r;
    ui_base.window = w;
    SDL_GetWindowSize(ui_base.window, &ui_base.window_width, &ui_base.window_height);
    ui_base.pool = (CONTROL*)malloc(MAX_CONTROLS * sizeof(CONTROL));
}

void ui_MouseStateReset()
{
    for (int i = 0; i < 5; ++i)
    {
        ui_base.mouse_last_frame[i] = ui_base.mouse_current_frame[i];
    }
}

void ui_MouseStateSet(i32 event_type, i32 mbutton)
{
    if(event_type == SDL_MOUSEBUTTONDOWN)
    {
        ui_base.mouse_current_frame[mbutton] = 1;
    }
    else
    {
        ui_base.mouse_current_frame[mbutton] = 0;
    }
}

MOUSE_STATE ui_MouseButton(i32 mbutton)
{
    if(ui_base.mouse_current_frame[mbutton] && !ui_base.mouse_last_frame[mbutton])
    {
        return MOUSE_STATE::PRESSED;
    }
    else if(ui_base.mouse_current_frame[mbutton] && ui_base.mouse_last_frame[mbutton])
    {
        return MOUSE_STATE::HELD;
    }
    else if(!ui_base.mouse_current_frame[mbutton] && ui_base.mouse_last_frame[mbutton])
    {
        return MOUSE_STATE::RELEASED;
    }
    
    return MOUSE_STATE::NONE;
}

void ui_GradientToTexture(SDL_Texture *t, SDL_Color c, i32 width, i32 height, i32 interv)
{
    SDL_SetRenderTarget(ui_base.renderer, t);
    
    i32 x = 0, y = 0;
    
    for (int i = 0; i < height; ++i)
    {
        SDL_SetRenderDrawColor(ui_base.renderer, c.r, c.g, c.b, 255);
        SDL_RenderDrawLine(ui_base.renderer, x, y + i, width, y + i);
        
        if(i % interv == 0)
        {
            c.r -= interv;
            c.g -= interv;
            c.b -= interv;
        }
    }
    
    SDL_SetRenderTarget(ui_base.renderer, NULL);
}

//////////////////////////
// Begin control creation
/////////////////////////

BUTTON *ui_CreateButton(char * caption, i32 x, i32 y)
{
    if(ui_base.count < MAX_CONTROLS)
    {
        CONTROL *ctrl = &ui_base.pool[ui_base.count];
        ui_base.count++;
        
        ctrl->type = CONTROL_TYPE::Button;
        ctrl->button.caption = caption;
        ctrl->button.x = x;
        ctrl->button.y = y;
        ctrl->button.w = 160;
        ctrl->button.h = 30;
        ctrl->button.render = false;
        ctrl->button.align = ALIGN::CENTER;
        
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(ui_base.font.handle,caption, c);
        ctrl->button.t_text = SDL_CreateTextureFromSurface(ui_base.renderer, s);
        ctrl->button.t_back = SDL_CreateTexture(ui_base.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ctrl->button.w - 2, ctrl->button.h - 2);
        
        ui_GradientToTexture(ctrl->button.t_back, ui_base.theme.col_base, ctrl->button.w-2, ctrl->button.h-2, (ctrl->button.h-2)/12);
        
        return &ctrl->button;
    }
    
    return NULL;
}

bool ui_Button(BUTTON *btn)
{
    btn->render = true;
    i32 mx, my;
    SDL_GetMouseState(&mx, &my);
    
    SDL_Rect r = {btn->x,btn->y,btn->w,btn->h};
    if(ui_PointCollision(r, mx, my))
    {
        if(ui_MouseButton(SDL_BUTTON_LEFT) == MOUSE_STATE::PRESSED)
        {
            SDL_SetTextureColorMod(btn->t_back, 100, 100, 100);
            return true;
        }
        if(ui_MouseButton(SDL_BUTTON_LEFT) == MOUSE_STATE::HELD)
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

SLIDER_INT *ui_CreateSliderInt(i32 x, i32 y, i32 min, i32 max, i32 value)
{
    if(ui_base.count < MAX_CONTROLS)
    {
        CONTROL *ctrl = &ui_base.pool[ui_base.count];
        ui_base.count++;
        
        ctrl->type = CONTROL_TYPE::SliderInt;
        ctrl->slider_int.x = x;
        ctrl->slider_int.y = y;
        ctrl->slider_int.w = 50;
        ctrl->slider_int.h = 10;
        ctrl->slider_int.min = min;
        ctrl->slider_int.max = max;
        ctrl->slider_int.value = value;
        
        return &ctrl->slider_int;
    }
    
    return NULL;
}

void ui_SetColor(SDL_Color c)
{
    SDL_SetRenderDrawColor(ui_base.renderer, c.r, c.g, c.b, c.a);
}

void ui_Render()
{
    for (int i = 0; i < ui_base.count; ++i)
    {
        if(ui_base.pool[i].type == CONTROL_TYPE::Button)
        {
            BUTTON *btn = &ui_base.pool[i].button;
            
            if(btn->render)
            {
                SDL_Rect r = {btn->x, btn->y, btn->w, btn->h};
                SDL_Rect testrect = {btn->x+1, btn->y+1, btn->w-2, btn->h-2};
                SDL_RenderCopy(ui_base.renderer, btn->t_back, NULL, &testrect);
                
                ui_SetColor(ui_base.theme.col_border);
                SDL_RenderDrawRect(ui_base.renderer, &r);
                
                i32 w = ui_base.font.width * strlen(btn->caption);
                i32 diffx = btn->w - w;
                i32 diffy = (btn->h - ui_base.font.height)/2 + 1;
                
                r = {btn->x + btn->align * diffx/2, btn->y + diffy, w, ui_base.font.height};
                SDL_RenderCopy(ui_base.renderer, btn->t_text, NULL, &r);
            }
        }
        else if(ui_base.pool[i].type == CONTROL_TYPE::SliderInt)
        {
            SLIDER_INT *si = &ui_base.pool[i].slider_int;
            SDL_SetRenderDrawColor(ui_base.renderer, 255, 0, 0, 255);
            SDL_Rect r = {si->x, si->y, si->w, si->h};
            SDL_RenderDrawRect(ui_base.renderer, &r);
        }
    }
    
    ui_MouseStateReset();
}