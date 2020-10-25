void SDLUI_DrawText(i32 x, i32 y, const char *text, SDL_Texture *dst)
{
	SDL_SetRenderTarget(SDLUI_Core.renderer, dst);
	i32 cur_char;
	SDL_Rect glyph_rect;
	SDL_Rect pos = {x, y, SDLUI_Font.width, SDLUI_Font.height};

	i32 count = strlen(text);

	for (int i = 0; i < count; ++i)
	{
		cur_char = (i32)text[i];
		glyph_rect = {(cur_char - 32) * SDLUI_Font.width, 0, SDLUI_Font.width, SDLUI_Font.height};

		SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Font.tex_font, &glyph_rect, &pos);
		pos.x += SDLUI_Font.width;
	}

	SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
}

void SDLUI_DrawTextRange(i32 x, i32 y, const char *text, i32 start, i32 length, SDL_Texture *dst)
{
	SDL_SetRenderTarget(SDLUI_Core.renderer, dst);
	i32 cur_char;
	SDL_Rect glyph_rect;
	SDL_Rect pos = {x, y, SDLUI_Font.width, SDLUI_Font.height};

	for (int i = start; i < start + length; ++i)
	{
		cur_char = (i32)text[i];
		glyph_rect = {(cur_char - 32) * SDLUI_Font.width, 0, SDLUI_Font.width, SDLUI_Font.height};

		SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Font.tex_font, &glyph_rect, &pos);
		pos.x += SDLUI_Font.width;
	}

	SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
}

void SDLUI_Render_Button(SDLUI_Control_Button *btn)
{
	if(btn->visible)
	{
		if(btn->text.modified)
		{
			SDL_Color c = {255, 255, 255, 255};
			SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle, btn->text.data, c);
			btn->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
			SDL_FreeSurface(s);
		}

		i32 xx = btn->x - btn->parent->x;
		i32 yy = btn->y - btn->parent->y;

		SDL_Rect r = {xx, yy, btn->w, btn->h};

		switch (btn->state)
		{
			case SDLUI_BUTTON_STATE_NORMAL:
			SDLUI_SetColor(SDLUI_Core.theme.col_button_normal);
			break;

			case SDLUI_BUTTON_STATE_HOVER:
			SDLUI_SetColor(SDLUI_Core.theme.col_highlight);
			break;

			case SDLUI_BUTTON_STATE_CLICK:
			SDLUI_SetColor(SDLUI_Core.theme.col_button_click);
			break;
		}

		SDL_RenderFillRect(SDLUI_Core.renderer, &r);

		SDLUI_SetColor(SDLUI_Core.theme.col_grey);
		SDL_RenderDrawRect(SDLUI_Core.renderer, &r);

		i32 tex_w, tex_h;
		SDL_QueryTexture(btn->tex_text, NULL, NULL, &tex_w, &tex_h);

		i32 diff = btn->w - tex_w;
		i32 offset = diff * btn->align / 2;

		if(btn->align == SDLUI_ALIGN_LEFT)
		{
			offset += SDLUI_MARGIN;
		}
		else if(btn->align == SDLUI_ALIGN_RIGHT)
		{
			offset -= SDLUI_MARGIN;
		}

		r = {xx + offset, yy + SDLUI_MARGIN, tex_w, tex_h};
		SDL_RenderCopy(SDLUI_Core.renderer, btn->tex_text, NULL, &r);
	}
}

