#pragma once

#include <iostream>
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define SDLUI_COLLECTION_CHUNK 10
#define SDLUI_STRING_CAPACITY 20
#define SDLUI_MARGIN 8

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
    SDLUI_CONTROL_TYPE_TOGGLE_BUTTON,
    SDLUI_CONTROL_TYPE_TAB_CONTAINER,
    SDLUI_CONTROL_TYPE_LABEL,
    SDLUI_CONTROL_TYPE_TEXT,
    SDLUI_CONTROL_TYPE_RADIO_BUTTON
};

// Structs --------------------------------------------

struct SDLUI_String
{
    i32 capacity = 0;
    i32 length = 0;
    bool modified = false;
    char *data;
    
    void create(char *str)
    {
        length = strlen(str) + 1;
        capacity = ((length / SDLUI_STRING_CAPACITY) + 1) * SDLUI_STRING_CAPACITY;
        data = (char*)malloc(length);
        memcpy(data, str, length - 1);
        data[length - 1] = 0;
    }
    
    void destroy()
    {
        free(data);
        data = NULL;
    }
    
    void modify(char *str)
    {
        i32 new_length = strlen(str) + 1;
        if(new_length >= capacity)
        {
            capacity = ((new_length / SDLUI_STRING_CAPACITY) + 1) * SDLUI_STRING_CAPACITY;
            data = (char*)realloc(data, capacity);
            memcpy(data, str, new_length - 1);
            memset(data + new_length - 1, 0, capacity - new_length);
            length = new_length - 1;
        }
        else
        {
            memcpy(data, str, new_length - 1);
            memset(data + new_length - 1, 0, capacity - new_length);
            length = new_length - 1;
        }
        
        modified = true;
    }
};

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

struct SDLUI_Control
{
    SDLUI_CONTROL_TYPE type;
    i32 x;
    i32 y;
    bool visible;
};

struct __SDLUI_Collection
{
    i32 capacity = 0;
    i32 used = 0;
    SDLUI_Control **elements;
    
    void create()
    {
        capacity = SDLUI_COLLECTION_CHUNK * sizeof(SDLUI_Control*);
        elements = (SDLUI_Control**)malloc(capacity);
    }
    
    void ensure_capacity()
    {
        if(used + 1 >= capacity)
        {
            capacity += SDLUI_COLLECTION_CHUNK;
            elements = (SDLUI_Control**)realloc(elements, capacity);
        }
    }
    
    void push(SDLUI_Control *elem)
    {
        ensure_capacity();
        elements[used] = elem;
        used++;
    }
    
    bool pop(SDLUI_Control *elem)
    {
        for (int i = 0; i < used; ++i)
        {
            if(elem == elements[i])
            {
                i32 num_elements = used - i - 1;
                memcpy(elements[i], elements[i + 1], num_elements);
                used--;
                return true;
            }
        }
        
        return false;
    }
    
}SDLUI_Collection;

struct SDLUI_Control_Button : SDLUI_Control
{
    i32 w;
    i32 h;
    SDLUI_String text;
    SDLUI_ALIGN align;
    SDLUI_BUTTON_STATE state;
    SDL_Texture *tex_text;
    SDL_Texture *tex_back_normal;
    SDL_Texture *tex_back_hover;
    SDL_Texture *tex_back_click;
};

struct SDLUI_Control_SliderInt : SDLUI_Control
{
    i32 w;
    i32 h;
    i32 min;
    i32 max;
    i32 value;
    SDLUI_ORIENTATION orientation;
    i32 thumb_size;
    bool ischanging;
};

struct SDLUI_Control_CheckBox : SDLUI_Control
{
    i32 w;
    i32 h;
    bool checked;
};

struct SDLUI_Control_ToggleButton : SDLUI_Control
{
    i32 w;
    i32 h;
    bool checked;
};

struct SDLUI_Control_Tab : SDLUI_Control
{
    i32 width;
    i32 index;
    SDLUI_String text;
    SDL_Texture *tex_text;
};

struct SDLUI_Control_TabContainer : SDLUI_Control
{
    i32 w;
    i32 h;
    i32 bar_height;
    i32 num_tabs;
    i32 active_tab;
    SDLUI_Control_Tab *tabs;
    i32 num_children;
    SDLUI_Control **children;
};

struct SDLUI_Control_Label : SDLUI_Control
{
    i32 w;
    i32 h;
    SDL_Texture *tex_text;
};

