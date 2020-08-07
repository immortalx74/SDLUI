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
    
    SDLUI_Window_Collection.create();
    
    SDLUI_Base.cursor_arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    SDLUI_Base.cursor_size_we = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    SDLUI_Base.cursor_size_ns = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    SDL_SetCursor(SDLUI_Base.cursor_arrow);
    
    SDL_Surface *s = IMG_Load("res/tick.png");
    SDLUI_Base.tex_tick = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
    
    s = IMG_Load("res/circle.png");
    SDLUI_Base.tex_circle = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
    
    s = IMG_Load("res/circle_fill_1.png");
    SDLUI_Base.tex_circle_fill_1 = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
    
    s = IMG_Load("res/circle_fill_2.png");
    SDLUI_Base.tex_circle_fill_2 = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
    
    s = IMG_Load("res/toggle.png");
    SDLUI_Base.tex_toggle = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
    
    s = IMG_Load("res/close.png");
    SDLUI_Base.tex_close = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
    
    SDL_FreeSurface(s);
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

void SDLUI_Colorize(SDL_Texture *t, SDL_Color c)
{
    SDL_SetTextureColorMod(t, c.r, c.g, c.b);
}

void SDLUI_SetActiveWindow(SDLUI_Control_Window *wnd)
{
    SDLUI_Window_Collection.to_back(wnd);
    SDLUI_Base.active_window = wnd;
}

//SDLUI_RESIZE_DIRECTION SDLUI_WindowResizeDirection()
//{
//
//}

void SDLUI_WindowHandler()
{
    i32 mx, my, index = 0;
    SDL_GetMouseState(&mx, &my);
    SDLUI_Control_Window *aw = (SDLUI_Control_Window*)SDLUI_Base.active_window;
    SDL_Rect left, top, right, bottom;
    left = {aw->x-8, aw->y, 8, aw->h};
    top = {aw->x, aw->y-8, aw->w, 8};
    right = {aw->x+aw->w, aw->y, 8, aw->h};
    bottom = {aw->x, aw->y+aw->h, aw->w, 8};
    
    if(SDLUI_PointCollision(left, mx, my))
    {
        std::cout << "left" << std::endl;
    }
    if(SDLUI_PointCollision(top, mx, my))
    {
        std::cout << "top" << std::endl;
    }
    if(SDLUI_PointCollision(right, mx, my))
    {
        SDL_SetCursor(SDLUI_Base.cursor_size_we);
        
        if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD)
        {
            aw->is_resized = true;
        }
    }
    else if(!aw->is_resized)
    {
        SDL_SetCursor(SDLUI_Base.cursor_arrow);
    }
    if(SDLUI_PointCollision(bottom, mx, my))
    {
        std::cout << "bot" << std::endl;
    }
    
    
    if(aw->is_resized)
    {
        aw->w = mx - aw->x;
    }
    if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED && aw->is_resized)
    {
        aw->is_resized = false;
        SDL_SetCursor(SDLUI_Base.cursor_arrow);
    }
    
    
    
    if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
    {
        SDLUI_Control_Window *wnd;
        SDL_Rect r;
        bool over_window = false;
        
        for (int i = 0; i < SDLUI_Window_Collection.size; ++i)
        {
            wnd = (SDLUI_Control_Window*)SDLUI_Window_Collection.data[i];
            
            r = {wnd->x, wnd->y, wnd->w, wnd->h};
            
            if(SDLUI_PointCollision(r, mx, my))
            {
                index = i;
                over_window = true;
            }
        }
        
        if(SDLUI_Window_Collection.data[index] != SDLUI_Base.active_window && over_window)
        {
            SDLUI_SetActiveWindow((SDLUI_Control_Window*)SDLUI_Window_Collection.data[index]);
        }
    }
}