void SDLUI_Render_SliderInt(SDLUI_Control_SliderInt *si)
{
	if(si->visible)
	{
		if(si->orientation == SDLUI_ORIENTATION_HORIZONTAL)
		{
			i32 xx = si->x - si->parent->x;
			i32 yy = si->y - si->parent->y;

			i32 fill = (i32)SDLUI_Map(si->min, si->max, 0, si->w, si->value);

			SDLUI_SetColor(SDLUI_Core.theme.col_highlight);
			SDL_RenderDrawLine(SDLUI_Core.renderer, xx, yy + 8, xx + fill, yy + 8);

			SDLUI_SetColor(SDLUI_Core.theme.col_border);
			SDL_RenderDrawLine(SDLUI_Core.renderer, xx + fill, yy + 8, xx + si->w, yy + 8);

			SDL_Rect r = {xx + fill-8, yy, 16, 16};
			r.x = SDLUI_Clamp(r.x, xx, xx + si->w - 14);
			SDLUI_Colorize(SDLUI_Core.tex_circle_fill_1, SDLUI_Core.theme.col_scrollbar_thumb);
			SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_circle_fill_1, NULL, &r);
			SDLUI_Colorize(SDLUI_Core.tex_circle, SDLUI_Core.theme.col_white);
			SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_circle, NULL, &r);
		}
		else
		{
			i32 xx = si->x - si->parent->x;
			i32 yy = si->y - si->parent->y;

			i32 fill = (i32)SDLUI_Map(si->min, si->max, 0, si->h, si->value);

			SDLUI_SetColor(SDLUI_Core.theme.col_highlight);
			SDL_RenderDrawLine(SDLUI_Core.renderer, xx + 8, yy + si->h, xx + 8, yy + si->h - fill);

			SDLUI_SetColor(SDLUI_Core.theme.col_border);
			SDL_RenderDrawLine(SDLUI_Core.renderer, xx + 8, yy + si->h - fill, xx + 8, yy);

			SDL_Rect r = {xx, yy + si->h - fill-8, 16, 16};
			r.y = SDLUI_Clamp(r.y, yy, yy + si->h - 14);
			SDLUI_Colorize(SDLUI_Core.tex_circle_fill_1, SDLUI_Core.theme.col_scrollbar_thumb);
			SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_circle_fill_1, NULL, &r);
			SDLUI_Colorize(SDLUI_Core.tex_circle, SDLUI_Core.theme.col_white);
			SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_circle, NULL, &r);
		}
	}
}

void SDLUI_Render_CheckBox(SDLUI_Control_CheckBox *chk)
{
	if(chk->visible)
	{
		i32 xx = chk->x - chk->parent->x;
		i32 yy = chk->y - chk->parent->y;

		SDLUI_SetColor(SDLUI_Core.theme.col_white);

		SDL_Rect r = {xx, yy, chk->w, chk->h};
		SDL_RenderDrawRect(SDLUI_Core.renderer, &r);

		if(chk->checked)
		{
			SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_tick, NULL, &r);
		}

		if(chk->tex_text != NULL)
		{
			i32 tex_w, tex_h;
			SDL_QueryTexture(chk->tex_text, NULL, NULL, &tex_w, &tex_h);
			r = {xx + chk->w + SDLUI_MARGIN, yy+2, tex_w, tex_h};
			SDL_RenderCopy(SDLUI_Core.renderer, chk->tex_text, NULL, &r);
		}
	}
}

void SDLUI_Render_Text(SDLUI_Control_Text *txt)
{
	if(txt->visible)
	{
		if(txt->text.modified)
		{
			txt->w = (txt->text.length) * SDLUI_Font.width;
			SDL_Color c = {255, 255, 255, 255};
			SDL_Surface *s = TTF_RenderText_Blended(SDLUI_Font.handle,txt->text.data, c);
			txt->tex_text = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);
			SDL_FreeSurface(s);
			txt->text.modified = false;
		}

		i32 xx = txt->x - txt->parent->x;
		i32 yy = txt->y - txt->parent->y;

		SDLUI_SetColor(SDLUI_Core.theme.col_white);
		SDL_Rect r = {xx, yy, txt->w, txt->h};
		SDL_RenderCopy(SDLUI_Core.renderer, txt->tex_text, NULL, &r);
	}
}

