SDLUI_Control_Window *SDLUI_CreateWindow(i32 x, i32 y, i32 w, i32 h, char const *title)
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
	wnd->visible = true;
	wnd->visible_last_frame = false;
	wnd->enabled = true;
	wnd->enabled_last_frame = false;
	wnd->is_resized = false;
	wnd->do_process = false;
	wnd->has_close_button = true;
	wnd->can_be_resized = true;

	SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,wnd->title.data, SDLUI_Core.theme.col_white);
	wnd->tex_title = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
	SDL_FreeSurface(s);

	wnd->tex_rect = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	wnd->children.create();
	SDLUI_Window_Collection.push(wnd);

	return wnd;
}

SDLUI_Control_Button *SDLUI_CreateButton(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text)
{
	SDLUI_Control_Button *btn = (SDLUI_Control_Button*)malloc(sizeof(SDLUI_Control_Button));

	btn->type = SDLUI_CONTROL_TYPE_BUTTON;
	btn->do_process = false;
	btn->text.create(text);
	btn->x = wnd->x + x;
	btn->y = wnd->y + y;
	btn->w = 100;
	btn->h = 30;
	btn->visible = true;
	btn->enabled = true;
	btn->owned_by_window = true;
	btn->align = SDLUI_ALIGN_CENTER;
	btn->state = SDLUI_BUTTON_STATE_NORMAL;
	btn->parent = wnd;

	SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, text, SDLUI_Core.theme.col_white);
	btn->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
	SDL_FreeSurface(s);

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
	si->visible = true;
	si->enabled = true;
	si->owned_by_window = true;
	si->min = min;
	si->max = max;
	si->value = value;
	si->thumb_size = 12;
	si->is_changing = false;
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

SDLUI_Control_CheckBox *SDLUI_CreateCheckBox(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text, bool checked)
{
	SDLUI_Control_CheckBox *chk = (SDLUI_Control_CheckBox*)malloc(sizeof(SDLUI_Control_CheckBox));

	chk->type = SDLUI_CONTROL_TYPE_CHECKBOX;
	chk->do_process = false;
	chk->x = wnd->x + x;
	chk->y = wnd->y + y;
	chk->w = 16;
	chk->h = 16;
	chk->visible = true;
	chk->enabled = true;
	chk->owned_by_window = true;
	chk->checked = checked;
	chk->parent = wnd;

	if(strlen(text) > 0)
	{
		chk->text.create(text);
		SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, text, SDLUI_Core.theme.col_white);
		chk->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
		SDL_FreeSurface(s);
	}
	else
	{
		chk->tex_text = NULL;
	}

	wnd->children.push(chk);
	return chk;
}

SDLUI_Control_Text *SDLUI_CreateText(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text)
{
	SDLUI_Control_Text *txt = (SDLUI_Control_Text*)malloc(sizeof(SDLUI_Control_Text));

	txt->type = SDLUI_CONTROL_TYPE_TEXT;
	txt->do_process = false;
	txt->text.create(text);
	txt->x = wnd->x + x;
	txt->y = wnd->y + y;
	txt->w = (txt->text.length) * SDLUI_Font.width;
	txt->h = SDLUI_Font.height;
	txt->visible = true;
	txt->enabled = true;
	txt->owned_by_window = true;
	txt->parent = wnd;

	SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,txt->text.data, SDLUI_Core.theme.col_white);
	txt->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
	SDL_FreeSurface(s);

	wnd->children.push(txt);
	return txt;
}

SDLUI_Control_ToggleButton *SDLUI_CreateToggleButton(SDLUI_Control_Window *wnd, i32 x, i32 y, char const *text, bool checked)
{
	SDLUI_Control_ToggleButton *tb = (SDLUI_Control_ToggleButton*)malloc(sizeof(SDLUI_Control_ToggleButton));

	tb->type = SDLUI_CONTROL_TYPE_TOGGLE_BUTTON;
	tb->do_process = false;
	tb->x = wnd->x + x;
	tb->y = wnd->y + y;
	tb->w = 32;
	tb->h = 16;
	tb->visible = true;
	tb->enabled = true;
	tb->owned_by_window = true;
	tb->checked = checked;
	tb->parent = wnd;

	if(strlen(text) > 0)
	{
		tb->text.create(text);
		SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, text, SDLUI_Core.theme.col_white);
		tb->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
		SDL_FreeSurface(s);
	}
	else
	{
		tb->tex_text = NULL;
	}

	wnd->children.push(tb);

	return tb;
}

SDLUI_ArrayOfControls SDLUI_CreateRadioButtonGroup()
{
	SDLUI_ArrayOfControls rb_group;
	rb_group.create();
	return rb_group;
}

