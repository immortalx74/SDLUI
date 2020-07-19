#pragma once

#include <iostream>
#include "SDL_ttf.h"
#include "SDL_image.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define SDLUI_MAX_CONTROLS 50
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
struct SDLUI_Control;

struct SDLUI_String
{
    i32 capacity = 0;
    i32 length = 0;
    bool changed = false;
    char *data;
    
    void create(char *str)
    {
        length = strlen(str);
        capacity = ((length / SDLUI_STRING_CAPACITY) + 1) * SDLUI_STRING_CAPACITY;
        data = (char*)calloc(1, capacity);
        memcpy(data, str, length);
    }
    
    void destroy()
    {
        free(data);
        data = NULL;
    }
    
    void modify(char *str)
    {
        i32 new_length = strlen(str);
        if(new_length >= capacity)
        {
            capacity = ((new_length / SDLUI_STRING_CAPACITY) + 1) * SDLUI_STRING_CAPACITY;
            data = (char*)realloc(data, capacity);
            memcpy(data, str, new_length);
            memset(data + new_length, 0, capacity - new_length);
            length = new_length;
        }
        else
        {
            memcpy(data, str, new_length);
            memset(data + new_length, 0, capacity - new_length);
            length = new_length;
        }
        
        changed = true;
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

struct SDLUI_Control_Button
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    char *caption;
    bool visible;
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
    bool visible;
    bool ischanging;
};

struct SDLUI_Control_CheckBox
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    bool visible;
    bool checked;
};

struct SDLUI_Control_ToggleButton
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    bool visible;
    bool checked;
};

struct SDLUI_Control_Tab
{
    i32 width;
    i32 index;
    SDLUI_String string;
    SDL_Texture *t_text;
};

struct SDLUI_Control_TabContainer
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    i32 bar_height;
    i32 num_tabs;
    i32 active_tab;
    bool visible;
    SDLUI_Control_Tab *tabs;
    i32 num_children;
    SDLUI_Control **children;
};

struct SDLUI_Control_Label
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    bool visible;
    SDL_Texture *t_text;
};

struct SDLUI_Control_Text
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    SDLUI_String string;
    bool modified;
    bool visible;
    SDL_Texture *t_text;
};

struct SDLUI_Control_RadioButton
{
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    i32 group;
    bool visible;
    bool checked;
    SDL_Texture *t_img;
};

struct __SDLUI_RadioButtonGroups
{
    i32 num_elements = 0;
    i32 *groups;
    SDLUI_Control_RadioButton *collection;
}SDLUI_RadioButtonGroups;