void SDLUI_Render_ToggleButton(SDLUI_Control_ToggleButton *tb)
{
	if(tb->visible)
	{
		i32 xx = tb->x - tb->parent->x;
		i32 yy = tb->y - tb->parent->y;

		SDL_Rect r = {xx, yy, tb->w, tb->h};
		SDLUI_Colorize(SDLUI_Core.tex_circle, SDLUI_Core.theme.col_white);
		SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_toggle, NULL, &r);

		if(tb->checked)
		{
			r = {xx + 16, yy, tb->w - 16, tb->h};
			SDLUI_Colorize(SDLUI_Core.tex_circle_fill_2, SDLUI_Core.theme.col_highlight);
			SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_circle_fill_2, NULL, &r);
		}
		else
		{
			r = {xx, yy, tb->w - 16, tb->h};
			SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_circle_fill_2, NULL, &r);
		}

		SDLUI_Colorize(SDLUI_Core.tex_circle_fill_2, SDLUI_Core.theme.col_white);

		if(tb->tex_text != NULL)
		{
			i32 tex_w, tex_h;
			SDL_QueryTexture(tb->tex_text, NULL, NULL, &tex_w, &tex_h);
			r = {xx + tb->w + SDLUI_MARGIN, yy+2, tex_w, tex_h};
			SDL_RenderCopy(SDLUI_Core.renderer, tb->tex_text, NULL, &r);
		}
	}
}

void SDLUI_Render_RadioButton(SDLUI_Control_RadioButton *rb)
{
	if(rb->visible)
	{
		i32 xx = rb->x - rb->parent->x;
		i32 yy = rb->y - rb->parent->y;

		SDLUI_Colorize(SDLUI_Core.tex_circle, SDLUI_Core.theme.col_white);
		SDL_Rect r = {xx, yy, rb->w, rb->h};
		SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_circle, NULL, &r);

		if(rb->checked)
		{
			SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_circle_fill_2, NULL, &r);
		}

		if(rb->tex_text != NULL)
		{
			i32 tex_w, tex_h;
			SDL_QueryTexture(rb->tex_text, NULL, NULL, &tex_w, &tex_h);
			r = {xx + rb->w + SDLUI_MARGIN, yy+2, tex_w, tex_h};
			SDL_RenderCopy(SDLUI_Core.renderer, rb->tex_text, NULL, &r);
		}
	}
}

void SDLUI_RenderChild(SDLUI_CONTROL_TYPE type, SDLUI_Control *ctrl);

void SDLUI_Render_Tabcontainer(SDLUI_Control_TabContainer *tbc)
{
	if(tbc->visible)
	{
		i32 xx = tbc->x - tbc->parent->x;
		i32 yy = tbc->y - tbc->parent->y;
		i32 offset = 0;

		SDL_Rect r = {xx, yy, tbc->w, tbc->bar_height};
		SDLUI_SetColor(SDLUI_Core.theme.col_inactive_window_bar);
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);

		SDLUI_Control_Tab *tab;

		for (int i = 0; i < tbc->tabs.size; ++i)
		{
			tab = (SDLUI_Control_Tab*)tbc->tabs.data[i];

			SDLUI_SetColor(SDLUI_Core.theme.col_white);
			r = {SDLUI_MARGIN + xx + offset, yy + SDLUI_MARGIN, tab->w, tab->h};
			SDL_RenderCopy(SDLUI_Core.renderer, tab->tex_text, NULL, &r);

			if(tab == tbc->active_tab)
			{
				SDLUI_SetColor(SDLUI_Core.theme.col_highlight);
				r.y += 2*SDLUI_MARGIN;
				r.h = 2;
				SDL_RenderFillRect(SDLUI_Core.renderer, &r);
			}

			offset += SDLUI_MARGIN + tab->w;
		}

		SDLUI_SetColor(SDLUI_Core.theme.col_window_bg);
		r = {xx, yy + tbc->bar_height, tbc->w, tbc->h - tbc->bar_height};
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);

		SDLUI_SetColor(SDLUI_Core.theme.col_grey);
		r = {xx, yy, tbc->w, tbc->h};
		SDL_RenderDrawRect(SDLUI_Core.renderer, &r);

		tab = (SDLUI_Control_Tab*)tbc->active_tab;

		SDLUI_CONTROL_TYPE type;
		SDLUI_Control *ctrl;

		for (int j = 0; j < tab->children.size; ++j)
		{
			type = tab->children.data[j]->type;
			ctrl = tab->children.data[j];

			ctrl->visible = true;

			if (ctrl->do_process)
			{
				SDLUI_RenderChild(type, ctrl);
			}
		}
	}
}