SDLUI_Control_RadioButton *SDLUI_CreateRadioButton(SDLUI_Control_Window *wnd, SDLUI_ArrayOfControls &group, i32 x, i32 y, char const *text, bool checked)
{
	SDLUI_Control_RadioButton *rb = (SDLUI_Control_RadioButton*)malloc(sizeof(SDLUI_Control_RadioButton));

	rb->type = SDLUI_CONTROL_TYPE_RADIO_BUTTON;
	rb->do_process = false;
	rb->x = wnd->x + x;
	rb->y = wnd->y + y;
	rb->w = 16;
	rb->h = 16;
	rb->visible = true;
	rb->enabled = true;
	rb->owned_by_window = true;
	rb->checked = checked;
	rb->group = &group;
	rb->group->push(rb);
	rb->parent = wnd;

	if(strlen(text) > 0)
	{
		rb->text.create(text);
		SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, text, SDLUI_Core.theme.col_white);
		rb->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
		SDL_FreeSurface(s);
	}
	else
	{
		rb->tex_text = NULL;
	}

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
	tbc->visible = true;
	tbc->enabled = true;
	tbc->owned_by_window = true;
	tbc->bar_height = 30;
	tbc->active_tab = NULL;
	tbc->parent = wnd;

	wnd->children.push(tbc);
	return tbc;

}

SDLUI_Control_ScrollArea *SDLUI_CreateScrollArea(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 w, i32 h, SDL_Texture *tex)
{
	SDLUI_Control_ScrollArea *sa = (SDLUI_Control_ScrollArea*)malloc(sizeof(SDLUI_Control_ScrollArea));

	sa->type = SDLUI_CONTROL_TYPE_SCROLL_AREA;
	sa->do_process = false;
	sa->x = wnd->x + x;
	sa->y = wnd->y + y;
	sa->w = w;
	sa->h = h;
	sa->visible = true;
	sa->enabled = true;
	sa->owned_by_window = true;
	sa->scrollbar_thickness = 12;
	sa->track_size_h = sa->w - sa->scrollbar_thickness;
	sa->track_size_v = sa->h - sa->scrollbar_thickness;
	sa->scroll_x = 0;
	sa->scroll_y = 0;
	sa->is_changing_v = false;
	sa->is_changing_h = false;
	sa->tex_rect = tex;
	if(SDL_QueryTexture(sa->tex_rect, NULL, NULL, &sa->content_width, &sa->content_height) == 0)
	{
		if(sa->content_width > sa->w)
		{
			sa->client_height = sa->h - sa->scrollbar_thickness;
		}
		if(sa->content_height > sa->h)
		{
			sa->client_width = sa->w - sa->scrollbar_thickness;
		}
	}
	else
	{
		sa->client_width = sa->w;
		sa->client_height = sa->h;
	}

	sa->parent = wnd;

	wnd->children.push(sa);
	return sa;
}

SDLUI_Control_List *SDLUI_CreateList(SDLUI_Control_Window *wnd, SDLUI_Control_ScrollArea *sa, i32 num_items)
{
	SDLUI_Control_List *lst = (SDLUI_Control_List*)malloc(sizeof(SDLUI_Control_List));

	lst->type = SDLUI_CONTROL_TYPE_LIST;
	lst->do_process = false;
	lst->scroll_area = sa;
	lst->num_items = num_items;
	lst->selected_index = 0;
	lst->max_string_width = 0;

	i32 h = num_items * SDLUI_Font.height;
	if(h < sa->h)
	{
		h = sa->h;
	}
	sa->tex_rect = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sa->w, h);
	sa->content_width = sa->w;
	sa->content_height = h;

	sa->client_height = sa->h - sa->scrollbar_thickness;

	if(SDL_QueryTexture(sa->tex_rect, NULL, NULL, &sa->content_width, &sa->content_height) == 0)
	{
		if(sa->content_width > sa->w)
		{
			sa->client_height = sa->h - sa->scrollbar_thickness;
		}
		if(sa->content_height > sa->h)
		{
			sa->client_width = sa->w - sa->scrollbar_thickness;
		}
	}
	else
	{
		sa->client_width = sa->w;
		sa->client_height = sa->h;
	}

	return lst;
}

SDLUI_Control_TextBox *SDLUI_CreateTextBox(SDLUI_Control_Window *wnd, i32 x, i32 y, i32 w)
{
	SDLUI_Control_TextBox *tbx = (SDLUI_Control_TextBox*)malloc(sizeof(SDLUI_Control_TextBox));

	tbx->do_process = false;

	tbx->type = SDLUI_CONTROL_TYPE_TEXTBOX;
	tbx->do_process = false;
	tbx->x = wnd->x + x;
	tbx->y = wnd->y + y;
	tbx->w = w;
	tbx->h = 30;
	tbx->visible = true;
	tbx->enabled = true;
	tbx->owned_by_window = true;
	tbx->text.create("");
	tbx->select_start = 0;
	tbx->select_end = 0;
	tbx->cursor_pos = 0;
	tbx->focused = false;
	tbx->scroll = 0;
	tbx->max_chars = (w - (2 * SDLUI_MARGIN)) / SDLUI_Font.width;

	i32 tw = w - (2 * SDLUI_MARGIN);
	i32 th = SDLUI_Font.height;
	tbx->tex_text = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tw, th);

	tbx->parent = wnd;
	wnd->children.push(tbx);

	return tbx;
}
