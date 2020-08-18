// Structs --------------------------------------------

struct SDLUI_String
{
    i32 capacity = 0;
    i32 length = 0;
    bool modified = false;
    char *data;
    
    void create(char *str)
    {
        length = strlen(str);
        capacity = ((length / SDLUI_STRING_CAPACITY) + 1) * SDLUI_STRING_CAPACITY;
        data = (char*)malloc(capacity);
        memcpy(data, str, length);
        memset(data + length, 0, 1);
        
        modified = false;
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
            memset(data + new_length, 0, 1);
            length = new_length;
        }
        else
        {
            memcpy(data, str, new_length);
            memset(data + new_length, 0, capacity - new_length);
            length = new_length;
        }
        
        modified = true;
    }
};

struct SDLUI_Theme
{
    SDL_Color col_base = {55, 55, 55, 255};
    SDL_Color col_border = {38, 38, 38, 255};
    SDL_Color col_active_window_bar = {10, 10, 10, 255};
    SDL_Color col_inactive_window_bar = {40, 40, 40, 255};
    SDL_Color col_hover = {80, 80, 80, 255};
    SDL_Color col_click = {40, 40, 40, 255};
    SDL_Color col_highlight = {65, 105, 225, 255};
    SDL_Color col_test = {255, 0, 225, 255};
    SDL_Color col_thumb = {66, 66, 66, 255};
    SDL_Color col_white = {255, 255, 255, 255};
    SDL_Color col_grey = {127, 127, 127, 255};
    SDL_Color col_red = {222, 17, 35, 255};
};

struct SDLUI_Control
{
    SDLUI_CONTROL_TYPE type;
    i32 x;
    i32 y;
    i32 w;
    i32 h;
    bool visible = true;
    bool enabled = true;
    bool owned_by_window = true;
    bool do_process = false;
    SDLUI_Control *parent;
};

struct __SDLUI_Font
{
    TTF_Font *handle;
    i32 size = 13;
    i32 width;
    i32 height;
}SDLUI_Font;

struct SDLUI_Array
{
    i32 capacity;
    i32 size;
    SDLUI_Control **data;
    
    void create()
    {
        capacity = SDLUI_COLLECTION_CHUNK;
        data = (SDLUI_Control**)malloc(capacity * sizeof(SDLUI_Control*));
        size = 0;
    }
    
    void ensure_capacity()
    {
        if(size >= capacity)
        {
            capacity += SDLUI_COLLECTION_CHUNK;
            data = (SDLUI_Control**)realloc(data, capacity * sizeof(SDLUI_Control*));
        }
    }
    
    void push(SDLUI_Control *elem)
    {
        ensure_capacity();
        data[size] = elem;
        size++;
    }
    
    void pop(SDLUI_Control *elem)
    {
        for (int i = 0; i < size; ++i)
        {
            if((i < size - 1) && elem == data[i])
            {
                i32 num_elements = size - i - 1;
                memmove(data + i, data + i + 1, num_elements * sizeof(data));
                size--;
                return;
            }
        }
    }
    
    void to_back(SDLUI_Control *elem)
    {
        for (int i = 0; i < size; ++i)
        {
            if((i < size - 1) && elem == data[i])
            {
                i32 num_elements = size - i - 1;
                memmove(data + i, data + i + 1, num_elements * sizeof(data));
                data[size - 1] = elem;
                return;
            }
        }
    }
};

SDLUI_Array SDLUI_Window_Collection;

struct SDLUI_Control_Window : SDLUI_Control
{
    i32 drag_x;
    i32 drag_y;
    bool is_dragged = false;
    bool is_resized = false;
    bool is_hovered = false;
    SDLUI_RESIZE_DIRECTION resize_direction;
    bool visible_last_frame = false;
    bool enabled_last_frame = false;
    SDLUI_String title;
    SDL_Texture *tex_title;
    SDL_Texture *tex_rect;
    bool active;
    SDLUI_Array children;
};

struct SDLUI_Control_Button : SDLUI_Control
{
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
    i32 min;
    i32 max;
    i32 value;
    SDLUI_ORIENTATION orientation;
    i32 thumb_size;
    bool ischanging;
};

struct SDLUI_Control_CheckBox : SDLUI_Control
{
    bool checked;
};

struct SDLUI_Control_ToggleButton : SDLUI_Control
{
    bool checked;
};

struct SDLUI_Control_RadioButton : SDLUI_Control
{
    SDLUI_Array *group;
    bool checked;
    bool checked_changed;
};

struct __SDLUI_Base
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    i32 window_width;
    i32 window_height;
    u8 mouse_current_frame[5] = {0};
    u8 mouse_last_frame[5] = {0};
    SDLUI_Theme theme;
    SDLUI_Control_Window *active_window;
    
    SDL_Event e;
    
    SDL_Cursor *cursor_arrow;
    SDL_Cursor *cursor_size_we;
    SDL_Cursor *cursor_size_ns;
    
    SDL_Cursor *cursor_size_nwse;
    SDL_Cursor *cursor_size_nesw;
    
    SDL_Texture *tex_tick;
    SDL_Texture *tex_circle;
    SDL_Texture *tex_circle_fill_1;
    SDL_Texture *tex_circle_fill_2;
    SDL_Texture *tex_toggle;
    SDL_Texture *tex_close;
}SDLUI_Base;

struct SDLUI_Control_Tab : SDLUI_Control
{
    i32 index;
    SDLUI_String text;
    SDL_Texture *tex_text;
    SDLUI_Array children;
};

struct SDLUI_Control_TabContainer : SDLUI_Control
{
    i32 bar_height;
    SDLUI_Array tabs;
    SDLUI_Control_Tab *active_tab;
    SDLUI_ORIENTATION orientation;
    
    void add_tab(char *text)
    {
        SDLUI_Control_Tab *tab = (SDLUI_Control_Tab*)malloc(sizeof(SDLUI_Control_Tab));
        
        tab->type = SDLUI_CONTROL_TYPE_TAB;
        tab->text.create(text);
        tab->w = (tab->text.length) * SDLUI_Font.width;
        tab->h = SDLUI_Font.height;
        tab->children.create();
        tab->index = tabs.size;
        
        SDL_Color c = {255, 255, 255, 255};
        SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,tab->text.data, c);
        tab->tex_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
        SDL_FreeSurface(s);
        
        this->tabs.push(tab);
        this->active_tab = tab;
        
    }
    
    void add_child(i32 tab_index, SDLUI_Control *ctrl)
    {
        SDLUI_Control_Window *wnd = (SDLUI_Control_Window*)this->parent;
        SDLUI_Control_Tab *tab;
        SDLUI_Control *cur_ctrl;
        
        for (int i = 0; i < wnd->children.size; ++i)
        {
            cur_ctrl = wnd->children.data[i];
            
            if(cur_ctrl == ctrl)
            {
                for (int j = 0; j < this->tabs.size; ++j)
                {
                    tab = (SDLUI_Control_Tab*)this->tabs.data[j];
                    
                    if(tab->index == tab_index)
                    {
                        ctrl->visible = false;
                        tab->children.push(ctrl);
                        ctrl->owned_by_window = false;
                        //wnd->children.pop(ctrl);
                    }
                }
            }
        }
    }
};

struct SDLUI_Control_Label : SDLUI_Control
{
    SDL_Texture *tex_text;
};

struct SDLUI_Control_Text : SDLUI_Control
{
    SDLUI_String text;
    bool modified;
    SDL_Texture *tex_text;
};