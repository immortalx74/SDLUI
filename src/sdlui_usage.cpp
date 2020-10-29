bool SDLUI_Window(SDLUI_Control_Window *wnd)
{
	if(!wnd->visible)
	{
		return false;
	}

	wnd->do_process = true;

	if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED || SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_NONE)
	{
		wnd->is_dragged = false;
	}

	if(wnd == SDLUI_Core.active_window)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_Rect r = {wnd->x, wnd->y, wnd->w, wnd->h};

		if(SDLUI_PointInRect(r, mx, my))
		{
			wnd->is_hovered = true;
		}
		else
		{
			wnd->is_hovered = false;
		}

		if(wnd->has_close_button)
		{
			r = {wnd->x + wnd->w - 30, wnd->y, 30, 30};

			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED && SDLUI_PointInRect(r, mx, my))
			{
				wnd->visible = false;
				SDLUI_Core.active_window = NULL;
				SDL_SetCursor(SDLUI_Core.cursor_arrow);
			}
		}

		r = {wnd->x,wnd->y,wnd->w - (wnd->has_close_button * 30), 30};

		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED && !wnd->is_dragged)
			{
				wnd->is_dragged = true;
				wnd->drag_x = mx - wnd->x;
				wnd->drag_y = my - wnd->y;
			}
		}

		if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD && wnd->is_dragged)
		{
			SDL_SetCursor(SDLUI_Core.cursor_arrow);
			i32 old_x = wnd->x;
			i32 old_y = wnd->y;
			wnd->x = mx - wnd->drag_x;
			wnd->y = my - wnd->drag_y;

			for (int i = 0; i < wnd->children.size; ++i)
			{
				wnd->children.data[i]->x += wnd->x - old_x;
				wnd->children.data[i]->y += wnd->y - old_y;
			}
		}

		return false;
	}

	return false;
}

bool SDLUI_Button(SDLUI_Control_Button *btn)
{
	btn->do_process = true;

	if(btn->visible && btn->parent == SDLUI_Core.active_window && SDLUI_Core.active_window->is_hovered)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_Rect r = {btn->x,btn->y,btn->w,btn->h};
		if(SDLUI_PointInRect(r, mx, my))
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

	btn->state = SDLUI_BUTTON_STATE_NORMAL;
	return false;
}

bool SDLUI_SliderInt(SDLUI_Control_SliderInt *si)
{
	si->do_process = true;

	if(si->visible && si->parent == SDLUI_Core.active_window && SDLUI_Core.active_window->is_hovered)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_Rect r = {si->x,si->y,si->w,si->h};
		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				// if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
				// {
				// 	si->value = SDLUI_Map(si->x, si->x + si->w, si->min, si->max, mx);
				// }
				// else
				// {
				// 	si->value = SDLUI_Map(si->y + si->h, si->y, si->min, si->max, my);
				// }

				si->is_changing = true;
			}
		}

		if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD && si->is_changing)
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

			// NOTE: Return here or on mouse release?
			return true;
		}

		if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED && si->is_changing)
		{
			si->is_changing = false;
			return true;
		}

		return false;
	}

	//si->is_changing = false;
	return false;
}

bool SDLUI_CheckBox(SDLUI_Control_CheckBox *chk)
{
	chk->do_process = true;

	if(chk->visible && chk->parent == SDLUI_Core.active_window && SDLUI_Core.active_window->is_hovered)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		i32 tex_w, tex_h;
		SDL_QueryTexture(chk->tex_text, NULL, NULL, &tex_w, &tex_h);

		SDL_Rect r = {chk->x, chk->y, chk->w, chk->h};
		if(chk->tex_text != NULL)
		{
			r.w += SDLUI_MARGIN + tex_w;
		}
		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				chk->checked = !chk->checked;
				return true;
			}
		}
		return false;
	}
	return false;
}

