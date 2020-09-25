# SDLUI



SDLUI is a bare bones GUI library made with SDL, for SDL, and written in a style that resembles SDL.
It depends on standard SDL extensions (SDL_image, SDL_ttf).
Its purpose is to have a convenient way to build small apps, utilities and tools right within SDL, without much hassle to set-up.

## How to use

See the included skeleton SDL app (main.cpp).

The boilerplate are 4 functions:
* SDLUI_Init
* SDLUI_EventHandler
* SDLUI_WindowHandler
* SDLUI_Render

Control creation and usage is a mix between retained and immediate mode. That means there's a separate function to create a control, but there are no callbacks, and using a control is similar to the immediate mode paradigm. The idea is to "prepare" the entirety of the UI beforehand, and then call each control's usage function inside the main application loop. e.g.:
```c
// Create a button in previously created window "wnd1"
SDLUI_Control_Button *btn1 = SDLUI_CreateButton(wnd1, 10, 40, "PushButton");
//...
//...
//...
// In main application loop
if(SDLUI_Button(btn1))
{
	// Do something on button click...
}
```
