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

bool SDLUI_PointInRect(SDL_Rect rect, i32 x, i32 y)
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
	#ifdef _WIN32
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
	#endif

	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_RWops *rw;
	rw = SDL_RWFromMem((void*)sdlui_font_data, sizeof(sdlui_font_data));
	SDLUI_Font.handle = TTF_OpenFontRW(rw, 1, SDLUI_Font.size);

	SDLUI_Font.height = TTF_FontHeight(SDLUI_Font.handle);
	TTF_SizeText(SDLUI_Font.handle, "0", &SDLUI_Font.width, &SDLUI_Font.height);

	SDLUI_Core.renderer = r;
	SDLUI_Core.window = w;
	SDL_GetWindowSize(SDLUI_Core.window, &SDLUI_Core.window_width, &SDLUI_Core.window_height);
    
    // Managing High DPI texture to window ratio (MacOS)
    i32 renderer_width, renderer_height;
    SDL_GetRendererOutputSize(SDLUI_Core.renderer, &renderer_width, &renderer_height);
    SDLUI_Core.texture_window_hdpi_ratio_x=(float)renderer_width/SDLUI_Core.window_width;
    SDLUI_Core.texture_window_hdpi_ratio_y=(float)renderer_height/SDLUI_Core.window_height;
    
	SDLUI_Window_Collection.create();

	SDLUI_Core.cursor_arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	SDLUI_Core.cursor_ibeam = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);

	SDLUI_Core.cursor_size_we = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
	SDLUI_Core.cursor_size_ns = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);

	SDLUI_Core.cursor_size_nwse = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	SDLUI_Core.cursor_size_nesw = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);

	SDL_SetCursor(SDLUI_Core.cursor_arrow);

	for (int i = 0; i <= 95; ++i)
	{
		SDLUI_Font.ascii[i] = 32 + i;
	}
	SDLUI_Font.ascii[95] = '\0';

	SDL_Surface *characters = TTF_RenderText_Blended(SDLUI_Font.handle, SDLUI_Font.ascii, SDLUI_Core.theme.col_white);
	SDLUI_Font.tex_font = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, characters);
	SDL_FreeSurface(characters);

	SDL_Surface *s;
	// SDL_RWops *rw;

	rw = SDL_RWFromMem((void*)sdlui_png_tick, sizeof(sdlui_png_tick));
	s = IMG_LoadPNG_RW(rw);
	SDLUI_Core.tex_tick = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);

	rw = SDL_RWFromMem((void*)sdlui_png_toggle, sizeof(sdlui_png_toggle));
	s = IMG_LoadPNG_RW(rw);
	SDLUI_Core.tex_toggle = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);

	rw = SDL_RWFromMem((void*)sdlui_png_circlefillsmall, sizeof(sdlui_png_circlefillsmall));
	s = IMG_LoadPNG_RW(rw);
	SDLUI_Core.tex_circle_fill_2 = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);

	rw = SDL_RWFromMem((void*)sdlui_png_circlefillbig, sizeof(sdlui_png_circlefillbig));
	s = IMG_LoadPNG_RW(rw);
	SDLUI_Core.tex_circle_fill_1 = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);

	rw = SDL_RWFromMem((void*)sdlui_png_circle, sizeof(sdlui_png_circle));
	s = IMG_LoadPNG_RW(rw);
	SDLUI_Core.tex_circle = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);

	rw = SDL_RWFromMem((void*)sdlui_png_close, sizeof(sdlui_png_close));
	s = IMG_LoadPNG_RW(rw);
	SDLUI_Core.tex_close = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, s);

	SDL_FreeSurface(s);
	SDL_RWclose(rw);
}

void SDLUI_MouseStateReset()
{
	for (int i = 0; i < 5; ++i)
	{
		SDLUI_Core.mouse_last_frame[i] = SDLUI_Core.mouse_current_frame[i];
	}
}

void SDLUI_MouseStateSet(i32 event_type, i32 mbutton)
{
	SDLUI_Core.mouse_current_frame[mbutton] = (event_type == SDL_MOUSEBUTTONDOWN || event_type == SDL_MOUSEMOTION);
}