bool SDLUI_ToggleButton(SDLUI_Control_ToggleButton *tb)
{
	tb->do_process = true;

	if(tb->visible && tb->parent == SDLUI_Core.active_window && SDLUI_Core.active_window->is_hovered)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		i32 tex_w, tex_h;
		SDL_QueryTexture(tb->tex_text, NULL, NULL, &tex_w, &tex_h);

		SDL_Rect r = {tb->x,tb->y,tb->w,tb->h};
		if(tb->tex_text != NULL)
		{
			r.w += SDLUI_MARGIN + tex_w;
		}

		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				tb->checked = !tb->checked;
				return true;
			}
		}

		return false;
	}
	return false;
}

bool SDLUI_RadioButton(SDLUI_Control_RadioButton *rb)
{
	rb->do_process = true;

	if(rb->visible && rb->parent == SDLUI_Core.active_window && SDLUI_Core.active_window->is_hovered)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		i32 tex_w, tex_h;
		SDL_QueryTexture(rb->tex_text, NULL, NULL, &tex_w, &tex_h);

		SDL_Rect r = {rb->x,rb->y,rb->w,rb->h};
		if(rb->tex_text != NULL)
		{
			r.w += SDLUI_MARGIN + tex_w;
		}

		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				SDLUI_Control_RadioButton *cur;

				for (int i = 0; i < rb->group->size; ++i)
				{
					cur = (SDLUI_Control_RadioButton*)rb->group->data[i];
					cur->checked = false;
				}

				rb->checked = true;

				return true;
			}
		}

		return false;
	}
	return false;
}

bool SDLUI_Text(SDLUI_Control_Text *txt)
{
	txt->do_process = true;

	if(txt->visible && txt->parent == SDLUI_Core.active_window && SDLUI_Core.active_window->is_hovered)
	{
		return true;
	}
	return false;
}

bool SDLUI_TabContainer(SDLUI_Control_TabContainer *tbc)
{
	tbc->do_process = true;

	if(tbc->visible && tbc->parent == SDLUI_Core.active_window && SDLUI_Core.active_window->is_hovered)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_Rect r = {tbc->x,tbc->y,tbc->w,tbc->bar_height};
		SDL_Rect tab_r;
		i32 offset = 0;
		SDLUI_Control_Tab *tab;

		if(SDLUI_PointInRect(r, mx, my))
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				for (int i = 0; i < tbc->tabs.size; ++i)
				{
					tab = (SDLUI_Control_Tab*)tbc->tabs.data[i];
					tab_r = {tbc->x + offset, tbc->y, tab->w + SDLUI_MARGIN, 30};

					if(SDLUI_PointInRect(tab_r, mx, my))
					{
						for (int j = 0; j < tbc->active_tab->children.size; ++j)
						{
							SDLUI_Control *ctrl = tbc->active_tab->children.data[j];
							ctrl->visible = false;
						}
						tbc->active_tab = tab;
					}

					offset += SDLUI_MARGIN + tab->w;
				}
			}
		}
	}
	return false;
}