void SDLUI_Render_ScrollArea(SDLUI_Control_ScrollArea *sa)
{
	if(sa->visible)
	{
		i32 xx = sa->x - sa->parent->x;
		i32 yy = sa->y - sa->parent->y;

		SDL_Rect r;
		SDL_Rect dst = {xx, yy, sa->client_width, sa->client_height};

		if(sa->content_height > sa->h)
		{
			// vertical scrollbar
			SDLUI_SetColor(SDLUI_Core.theme.col_scrollbar_track);
			r = {xx + sa->w - sa->scrollbar_thickness, yy, sa->scrollbar_thickness, sa->track_size_v};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);

			SDLUI_SetColor(SDLUI_Core.theme.col_scrollbar_thumb);
			sa->thumb_size_v = (float)(sa->track_size_v * (sa->h - sa->scrollbar_thickness)) / (float)sa->content_height;

			r = {xx + sa->w - sa->scrollbar_thickness+1, yy + sa->scroll_y, sa->scrollbar_thickness-4, sa->thumb_size_v};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);
		}

		if(sa->content_width > sa->w)
		{
			// horizontal scrollbar
			SDLUI_SetColor(SDLUI_Core.theme.col_scrollbar_track);
			r = {xx, yy + sa->h - sa->scrollbar_thickness, sa->track_size_h, sa->scrollbar_thickness};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);

			SDLUI_SetColor(SDLUI_Core.theme.col_scrollbar_thumb);
			sa->thumb_size_h = (float)(sa->track_size_h * (sa->w - sa->scrollbar_thickness)) / (float)sa->content_width;

			r = {xx + sa->scroll_x, yy + sa->h - sa->scrollbar_thickness+1, sa->thumb_size_h, sa->scrollbar_thickness-4};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);
		}

		// Texture
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

		float ratio_y = (float)sa->content_height / sa->client_height;
		float ratio_x = (float)sa->content_width / sa->client_width;

		float offset_x = sa->scroll_x * ratio_x;
		float offset_y = sa->scroll_y * ratio_y;

		offset_x = SDLUI_Clamp(offset_x, 0, sa->content_width - sa->client_width);
		offset_y = SDLUI_Clamp(offset_y, 0, sa->content_height - sa->client_height);

		SDL_Rect src = {(i32)offset_x, (i32)offset_y, sa->client_width, sa->client_height};
		SDL_RenderCopy(SDLUI_Core.renderer, sa->tex_rect, &src, &dst);

		// Container rect
		SDLUI_SetColor(SDLUI_Core.theme.col_grey);
		r = {xx, yy, sa->w, sa->h};
		SDL_RenderDrawRect(SDLUI_Core.renderer, &r);
	}
}

void SDLUI_Render_TextBox(SDLUI_Control_TextBox *tbx)
{
	if(tbx->visible)
	{
		i32 xx = tbx->x - tbx->parent->x;
		i32 yy = tbx->y - tbx->parent->y;

		SDLUI_SetColor(SDLUI_Core.theme.col_textbox_bg);
		SDL_Rect r = {xx, yy, tbx->w, tbx->h};
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);

		SDLUI_SetColor(SDLUI_Core.theme.col_grey);
		SDL_RenderDrawRect(SDLUI_Core.renderer, &r);

		if(tbx->text.length > 0)
		{
			i32 tex_w, tex_h;
			SDL_QueryTexture(tbx->tex_text, NULL, NULL, &tex_w, &tex_h);
			SDL_Rect src = {0, 0, tex_w, tex_h};
			SDL_Rect dst = {xx + SDLUI_MARGIN, yy + SDLUI_MARGIN, src.w, src.h};
			SDL_RenderCopy(SDLUI_Core.renderer, tbx->tex_text, &src, &dst);
		}

		if(tbx->focused && tbx->parent == SDLUI_Core.active_window && SDLUI_Core.active_window != NULL)
		{
			r = {xx + SDLUI_MARGIN + ((tbx->cursor_pos - tbx->scroll) * SDLUI_Font.width), yy + 6, 2, tbx->h - 12};
			SDLUI_SetColor(SDLUI_Core.theme.col_white);
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);
		}

		SDL_SetRenderTarget(SDLUI_Core.renderer, tbx->tex_text);
		r = {0, 0, tbx->w, tbx->h};
		SDLUI_SetColor(SDLUI_Core.theme.col_textbox_bg);
		SDL_RenderFillRect(SDLUI_Core.renderer, &r);

		SDLUI_SetColor(SDLUI_Core.theme.col_white);
		if(tbx->text.length <= tbx->max_chars)
		{
			SDLUI_DrawText(0, 0, tbx->text.data, tbx->tex_text);
		}
		else
		{
			SDLUI_DrawTextRange(0, 0, tbx->text.data, tbx->scroll, tbx->max_chars, tbx->tex_text);
		}
	}
}