SDLUI_MOUSEBUTTON SDLUI_MouseButton(i32 mbutton)
{
	if(SDLUI_Core.mouse_current_frame[mbutton] && !SDLUI_Core.mouse_last_frame[mbutton])
	{
		return SDLUI_MOUSEBUTTON_PRESSED;
	}
	else if(SDLUI_Core.mouse_current_frame[mbutton] && SDLUI_Core.mouse_last_frame[mbutton])
	{
		return SDLUI_MOUSEBUTTON_HELD;
	}
	else if(!SDLUI_Core.mouse_current_frame[mbutton] && SDLUI_Core.mouse_last_frame[mbutton])
	{
		return SDLUI_MOUSEBUTTON_RELEASED;
	}

	return SDLUI_MOUSEBUTTON_NONE;
}

void SDLUI_SetColor(SDL_Color c)
{
	SDL_SetRenderDrawColor(SDLUI_Core.renderer, c.r, c.g, c.b, c.a);
}

void SDLUI_GradientToTexture(SDL_Texture *t, SDL_Color c, i32 width, i32 height, i32 interv)
{
	SDL_SetRenderTarget(SDLUI_Core.renderer, t);

	i32 x = 0, y = 0;
	SDLUI_SetColor(c);

	for (int i = 0; i < height; ++i)
	{
		SDL_RenderDrawLine(SDLUI_Core.renderer, x, y + i, width, y + i);

		if(i % interv == 0)
		{
			c.r -= interv;
			c.g -= interv;
			c.b -= interv;
		}
	}

	SDL_SetRenderTarget(SDLUI_Core.renderer, NULL);
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
	SDLUI_Core.active_window = wnd;
}

SDLUI_RESIZE_DIRECTION SDLUI_SetWindowResizeCursor(SDLUI_Control_Window *wnd, i32 mousex, i32 mousey)
{
	SDL_Rect left, top, right, bottom, lt, rt, lb, rb;
	left = {wnd->x-8, wnd->y, 8, wnd->h};
	top = {wnd->x, wnd->y-8, wnd->w, 8};
	right = {wnd->x+wnd->w, wnd->y, 8, wnd->h};
	bottom = {wnd->x, wnd->y+wnd->h, wnd->w, 8};

	lt = {wnd->x-8, wnd->y-8, 8, 8};
	rt = {wnd->x+wnd->w, wnd->y-8, 8, 8};
	lb = {wnd->x-8, wnd->y+wnd->h, 8, 8};
	rb = {wnd->x+wnd->w, wnd->y+wnd->h, 8, 8};

	if(SDLUI_PointInRect(left, mousex, mousey))
	{
		SDL_SetCursor(SDLUI_Core.cursor_size_we);
		return SDLUI_RESIZE_LEFT;
	}
	else if(SDLUI_PointInRect(top, mousex, mousey))
	{
		SDL_SetCursor(SDLUI_Core.cursor_size_ns);
		return SDLUI_RESIZE_TOP;
	}
	else if(SDLUI_PointInRect(right, mousex, mousey))
	{
		SDL_SetCursor(SDLUI_Core.cursor_size_we);
		return SDLUI_RESIZE_RIGHT;
	}
	else if(SDLUI_PointInRect(bottom, mousex, mousey))
	{
		SDL_SetCursor(SDLUI_Core.cursor_size_ns);
		return SDLUI_RESIZE_BOTTOM;
	}
	else if(SDLUI_PointInRect(lt, mousex, mousey))
	{
		SDL_SetCursor(SDLUI_Core.cursor_size_nwse);
		return SDLUI_RESIZE_LEFT_TOP;
	}
	else if(SDLUI_PointInRect(rt, mousex, mousey))
	{
		SDL_SetCursor(SDLUI_Core.cursor_size_nesw);
		return SDLUI_RESIZE_RIGHT_TOP;
	}
	else if(SDLUI_PointInRect(lb, mousex, mousey))
	{
		SDL_SetCursor(SDLUI_Core.cursor_size_nesw);
		return SDLUI_RESIZE_LEFT_BOTTOM;
	}
	else if(SDLUI_PointInRect(rb, mousex, mousey))
	{
		SDL_SetCursor(SDLUI_Core.cursor_size_nwse);
		return SDLUI_RESIZE_RIGHT_BOTTOM;
	}

	SDL_SetCursor(SDLUI_Core.cursor_arrow);

	return SDLUI_RESIZE_NONE;
}