struct SDLUI_Control
{
    union
    {
        SDLUI_Control_Button button;
        SDLUI_Control_SliderInt slider_int;
        SDLUI_Control_CheckBox checkbox;
        SDLUI_Control_ToggleButton toggle_button;
        SDLUI_Control_TabContainer tab_container;
        SDLUI_Control_Label label;
        SDLUI_Control_Text text;
        SDLUI_Control_RadioButton radio_button;
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

//void draw_point(i32 offsetx, i32 offsety, i32 x, i32 y)
//{
//SDL_RenderDrawPoint(SDLUI_Base.renderer, offsetx - x, offsety + y);
//SDL_RenderDrawPoint(SDLUI_Base.renderer, offsetx + x, offsety + y);
//SDL_RenderDrawPoint(SDLUI_Base.renderer, offsetx - x, offsety - y);
//SDL_RenderDrawPoint(SDLUI_Base.renderer, offsetx + x, offsety + y);
//}
//
//void draw_circle(i32 posx, i32 posy, i32 radius)
//{
//i32 i = 0;
//i32 j = radius;
//i32 d = 0;
//i32 T = 0;
//
//SDL_SetRenderDrawColor(SDLUI_Base.renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
//draw_point(posx, posy, i, j);
//
//while(i < j + 1)
//{
//i += 1;
//double s = sqrt(max(radius * radius - i * i, 0.0f));
//d = floor(SDL_ALPHA_OPAQUE * ceil(s) - s) + 0.5f;
//
//if(d < T)
//{
//j -= 1;
//}
//
//T = d;
//i32 alpha;
//
//if(d > 0)
//{
//alpha = d;
//SDL_SetRenderDrawColor(SDLUI_Base.renderer, 255, 255, 255, alpha);
//draw_point(posx, posy, i, j);
//
//if(i != j)
//{
//draw_point(posx, posy, j, i);
//}
//}
//
//if(SDL_ALPHA_OPAQUE - d > 0)
//{
//alpha = SDL_ALPHA_OPAQUE - d;
//SDL_SetRenderDrawColor(SDLUI_Base.renderer, 255, 255, 255, alpha);
//draw_point(posx, posy, i, j + 1);
//
//if(i != j + 1)
//{
//draw_point(posx, posy, j + 1, i);
//}
//}
//}
//}

// Render ---------------------------------------------------
void SDLUI_Render_Button(SDLUI_Control_Button *btn)
{
    if(btn->visible)
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
            SDL_QueryTexture(tbc->tabs[i].t_text, NULL, NULL, &tex_w, &tex_h);
            SDL_Rect dst = {r.x + SDLUI_MARGIN, r.y + SDLUI_MARGIN, tex_w, tex_h};
            SDL_RenderCopy(SDLUI_Base.renderer, tbc->tabs[i].t_text, NULL, &dst);
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
        SDL_RenderCopy(SDLUI_Base.renderer, lbl->t_text, NULL, &r);
    }
}

void SDLUI_Render_Text(SDLUI_Control_Text *txt)
{
    if(txt->visible)
    {
        SDLUI_SetColor(SDLUI_Base.theme.col_white);
        SDL_Rect r = {txt->x, txt->y, txt->w, txt->h};
        SDL_RenderCopy(SDLUI_Base.renderer, txt->t_text, NULL, &r);
    }
}

void SDLUI_Render_RadioButton(SDLUI_Control_RadioButton *rdb)
{
    if(rdb->visible)
    {
        SDLUI_SetColor(SDLUI_Base.theme.col_white);
        SDL_Rect src = {rdb->checked * 48, 0, 48, 48};
        SDL_Rect r = {rdb->x, rdb->y, rdb->w, rdb->h};
        SDL_RenderCopy(SDLUI_Base.renderer, rdb->t_img, &src, &r);
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
            
            case SDLUI_CONTROL_TYPE_TOGGLE_BUTTON:
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
            
            case SDLUI_CONTROL_TYPE_LABEL:
            {
                SDLUI_Control_Label *lbl = &SDLUI_Base.pool[i].label;
                SDLUI_Render_Label(lbl);
            }
            break;
            
            case SDLUI_CONTROL_TYPE_TEXT:
            {
                SDLUI_Control_Text *txt = &SDLUI_Base.pool[i].text;
                SDLUI_Render_Text(txt);
            }
            break;
            
            case SDLUI_CONTROL_TYPE_RADIO_BUTTON:
            {
                SDLUI_Control_RadioButton *rdb = &SDLUI_Base.pool[i].radio_button;
                SDLUI_Render_RadioButton(rdb);
            }
            break;
        }
    }
    
    SDLUI_MouseStateReset();
}

void SDLUI_Destroy_Control(void  *ctrl)
{
    i32 offset = (u8*)ctrl - (u8*)SDLUI_Base.pool;
    i32 index = offset / sizeof(SDLUI_Control);
    
    std::cout << offset << std::endl;
}

// Create ---------------------------------------------------

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
        ctrl->button.visible = false;
        ctrl->button.align = SDLUI_ALIGN_CENTER;
        ctrl->button.state = SDLUI_BUTTON_STATE_NORMAL;
        
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Base.font.handle,caption, c);
        ctrl->button.t_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        SDL_FreeSurface(s);
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
        //ctrl->slider_int.visible = false;
        ctrl->slider_int.visible = true;
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
        ctrl->checkbox.visible = false;
        ctrl->checkbox.checked = false;
        
        return &ctrl->checkbox;
    }
    
    return NULL;
}