bool SDLUI_ScrollArea(SDLUI_Control_ScrollArea *sa)
{
	sa->do_process = true;

	if(sa->visible && sa->parent == SDLUI_Core.active_window && SDLUI_Core.active_window->is_hovered)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_Rect r, rv, rh;

		r = {sa->x, sa->y, sa->w, sa->h};

		if(sa->content_height > sa->h)
		{
			if(SDLUI_PointInRect(r, mx, my))
			{
				// mouse wheel vertical
				if(SDLUI_Core.mouse_wheel_y != 0)
				{
					sa->scroll_y -= SDLUI_Core.mouse_wheel_y;
					sa->scroll_y = SDLUI_Clamp(sa->scroll_y, 0, sa->track_size_v - sa->thumb_size_v);
					SDLUI_Core.mouse_wheel_y = 0;
					return true;
				}
			}

			static i32 my_offset;

			rv = {sa->x + sa->w - sa->scrollbar_thickness, sa->y + sa->scroll_y, sa->scrollbar_thickness, sa->thumb_size_v};
			if(SDLUI_PointInRect(rv, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					my_offset = my - sa->scroll_y;
					sa->is_changing_v = true;
				}
			}

			// scroll per page
			rv.y = sa->y;
			rv.h = sa->scroll_y;

			if(SDLUI_PointInRect(rv, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					float pages = (float)sa->content_height / (float)sa->h;
					float pixels_per_page = (float)sa->track_size_v / pages;
					sa->scroll_y -= pixels_per_page;
					sa->scroll_y = SDLUI_Clamp(sa->scroll_y, 0, sa->track_size_v - sa->thumb_size_v);
					return true;
				}
			}

			rv.y = sa->y + sa->scroll_y + sa->thumb_size_v;
			rv.h = sa->track_size_v - sa->thumb_size_v - sa->scroll_y;

			if(SDLUI_PointInRect(rv, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					float pages = (float)sa->content_height / (float)sa->h;
					float pixels_per_page = (float)sa->track_size_v / pages;
					sa->scroll_y += pixels_per_page;
					sa->scroll_y = SDLUI_Clamp(sa->scroll_y, 0, sa->track_size_v - sa->thumb_size_v);
					return true;
				}
			}

			if(sa->is_changing_v)
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD)
				{
					sa->scroll_y = my - my_offset;
					sa->scroll_y = SDLUI_Clamp(sa->scroll_y, 0, sa->track_size_v - sa->thumb_size_v);
				}

				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED)
				{
					sa->is_changing_v = false;
					return true;
				}
			}
		}

		if(sa->content_width > sa->w)
		{
			if(SDLUI_PointInRect(r, mx, my))
			{
				// mouse wheel horizontal
				if(SDLUI_Core.mouse_wheel_x != 0)
				{
					sa->scroll_x += SDLUI_Core.mouse_wheel_x;
					sa->scroll_x = SDLUI_Clamp(sa->scroll_x, 0, sa->track_size_h - sa->thumb_size_h);
					SDLUI_Core.mouse_wheel_x = 0;
					return true;
				}
			}

			static i32 mx_offset;

			rh = {sa->x + sa->scroll_x, sa->y + sa->h - sa->scrollbar_thickness, sa->thumb_size_h, sa->scrollbar_thickness};
			if(SDLUI_PointInRect(rh, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					mx_offset = mx - sa->scroll_x;
					sa->is_changing_h = true;
				}
			}

			// scroll per page
			rh.x = sa->x;
			rh.w = sa->scroll_x;

			if(SDLUI_PointInRect(rh, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					float pages = (float)sa->content_width / (float)sa->w;
					float pixels_per_page = (float)sa->track_size_h / pages;
					sa->scroll_x -= pixels_per_page;
					sa->scroll_x = SDLUI_Clamp(sa->scroll_x, 0, sa->track_size_h - sa->thumb_size_h);
					return true;
				}
			}

			rh.x = sa->x + sa->scroll_x + sa->thumb_size_h;
			rh.w = sa->track_size_h - sa->thumb_size_h - sa->scroll_x;

			if(SDLUI_PointInRect(rh, mx, my))
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
				{
					float pages = (float)sa->content_width / (float)sa->w;
					float pixels_per_page = (float)sa->track_size_h / pages;
					sa->scroll_x += pixels_per_page;
					sa->scroll_x = SDLUI_Clamp(sa->scroll_x, 0, sa->track_size_h - sa->thumb_size_h);
					return true;
				}
			}

			if(sa->is_changing_h)
			{
				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD)
				{
					sa->scroll_x = mx - mx_offset;
					sa->scroll_x = SDLUI_Clamp(sa->scroll_x, 0, sa->track_size_h - sa->thumb_size_h);
				}

				if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED)
				{
					sa->is_changing_h = false;
					return true;
				}
			}

		}
		return false;
	}
	return false;
}