void SDLUI_RenderChild(SDLUI_CONTROL_TYPE type, SDLUI_Control *ctrl)
{
	switch (type)
	{
		case SDLUI_CONTROL_TYPE_BUTTON:
		{
			SDLUI_Render_Button((SDLUI_Control_Button*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_SLIDER_INT:
		{
			SDLUI_Render_SliderInt((SDLUI_Control_SliderInt*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_CHECKBOX:
		{
			SDLUI_Render_CheckBox((SDLUI_Control_CheckBox*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_TOGGLE_BUTTON:
		{
			SDLUI_Render_ToggleButton((SDLUI_Control_ToggleButton*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_RADIO_BUTTON:
		{
			SDLUI_Render_RadioButton((SDLUI_Control_RadioButton*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_TEXT:
		{
			SDLUI_Render_Text((SDLUI_Control_Text*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_TAB_CONTAINER:
		{
			SDLUI_Render_Tabcontainer((SDLUI_Control_TabContainer*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_SCROLL_AREA:
		{
			SDLUI_Render_ScrollArea((SDLUI_Control_ScrollArea*)ctrl);
		}
		break;

		case SDLUI_CONTROL_TYPE_TEXTBOX:
		{
			SDLUI_Render_TextBox((SDLUI_Control_TextBox*)ctrl);
		}
		break;

		default:
    		break;
	}
}

void SDLUI_Render_Window(SDLUI_Control_Window *wnd)
{
	if(!wnd->do_process)
	{
		return;
	}

	if(wnd->visible != wnd->visible_last_frame)
	{
		for (int i = 0; i < wnd->children.size; ++i)
		{
			wnd->children.data[i]->visible = wnd->visible;
			wnd->visible_last_frame = wnd->visible;
		}

	}

	if(wnd->enabled != wnd->enabled_last_frame)
	{
		for (int i = 0; i < wnd->children.size; ++i)
		{
			wnd->children.data[i]->enabled = wnd->enabled;
			wnd->enabled_last_frame = wnd->enabled;
		}

	}

	if(wnd->visible)
	{
		if(wnd == SDLUI_Core.active_window)
		{
			SDL_SetRenderTarget(SDLUI_Core.renderer, wnd->tex_rect);
			SDLUI_SetColor(SDLUI_Core.theme.col_active_window_bar);
			SDL_Rect r = {0, 0, wnd->w, 30};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);

			SDLUI_SetColor(SDLUI_Core.theme.col_white);
			i32 tex_w, tex_h;
			SDL_QueryTexture(wnd->tex_title, NULL, NULL, &tex_w, &tex_h);
			r = {SDLUI_MARGIN, SDLUI_MARGIN, tex_w, tex_h};
			SDL_RenderCopy(SDLUI_Core.renderer, wnd->tex_title, NULL, &r);

			SDLUI_SetColor(SDLUI_Core.theme.col_window_bg);
			r = {0, 0 + 30, wnd->w, wnd->h - 30};
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);

			i32 mx, my;
			SDL_GetMouseState(&mx, &my);
            #ifdef __APPLE__
            mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
            my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
            #endif
            
			r = {wnd->x + wnd->w - 30, wnd->y, 30, 30};

			if(wnd->has_close_button)
			{
				if(SDLUI_PointInRect(r, mx, my) && !wnd->is_resized)
				{
					SDLUI_SetColor(SDLUI_Core.theme.col_red);
					r = {wnd->w - 29, 1, 28, 28};
				}
				else
				{
					SDLUI_SetColor(SDLUI_Core.theme.col_active_window_bar);
					r = {wnd->w - 29, 1, 28, 28};
				}

				r = {0 + wnd->w - 30, 0, 30, 30};
        			SDL_RenderFillRect(SDLUI_Core.renderer, &r);
				SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_close, NULL, &r);
			}

			SDLUI_CONTROL_TYPE type;
			SDLUI_Control *ctrl;

			for (int j = 0; j < wnd->children.size; ++j)
			{
				type = wnd->children.data[j]->type;
				ctrl = wnd->children.data[j];

				if(ctrl->owned_by_window && ctrl->do_process)
				{
					if(ctrl->x < wnd->x + wnd->w && ctrl->y < wnd->y + wnd->h)
					{
						SDLUI_RenderChild(type, ctrl);
					}
				}

				/* Make sure to reset the render target back to the window's
                                 * because the child might have changed it */
				SDL_SetRenderTarget(SDLUI_Core.renderer, wnd->tex_rect);
			}

			SDLUI_SetColor(SDLUI_Core.theme.col_grey);
			r = {0, 0, wnd->w, wnd->h};
			SDL_RenderDrawRect(SDLUI_Core.renderer, &r);
			r = {wnd->x, wnd->y, wnd->w, wnd->h};
			SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
			SDL_RenderCopy(SDLUI_Core.renderer, wnd->tex_rect, NULL, &r);
		}
		else
		{
			SDL_SetRenderTarget(SDLUI_Core.renderer, wnd->tex_rect);
			SDL_Rect r = {0, 0, wnd->w, 30};
			SDLUI_SetColor(SDLUI_Core.theme.col_inactive_window_bar);
			SDL_RenderFillRect(SDLUI_Core.renderer, &r);

			SDLUI_SetColor(SDLUI_Core.theme.col_white);
			i32 tex_w, tex_h;
			SDL_QueryTexture(wnd->tex_title, NULL, NULL, &tex_w, &tex_h);
			r = {SDLUI_MARGIN, SDLUI_MARGIN, tex_w, tex_h};
			SDL_RenderCopy(SDLUI_Core.renderer, wnd->tex_title, NULL, &r);

			if(wnd->has_close_button)
			{
				r = {0 + wnd->w - 30, 0, 30, 30};
				SDL_RenderCopy(SDLUI_Core.renderer, SDLUI_Core.tex_close, NULL, &r);
			}

			SDLUI_SetColor(SDLUI_Core.theme.col_grey);
			r = {0, 0, wnd->w, wnd->h};
			SDL_RenderDrawRect(SDLUI_Core.renderer, &r);

			r = {wnd->x, wnd->y, wnd->w, wnd->h};
			SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
			SDL_RenderCopy(SDLUI_Core.renderer, wnd->tex_rect, NULL, &r);
		}
	}
}

void SDLUI_ForceReDraw()
{
	SDLUI_Control_Window *wnd;
	SDLUI_Control_Window *aw = SDLUI_Core.active_window;

	for (int i = 0; i < SDLUI_Window_Collection.size; ++i)
	{
		wnd = (SDLUI_Control_Window*)SDLUI_Window_Collection.data[i];
		SDLUI_Core.active_window = wnd;
		SDLUI_Render_Window(wnd);
	}

	SDLUI_Core.active_window = aw;
}

void SDLUI_Render()
{
	SDLUI_Control_Window *wnd;

	static bool once = false;
	if(!once)
	{
		SDLUI_ForceReDraw();
		once = true;
	}

	for (int i = 0; i < SDLUI_Window_Collection.size; ++i)
	{
		wnd = (SDLUI_Control_Window*)SDLUI_Window_Collection.data[i];
		SDLUI_Render_Window(wnd);
	}

	SDLUI_MouseStateReset();
}
