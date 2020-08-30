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