bool SDLUI_List(SDLUI_Control_List *lst, const char *cur_item, i32 num_items, i32 cur_index)
{
	lst->do_process = true;

	i32 clicked = false;

	if(lst->scroll_area->visible && lst->scroll_area->parent == SDLUI_Core.active_window)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_Rect r = {lst->scroll_area->x, lst->scroll_area->y, lst->scroll_area->client_width, lst->scroll_area->client_height};

		if(SDLUI_PointInRect(r, mx, my) && cur_index == 0)
		{
			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
			{
				float ratio = (float)lst->scroll_area->content_height / (float)lst->scroll_area->client_height;
				float oy = my - lst->scroll_area->y + ((float)lst->scroll_area->scroll_y * ratio);
				lst->selected_index = oy / SDLUI_Font.height;
				lst->selected_index = SDLUI_Clamp(lst->selected_index, 0, lst->num_items - 1);
				clicked = true;
			}
		}
	}

	if(lst->num_items != num_items)
	{
		lst->num_items = num_items;
		SDL_DestroyTexture(lst->scroll_area->tex_rect);
		i32 h = num_items * SDLUI_Font.height;

		if(h < lst->scroll_area->h)
		{
			h = lst->scroll_area->h;
		}

		lst->scroll_area->tex_rect = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, lst->scroll_area->w, h);
		lst->scroll_area->content_width = lst->scroll_area->w;
		lst->scroll_area->content_height = h;
	}

	static i32 counter;

	if(counter == 0)
	{
		SDLUI_SetColor(SDLUI_Core.theme.col_list_bg);
		SDL_Rect r = {0, 0, lst->scroll_area->content_width, lst->scroll_area->content_height};
		SDL_SetRenderTarget(SDLUI_Core.renderer, lst->scroll_area->tex_rect);
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);
		SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
	}

	if(counter == lst->selected_index)
	{
		SDL_SetRenderTarget(SDLUI_Core.renderer, lst->scroll_area->tex_rect);
		SDLUI_SetColor(SDLUI_Core.theme.col_highlight);

		float ratio = (float)lst->scroll_area->content_width / (float)lst->scroll_area->client_width;
		i32 offset_x = lst->scroll_area->scroll_x * ratio;
		SDL_Rect r = {offset_x, lst->selected_index * SDLUI_Font.height, lst->scroll_area->w, SDLUI_Font.height};
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);
	}

	if(strlen(cur_item) * SDLUI_Font.width > lst->max_string_width)
	{
		lst->max_string_width = strlen(cur_item) * SDLUI_Font.width;
	}
	i32 cur_tex_width;
	SDL_QueryTexture(lst->scroll_area->tex_rect, NULL, NULL, &cur_tex_width, NULL);

	if(lst->max_string_width > cur_tex_width)
	{
		SDL_DestroyTexture(lst->scroll_area->tex_rect);
		i32 h = num_items * SDLUI_Font.height;
		if(h < lst->scroll_area->h)
		{
			h = lst->scroll_area->h;
		}
		lst->scroll_area->tex_rect = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, lst->max_string_width, h);
		lst->scroll_area->content_width = lst->max_string_width;
		lst->scroll_area->content_height = h;
	}

	static i32 offset_y;

	SDLUI_DrawText(SDLUI_MARGIN, offset_y, cur_item, lst->scroll_area->tex_rect);
	offset_y += SDLUI_Font.height;
	counter++;

	if(counter == num_items)
	{
		offset_y = 0;
		counter = 0;
        clicked = false;
	}

	return clicked;
}