SDLUI_Control_ToggleButton *SDLUI_CreateToggleButton(i32 x, i32 y, bool checked)
{
    if(SDLUI_Base.count < SDLUI_MAX_CONTROLS)
    {
        SDLUI_Control *ctrl = &SDLUI_Base.pool[SDLUI_Base.count];
        SDLUI_Base.count++;
        
        ctrl->type = SDLUI_CONTROL_TYPE_TOGGLE_BUTTON;
        ctrl->toggle_button.x = x;
        ctrl->toggle_button.y = y;
        ctrl->toggle_button.w = 40;
        ctrl->toggle_button.h = 20;
        ctrl->toggle_button.visible = false;
        ctrl->toggle_button.checked = false;
        
        return &ctrl->toggle_button;
    }
    
    return NULL;
}

SDLUI_Control_TabContainer *SDLUI_CreateTabContainer(i32 x, i32 y, i32 w, i32 h)
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
        ctrl->tab_container.bar_height = 30;
        ctrl->tab_container.num_tabs = 0;
        ctrl->tab_container.num_children = 0;
        ctrl->tab_container.active_tab = 0;
        ctrl->tab_container.visible = false;
        
        return &ctrl->tab_container;
    }
    
    return NULL;
}

void SDLUI_CreateTab(SDLUI_Control_TabContainer *tbc, char *caption)
{
    if(tbc->num_tabs == 0)
    {
        tbc->tabs = (SDLUI_Control_Tab*)malloc(1 * sizeof(SDLUI_Control_Tab));
        tbc->tabs[0].string.create(caption);
        tbc->tabs[0].index = 0;
        tbc->tabs[0].width = (tbc->tabs[0].string.length * SDLUI_Base.font.width) + (2 * SDLUI_MARGIN);
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Base.font.handle,caption, c);
        tbc->tabs[0].t_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        SDL_FreeSurface(s);
        
        tbc->num_tabs++;
    }
    else
    {
        tbc->num_tabs++;
        tbc->tabs = (SDLUI_Control_Tab*)realloc(tbc->tabs, tbc->num_tabs * sizeof(SDLUI_Control_Tab));
        tbc->tabs[tbc->num_tabs - 1].string.create(caption);
        tbc->tabs[tbc->num_tabs - 1].index = tbc->num_tabs - 1;
        tbc->tabs[tbc->num_tabs - 1].width = (tbc->tabs[tbc->num_tabs - 1].string.length * SDLUI_Base.font.width) + (2 * SDLUI_MARGIN);
        
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Base.font.handle,caption, c);
        tbc->tabs[tbc->num_tabs - 1].t_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        SDL_FreeSurface(s);
    }
    
}

void SDLUI_Add_TabContainer_Child(SDLUI_Control_TabContainer *tbc, void *ctrl)
{
    if(tbc->num_children == 0)
    {
        tbc->children = (SDLUI_Control**)malloc(1 * sizeof(SDLUI_Control*));
        
        SDLUI_CONTROL_TYPE type = ((SDLUI_Control*)ctrl)->type;
        //tbc->children[0] = ctrl;
    }
    else
    {
        
    }
}

SDLUI_Control_Label *SDLUI_CreateLabel(i32 x, i32 y, char *caption)
{
    if(SDLUI_Base.count < SDLUI_MAX_CONTROLS)
    {
        SDLUI_Control *ctrl = &SDLUI_Base.pool[SDLUI_Base.count];
        SDLUI_Base.count++;
        
        ctrl->type = SDLUI_CONTROL_TYPE_LABEL;
        ctrl->label.visible = false;
        ctrl->label.x = x;
        ctrl->label.y = y;
        i32 len = strlen(caption);
        ctrl->label.w = len * SDLUI_Base.font.width;
        ctrl->label.h = SDLUI_Base.font.height;
        
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Base.font.handle,caption, c);
        ctrl->label.t_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        SDL_FreeSurface(s);
        
        return &ctrl->label;
    }
    
    return NULL;
}