void SDLUI_ResizeWindow(SDLUI_Control_Window *aw, SDLUI_RESIZE_DIRECTION res_dir, i32 mx, i32 my)
{
	if(res_dir == SDLUI_RESIZE_RIGHT)
	{
		aw->w = mx - aw->x;
	}
	else if(res_dir == SDLUI_RESIZE_LEFT)
	{
		i32 old_x = aw->x;
		aw->w += aw->x - mx;
		if(aw->w > SDLUI_WINDOW_MIN_SIZE)
		{
			aw->x = mx;
		}

		for (int i = 0; i < aw->children.size; ++i)
		{
			aw->children.data[i]->x += aw->x - old_x;
		}
	}
	else if(res_dir == SDLUI_RESIZE_BOTTOM)
	{
		aw->h = my - aw->y;
	}
	else if(res_dir == SDLUI_RESIZE_TOP)
	{
		i32 old_y = aw->y;
		aw->h += aw->y - my;
		if(aw->h > SDLUI_WINDOW_MIN_SIZE)
		{
			aw->y = my;
		}

		for (int i = 0; i < aw->children.size; ++i)
		{
			aw->children.data[i]->y += aw->y - old_y;
		}
	}
	else if(res_dir == SDLUI_RESIZE_LEFT_TOP)
	{
		i32 old_x = aw->x;
		i32 old_y = aw->y;
		aw->w += aw->x - mx;
		if(aw->w > SDLUI_WINDOW_MIN_SIZE)
		{
			aw->x = mx;
		}
		aw->h += aw->y - my;
		if(aw->h > SDLUI_WINDOW_MIN_SIZE)
		{
			aw->y = my;
		}

		for (int i = 0; i < aw->children.size; ++i)
		{
			aw->children.data[i]->x += aw->x - old_x;
			aw->children.data[i]->y += aw->y - old_y;
		}
	}
	else if(res_dir == SDLUI_RESIZE_RIGHT_TOP)
	{
		i32 old_y = aw->y;
		aw->h += aw->y - my;
		if(aw->h > SDLUI_WINDOW_MIN_SIZE)
		{
			aw->y = my;
		}
		aw->w = mx - aw->x;

		for (int i = 0; i < aw->children.size; ++i)
		{
			aw->children.data[i]->y += aw->y - old_y;
		}
	}
	else if(res_dir == SDLUI_RESIZE_LEFT_BOTTOM)
	{
		i32 old_x = aw->x;
		aw->w += aw->x - mx;
		if(aw->w > SDLUI_WINDOW_MIN_SIZE)
		{
			aw->x = mx;
		}
		aw->h = my - aw->y;

		for (int i = 0; i < aw->children.size; ++i)
		{
			aw->children.data[i]->x += aw->x - old_x;
		}
	}
	else if(res_dir == SDLUI_RESIZE_RIGHT_BOTTOM)
	{
		aw->w = mx - aw->x;
		aw->h = my - aw->y;
	}

	aw->w = SDLUI_Clamp(aw->w, 120, 10000);
	aw->h = SDLUI_Clamp(aw->h, 120, 10000);
}