bool SDLUI_TextBox(SDLUI_Control_TextBox *tbx)
{
	tbx->do_process = true;

	if(tbx->visible && tbx->parent == SDLUI_Core.active_window && SDLUI_Core.active_window->is_hovered)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);
        #ifdef __APPLE__
		mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
		my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
        #endif

		SDL_Rect r = {tbx->x,tbx->y,tbx->w,tbx->h};

		if(SDLUI_PointInRect(r, mx, my))
		{
			SDL_SetCursor(SDLUI_Core.cursor_ibeam);
		}
		else
		{
			SDL_SetCursor(SDLUI_Core.cursor_arrow);
		}


		if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED)
		{
			if(SDLUI_PointInRect(r, mx, my))
			{
				tbx->focused = true;
				return true;
			}
			else
			{
				tbx->focused = false;
			}
		}
	}

	if(tbx->focused && tbx->parent == SDLUI_Core.active_window)
	{
		if(SDLUI_Core.e.type == SDL_TEXTINPUT)
		{
			tbx->text.insert_char(SDLUI_Core.e.text.text[0], tbx->cursor_pos);
			tbx->cursor_pos++;

			if(tbx->cursor_pos > tbx->max_chars)
			{
				tbx->scroll++;
			}
		}

		if(SDLUI_Core.e.type == SDL_KEYDOWN)
		{
			if(SDLUI_Core.e.key.keysym.scancode == SDL_SCANCODE_LEFT)
			{
				if(SDLUI_Core.e.key.keysym.mod == KMOD_LCTRL)
				{
					while(tbx->cursor_pos > 0)
					{
						tbx->cursor_pos--;
						if(tbx->text.data[tbx->cursor_pos] != ' ')
						{
							break;
						}
					}

					while(tbx->cursor_pos > 0)
					{
						int index = tbx->cursor_pos > 0 ? tbx->cursor_pos - 1 : 0;
						if(tbx->text.data[index] == ' ')
						{
							break;
						}

						tbx->cursor_pos--;
					}
				}
				else
				{
					if(tbx->cursor_pos > 0)
					{
						tbx->cursor_pos--;

						if(tbx->cursor_pos < tbx->scroll)
						{
							tbx->scroll--;
        						//0123456789-----15
        						//asdfghjkl1234567
        						//          |    |
						}
					}
				}
			}
			else if(SDLUI_Core.e.key.keysym.scancode == SDL_SCANCODE_RIGHT)
			{
				if(SDLUI_Core.e.key.keysym.mod == KMOD_LCTRL)
				{
					if(tbx->text.data[tbx->cursor_pos] == ' ')
					{
						while(tbx->cursor_pos < tbx->text.length)
						{
							tbx->cursor_pos++;
							if(tbx->text.data[tbx->cursor_pos] != ' ')
							{
								break;
							}
						}
					}

					while(tbx->cursor_pos < tbx->text.length)
					{
						if(tbx->text.data[tbx->cursor_pos] == ' ')
						{
							break;
						}

						tbx->cursor_pos++;
					}
				}
				else
				{
					if(tbx->cursor_pos < tbx->text.length)
					{
						tbx->cursor_pos++;

						if(tbx->cursor_pos > tbx->max_chars)
						{
							tbx->scroll++;
						}
					}
				}
			}
			else if(SDLUI_Core.e.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
			{
				if(SDLUI_Core.e.key.keysym.mod == KMOD_LCTRL)
				{
					int start_to_reverse = tbx->cursor_pos;
					while(tbx->cursor_pos > 0)
					{
						tbx->cursor_pos--;
						if(tbx->text.data[tbx->cursor_pos] != ' ')
						{
							break;
						}
					}

					while(tbx->cursor_pos > 0)
					{
						int index = tbx->cursor_pos > 0 ? tbx->cursor_pos - 1 : 0;
						if(tbx->text.data[index] == ' ')
						{
							break;
						}

						tbx->cursor_pos--;
					}

					int now_cursor_pos = tbx->cursor_pos;
					while(start_to_reverse > now_cursor_pos)
					{
						tbx->text.delete_char(start_to_reverse - 1);
						start_to_reverse--;
					}
				} else
				{
					if(tbx->cursor_pos > 0)
					{
						tbx->text.delete_char(tbx->cursor_pos - 1);
						tbx->cursor_pos--;

						if(tbx->text.length > tbx->max_chars - 1)
						{
							tbx->scroll--;
						}
					}
				}
			} else if(SDLUI_Core.e.key.keysym.scancode == SDL_SCANCODE_HOME)
			{
				tbx->cursor_pos = 0;
				tbx->scroll = 0;
			} else if(SDLUI_Core.e.key.keysym.scancode == SDL_SCANCODE_END)
			{
				tbx->cursor_pos = tbx->text.length;
				tbx->scroll = tbx->text.length - tbx->max_chars;
				if(tbx->scroll < 0)
				{
					tbx->scroll = 0;
				}
			}
		}
	}

	return false;
}