SDLUI_Control_Text *SDLUI_CreateText(i32 x, i32 y, char *caption)
{
    if(SDLUI_Base.count < SDLUI_MAX_CONTROLS)
    {
        SDLUI_Control *ctrl = &SDLUI_Base.pool[SDLUI_Base.count];
        SDLUI_Base.count++;
        
        ctrl->type = SDLUI_CONTROL_TYPE_TEXT;
        ctrl->text.visible = false;
        ctrl->text.x = x;
        ctrl->text.y = y;
        i32 len = strlen(caption);
        
        SDLUI_String str;
        str.create(caption);
        ctrl->text.string = str;
        
        ctrl->text.w = len * SDLUI_Base.font.width;
        ctrl->text.h = SDLUI_Base.font.height;
        
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Base.font.handle,ctrl->text.string.data, c);
        ctrl->text.t_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        SDL_FreeSurface(s);
        
        return &ctrl->text;
    }
    
    return NULL;
}

SDLUI_Control_RadioButton *SDLUI_CreateRadioButton(i32 x, i32 y, i32 group)
{
    if(SDLUI_Base.count < SDLUI_MAX_CONTROLS)
    {
        SDLUI_Control *ctrl = &SDLUI_Base.pool[SDLUI_Base.count];
        SDLUI_Base.count++;
        
        ctrl->type = SDLUI_CONTROL_TYPE_RADIO_BUTTON;
        ctrl->radio_button.x = x;
        ctrl->radio_button.y = y;
        ctrl->radio_button.w = 20;
        ctrl->radio_button.h = 20;
        ctrl->radio_button.checked = false;
        ctrl->radio_button.group = group;
        
        SDL_Surface *s = IMG_Load("radio.png");
        
        ctrl->radio_button.t_img = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        SDL_FreeSurface(s);
        
        return &ctrl->radio_button;
    }
    
    return NULL;
}

// Usage -----------------------------------------------------
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

bool SDLUI_SliderInt(SDLUI_Control_SliderInt *si)
{
    //si->visible = true;
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
    chk->visible = true;
    
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
    tb->visible = true;
    
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

bool SDLUI_TabContainer(SDLUI_Control_TabContainer *tbc)
{
    if(tbc->num_tabs == 0)
    {
        return false;
    }
    
    tbc->visible = true;
    
    i32 mx, my;
    SDL_GetMouseState(&mx, &my);
    
    if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
    {
        SDL_Rect r;
        i32 tab_width = 0;
        i32 offset = 0;
        
        for (int i = 0; i < tbc->num_tabs; ++i)
        {
            tab_width = tbc->tabs[i].width;
            r = {tbc->x + offset, tbc->y, tab_width, tbc->bar_height};
            
            if(SDLUI_PointCollision(r, mx, my))
            {
                tbc->active_tab = i;
                return true;
            }
            
            offset += tab_width;
        }
    }
    
    return false;
}

bool SDLUI_Label(SDLUI_Control_Label *lbl)
{
    lbl->visible = true;
    return true;
}

bool SDLUI_Text(SDLUI_Control_Text *txt)
{
    txt->visible = true;
    
    if(txt->string.changed)
    {
        i32 len = strlen(txt->string.data);
        txt->w = len * SDLUI_Base.font.width;
        
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Base.font.handle,txt->string.data, c);
        txt->t_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        SDL_FreeSurface(s);
        
        txt->string.changed = false;
    }
    
    return true;
}

bool SDLUI_RadioButton(SDLUI_Control_RadioButton *rdb)
{
    rdb->visible = true;
    
    //i32 mx, my;
    //SDL_GetMouseState(&mx, &my);
    //
    //SDL_Rect r = {rdb->x,rdb->y,rdb->w,rdb->h};
    //if(SDLUI_PointCollision(r, mx, my))
    //{
    //if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
    //{
    //rdb->checked = !rdb->checked;
    //return true;
    //}
    //}
    
    return false;
}