void SDLUI_WindowHandler()
{
	i32 mx, my, index = 0;
	SDL_GetMouseState(&mx, &my);
    #ifdef __APPLE__
    mx=mx*SDLUI_Core.texture_window_hdpi_ratio_x;
    my=my*SDLUI_Core.texture_window_hdpi_ratio_y;
    #endif
    
	SDLUI_Control_Window *aw = SDLUI_Core.active_window;
	static SDLUI_RESIZE_DIRECTION res_dir = SDLUI_RESIZE_NONE;

	if(aw != NULL)
	{
		if(!aw->is_resized && aw->can_be_resized)
		{
			res_dir = SDLUI_SetWindowResizeCursor(aw, mx, my);

			if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_HELD)
			{
				aw->is_resized = true;
			}
		}

		if(aw->is_resized && !aw->is_dragged)
		{
			SDLUI_ResizeWindow(aw, res_dir, mx, my);
			SDL_DestroyTexture(aw->tex_rect);
			aw->tex_rect = SDL_CreateTexture(SDLUI_Core.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, aw->w, aw->h);
		}
		if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_RELEASED && aw->is_resized)
		{
			aw->is_resized = false;
			SDL_SetCursor(SDLUI_Core.cursor_arrow);
		}
	}

	if(SDLUI_MouseButton(SDL_BUTTON_LEFT) == SDLUI_MOUSEBUTTON_PRESSED && SDL_GetCursor() == SDLUI_Core.cursor_arrow)
	{
		SDLUI_Control_Window *wnd;
		SDL_Rect r;
		bool hovers_window = false;

		for (int i = 0; i < SDLUI_Window_Collection.size; ++i)
		{
			wnd = (SDLUI_Control_Window*)SDLUI_Window_Collection.data[i];

			if(wnd->visible)
			{
				r = {wnd->x, wnd->y, wnd->w, wnd->h};

				if(SDLUI_PointInRect(r, mx, my))
				{
					index = i;
					hovers_window = true;
				}
			}
		}

		if(SDLUI_Window_Collection.data[index] != SDLUI_Core.active_window && hovers_window)
		{
			SDLUI_SetActiveWindow((SDLUI_Control_Window*)SDLUI_Window_Collection.data[index]);
		}
		else if(!hovers_window)
		{
			SDLUI_Core.active_window = NULL;
		}
	}
}

// Got this from StackOverflow.
void save_texture(SDL_Renderer *ren, SDL_Texture *tex, const char *filename)
{
	SDL_Texture *ren_tex;
	SDL_Surface *surf;
	int st;
	int w;
	int h;
	int format;
	void *pixels;

	pixels  = NULL;
	surf    = NULL;
	ren_tex = NULL;
	format  = SDL_PIXELFORMAT_RGBA32;

    /* Get information about texture we want to save */
	st = SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	if (st != 0) {
		SDL_Log("Failed querying texture: %s\n", SDL_GetError());
		goto cleanup;
	}

	ren_tex = SDL_CreateTexture(ren, format, SDL_TEXTUREACCESS_TARGET, w, h);
	if (!ren_tex) {
		SDL_Log("Failed creating render texture: %s\n", SDL_GetError());
		goto cleanup;
	}

    /*
     * Initialize our canvas, then copy texture to a target whose pixel data we
     * can access
     */
     st = SDL_SetRenderTarget(ren, ren_tex);
     if (st != 0) {
     	SDL_Log("Failed setting render target: %s\n", SDL_GetError());
     	goto cleanup;
     }

     SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, 0x00);
     SDL_RenderClear(ren);

     st = SDL_RenderCopy(ren, tex, NULL, NULL);
     if (st != 0) {
     	SDL_Log("Failed copying texture data: %s\n", SDL_GetError());
     	goto cleanup;
     }

    /* Create buffer to hold texture data and load it */
     pixels = malloc(w * h * SDL_BYTESPERPIXEL(format));
     if (!pixels) {
     	SDL_Log("Failed allocating memory\n");
     	goto cleanup;
     }

     st = SDL_RenderReadPixels(ren, NULL, format, pixels, w * SDL_BYTESPERPIXEL(format));
     if (st != 0) {
     	SDL_Log("Failed reading pixel data: %s\n", SDL_GetError());
     	goto cleanup;
     }

    /* Copy pixel data over to surface */
     surf = SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, SDL_BITSPERPIXEL(format), w * SDL_BYTESPERPIXEL(format), format);
     if (!surf) {
     	SDL_Log("Failed creating new surface: %s\n", SDL_GetError());
     	goto cleanup;
     }

    /* Save result to an image */
     st = SDL_SaveBMP(surf, filename);
     if (st != 0) {
     	SDL_Log("Failed saving image: %s\n", SDL_GetError());
     	goto cleanup;
     }

     SDL_Log("Saved texture as BMP to \"%s\"\n", filename);

     cleanup:
     SDL_FreeSurface(surf);
     free(pixels);
     SDL_DestroyTexture(ren_tex);
 }
