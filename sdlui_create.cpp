// Create ---------------------------------------------------

SDLUI_Control_Window *SDLUI_CreateWindow(i32 x, i32 y, i32 w, i32 h, char *title)
{
	SDLUI_Control_Window *wnd = (SDLUI_Control_Window*)malloc(sizeof(SDLUI_Control_Window));
	
	wnd->type = SDLUI_CONTROL_TYPE_WINDOW;
	wnd->title.create(title);
	wnd->x = x;
	wnd->y = y;
	wnd->drag_x = 0;
	wnd->drag_y = 0;
	wnd->w = w;
	wnd->h = h;
	wnd->is_resized = false;
	
	SDL_Color c = {255, 255, 255, 255};
	SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,wnd->title.data, c);
	wnd->tex_title = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
	SDL_FreeSurface(s);
	
	wnd->tex_rect = SDL_CreateTexture(SDLUI_Base.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	
	wnd->children.create();
	SDLUI_Window_Collection.push(wnd);
	SDLUI_SetActiveWindow(wnd);
	return wnd;
}

SDLUI_Control_Button *SDLUI_CreateButton(SDLUI_Control_Window *wnd, i32 x, i32 y, char *text)
{
	SDLUI_Control_Button *btn = (SDLUI_Control_Button*)malloc(sizeof(SDLUI_Control_Button));
	
	btn->type = SDLUI_CONTROL_TYPE_BUTTON;
	btn->do_process = false;
	btn->text.create(text);
	btn->x = wnd->x + x;
	btn->y = wnd->y + y;
	btn->w = 100;
	btn->h = 30;
	btn->align = SDLUI_ALIGN_CENTER;
	btn->state = SDLUI_BUTTON_STATE_NORMAL;
	btn->parent = wnd;
	
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
	
	wnd->children.push(btn);
	return btn;
}

SDLUI_Control_SliderInt *SDLUI_CreateSliderInt(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 min, i32 max, i32 value, SDLUI_ORIENTATION orientation = SDLUI_ORIENTATION_HORIZONTAL)
{
	SDLUI_Control_SliderInt *si = (SDLUI_Control_SliderInt*)malloc(sizeof(SDLUI_Control_SliderInt));
	
	si->type = SDLUI_CONTROL_TYPE_SLIDER_INT;
	si->do_process = false;
	si->x = wnd->x + x;
	si->y = wnd->y + y;
	si->min = min;
	si->max = max;
	si->value = value;
	si->thumb_size = 12;
	si->ischanging = false;
	si->orientation = orientation;
	si->parent = wnd;
	
	if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
	{
		si->w = 100;
		si->h = 16;
	}
	else
	{
		si->w = 16;
		si->h = 100;
	}
	
	wnd->children.push(si);
	return si;
}

SDLUI_Control_CheckBox *SDLUI_CreateCheckBox(SDLUI_Control_Window *wnd, i32 x, i32 y, bool checked)
{
	SDLUI_Control_CheckBox *chk = (SDLUI_Control_CheckBox*)malloc(sizeof(SDLUI_Control_CheckBox));
	
	chk->type = SDLUI_CONTROL_TYPE_CHECKBOX;
	chk->do_process = false;
	chk->x = wnd->x + x;
	chk->y = wnd->y + y;
	chk->w = 16;
	chk->h = 16;
	chk->checked = checked;
	chk->parent = wnd;
	
	wnd->children.push(chk);
	return chk;
}

SDLUI_Control_Text *SDLUI_CreateText(SDLUI_Control_Window *wnd, i32 x, i32 y, char *text)
{
	SDLUI_Control_Text *txt = (SDLUI_Control_Text*)malloc(sizeof(SDLUI_Control_Text));
	
	txt->type = SDLUI_CONTROL_TYPE_TEXT;
	txt->do_process = false;
	txt->text.create(text);
	txt->x = wnd->x + x;
	txt->y = wnd->y + y;
	txt->w = (txt->text.length) * SDLUI_Font.width;
	txt->h = SDLUI_Font.height;
	txt->parent = wnd;
	
	SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,txt->text.data, SDLUI_Base.theme.col_white);
	txt->tex_text = SDL_CreateTextureFromSurface(SDLUI_Base.renderer, s);
	SDL_FreeSurface(s);
	
	wnd->children.push(txt);
	return txt;
}

SDLUI_Control_ToggleButton *SDLUI_CreateToggleButton(SDLUI_Control_Window *wnd, i32 x, i32 y, bool checked)
{
	SDLUI_Control_ToggleButton *tb = (SDLUI_Control_ToggleButton*)malloc(sizeof(SDLUI_Control_ToggleButton));
	
	tb->type = SDLUI_CONTROL_TYPE_TOGGLE_BUTTON;
	tb->do_process = false;
	tb->x = wnd->x + x;
	tb->y = wnd->y + y;
	tb->w = 32;
	tb->h = 16;
	tb->checked = checked;
	tb->parent = wnd;
	
	wnd->children.push(tb);
	
	return tb;
}

SDLUI_Array SDLUI_Create_RadioButtonGroup()
{
	SDLUI_Array rb_group;
	rb_group.create();
	return rb_group;
}

SDLUI_Control_RadioButton *SDLUI_CreateRadioButton(SDLUI_Control_Window *wnd, SDLUI_Array &group, i32 x, i32 y, bool checked)
{
	SDLUI_Control_RadioButton *rb = (SDLUI_Control_RadioButton*)malloc(sizeof(SDLUI_Control_RadioButton));
	
	rb->type = SDLUI_CONTROL_TYPE_RADIO_BUTTON;
	rb->do_process = false;
	rb->x = wnd->x + x;
	rb->y = wnd->y + y;
	rb->w = 16;
	rb->h = 16;
	rb->checked = checked;
	rb->group = &group;
	rb->group->push(rb);
	rb->parent = wnd;
	
	wnd->children.push(rb);
	
	return rb;
}

SDLUI_Control_TabContainer *SDLUI_CreateTabContainer(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 w, i32 h)
{
	SDLUI_Control_TabContainer *tbc = (SDLUI_Control_TabContainer*)malloc(sizeof(SDLUI_Control_TabContainer));
	
	tbc->type = SDLUI_CONTROL_TYPE_TAB_CONTAINER;
	tbc->do_process = false;
	tbc->tabs.create();
	tbc->x = wnd->x + x;
	tbc->y = wnd->y + y;
	tbc->w = w;
	tbc->h = h;
	tbc->bar_height = 30;
	tbc->active_tab = NULL;
	tbc->parent = wnd;
	
	wnd->children.push(tbc);
	return tbc;
	
}

SDLUI_Control_ScrollArea *SDLUI_CreateScrollArea(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 w, i32 h)
{
	SDLUI_Control_ScrollArea *sa = (SDLUI_Control_ScrollArea*)malloc(sizeof(SDLUI_Control_ScrollArea));

	sa->type = SDLUI_CONTROL_TYPE_SCROLL_AREA;
	sa->do_process = false;
	sa->x = wnd->x + x;
	sa->y = wnd->y + y;
	sa->w = w;
	sa->h = h;
	sa->content_width = w*2;
	sa->content_height = h*2;
	sa->scrollbar_thickness = 12;
	sa->scrollbar_length_h = sa->w - sa->scrollbar_thickness;
	sa->scrollbar_length_v = sa->h - sa->scrollbar_thickness;
	sa->thumb_size_h = w;
	sa->thumb_size_v = h;
	sa->scroll_x = 0;
	sa->scroll_y = 0;
	sa->parent = wnd;

	wnd->children.push(sa);
	return sa;
}