struct SDLUI_Control_Text : SDLUI_Control
{
    i32 w;
    i32 h;
    SDLUI_String text;
    bool modified;
    SDL_Texture *tex_text;
};

struct SDLUI_Control_RadioButton : SDLUI_Control
{
    i32 w;
    i32 h;
    i32 group;
    bool checked;
    SDL_Texture *tex_img;
};

struct __SDLUI_RadioButtonGroups
{
    i32 num_elements = 0;
    i32 *groups;
    SDLUI_Control_RadioButton *collection;
}SDLUI_RadioButtonGroups;

struct __SDLUI_Font
{
    TTF_Font *handle;
    i32 size = 13;
    i32 width;
    i32 height;
}SDLUI_Font;

struct __SDLUI_Base
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    i32 window_width;
    i32 window_height;
    u8 mouse_current_frame[5] = {0};
    u8 mouse_last_frame[5] = {0};
    SDLUI_Theme theme;
}SDLUI_Base;


// Helpers ---------------------------------------------------------

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
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    SDLUI_Font.handle = TTF_OpenFont("liberation-mono.ttf", SDLUI_Font.size);
    SDLUI_Font.height = TTF_FontHeight(SDLUI_Font.handle);
    TTF_SizeText(SDLUI_Font.handle, "0", &SDLUI_Font.width, &SDLUI_Font.height);
    
    SDLUI_Base.renderer = r;
    SDLUI_Base.window = w;
    SDL_GetWindowSize(SDLUI_Base.window, &SDLUI_Base.window_width, &SDLUI_Base.window_height);
    
    SDLUI_Collection.create();
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

float SDLUI_Max(float a, float b)
{
    if(a > b)
    {
        return a;
    }
    
    return b;
}

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
        }
    }
    
    SDLUI_MouseStateReset();
}

// Create ---------------------------------------------------

SDLUI_Control_Button *SDLUI_CreateButton(i32 x, i32 y, char *text)
{
    SDLUI_Control_Button *btn = (SDLUI_Control_Button*)malloc(sizeof(SDLUI_Control_Button));
    
    btn->text.create(text);
    btn->type = SDLUI_CONTROL_TYPE_BUTTON;
    btn->x = x;
    btn->y = y;
    btn->w = 100;
    btn->h = 30;
    btn->visible = false;
    btn->align = SDLUI_ALIGN_CENTER;
    btn->state = SDLUI_BUTTON_STATE_NORMAL;
    
    SDL_Color c = {255, 255, 255, 255};
    SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, text, c);
    btn->tex_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
    SDL_FreeSurface(s);
    btn->tex_back_normal = SDL_CreateTexture(SDLUI_Base.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, btn->w - 2, btn->h - 2);
    btn->tex_back_hover = SDL_CreateTexture(SDLUI_Base.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, btn->w - 2, btn->h - 2);
    btn->tex_back_click = SDL_CreateTexture(SDLUI_Base.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, btn->w - 2, btn->h - 2);
    
    SDLUI_GradientToTexture(btn->tex_back_normal, SDLUI_Base.theme.col_base, btn->w-2, btn->h-2, (btn->h-2)/12);
    SDLUI_GradientToTexture(btn->tex_back_hover, SDLUI_Base.theme.col_highlight, btn->w-2, btn->h-2, (btn->h-2)/12);
    SDLUI_GradientToTexture(btn->tex_back_click, SDLUI_Base.theme.col_border, btn->w-2, btn->h-2, (btn->h-2)/12);
    
    SDLUI_Collection.push(btn);
    return btn;
}

SDLUI_Control_Text *SDLUI_CreateText(i32 x, i32 y, char *text)
{
    SDLUI_Control_Text *txt = (SDLUI_Control_Text*)malloc(sizeof(SDLUI_Control_Text));
    
    txt->text.create(text);
    txt->type = SDLUI_CONTROL_TYPE_TEXT;
    txt->x = x;
    txt->y = y;
    txt->w = txt->text.length * SDLUI_Font.width;
    txt->h = SDLUI_Font.height;
    txt->visible = false;
    
    SDL_Color c = {255, 255, 255, 255};
    SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,txt->text.data, c);
    txt->tex_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
    SDL_FreeSurface(s);
    
    SDLUI_Collection.push(txt);
    return txt;
}

// Usage
bool SDLUI_Button(SDLUI_Control_Button *btn)
{
    btn->visible = true;
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
