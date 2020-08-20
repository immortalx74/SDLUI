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

#include "sdlui_enums.cpp"
#include "sdlui_structs.cpp"
#include "sdlui_embeded.cpp"
#include "sdlui_helpers.cpp"
#include "sdlui_render.cpp"
#include "sdlui_create.cpp"
#include "sdlui_event.cpp"
#include "sdlui_usage.cpp"

// Each window should have an array of children
// Each control should have a parent_window pointer field (in SDLUI_Control struct)
// Could a window belong to another window or NULL if it lives on screen???
// A window_collection array holds pointers to all windows

// Each window has an "active" field. With every click on screen, a check should
// be made to determine if it's inside the active window. If not, another check
// should be made to determine if it's inside another window. If true, first make
// the other window active. Proccess ONLY the active window's child controls
// on render loop (by checking if the control on current iteration belongs to
// the active_window??)

// Make the active window and its children render last.

// Controls that belong to a window should get clipped it their position is outside
// of the window bounds. Maybe render each window with its controls to a texture
// and then draw only the window rectangle part of the texture?

// Windows should prob have a resize feature