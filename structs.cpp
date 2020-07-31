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
    bool visible = true;
    bool enabled = true;
    SDLUI_Control **children;
    SDLUI_Control *parent;
    i32 num_children = 0;
    
    void parent_to(SDLUI_Control *dst)
    {
        if(dst->num_children == 0)
        {
            dst->children = (SDLUI_Control**)malloc(1 * sizeof(SDLUI_Control*));
            dst->children[0] = this;
            dst->num_children++;
            this->parent = dst;
        }
        else
        {
            dst->num_children++;
            dst->children = (SDLUI_Control**)realloc(dst->children, dst->num_children * sizeof(SDLUI_Control*));
            dst->children[dst->num_children - 1] = this;
            this->parent = dst;
        }
    }
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

struct SDLUI_Control_Window : SDLUI_Control
{
    i32 w;
    i32 h;
    i32 drag_x;
    i32 drag_y;
    bool is_dragged = false;
    bool visible_last_frame = false;
    bool enabled_last_frame = false;
    SDLUI_String title;
    SDL_Texture *tex_title;
};

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

struct SDLUI_Control_RadioButton : SDLUI_Control
{
    i32 w;
    i32 h;
    i32 group;
    bool checked;
    bool checked_changed;
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
    
    SDL_Texture *tex_tick;
    SDL_Texture *tex_circle;
    SDL_Texture *tex_circle_fill_1;
    SDL_Texture *tex_circle_fill_2;
    SDL_Texture *tex_toggle;
}SDLUI_Base;
