#pragma once

#include <iostream>
#include <stdlib.h>

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#pragma comment(lib, "Shcore.lib")
	#include <windows.h>
	#include <ShellScalingAPI.h>
	#include <comdef.h>
#endif

#ifdef __APPLE__
    #include <SDL2/SDL.h>
    #include <SDL2_image/SDL_image.h>
    #include <SDL2_ttf/SDL_ttf.h>
#else
    #include "SDL.h"
    #include "SDL_ttf.h"
    #include "SDL_image.h"
#endif

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
#define SDLUI_WINDOW_MIN_SIZE 120

#include "sdlui_enums.cpp"
#include "sdlui_structs.cpp"
#include "sdlui_embeded.cpp"
#include "sdlui_helpers.cpp"
#include "sdlui_render.cpp"
#include "sdlui_create.cpp"
#include "sdlui_event.cpp"
#include "sdlui_usage.cpp"
