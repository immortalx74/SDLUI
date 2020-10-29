# SDLUI



SDLUI is a bare bones GUI library made with SDL, for SDL, and written in a style that resembles SDL.
It depends on standard SDL extensions (SDL_image, SDL_ttf).
Its purpose is to have a convenient way to build small apps, utilities and tools right within SDL, without much hassle to set-up.

## How to use

Include ```sdlui.h``` in your project. See the example skeleton SDL app (main.cpp).

The boilerplate are 4 functions:
* SDLUI_Init
* SDLUI_EventHandler
* SDLUI_WindowHandler
* SDLUI_Render

Control creation and usage is a mix between retained and immediate mode. That means there's a separate function to create a control,
yet there are no callbacks, and using a control is similar to the immediate mode paradigm. The idea is to "prepare" the entirety of the UI beforehand, and then call each control's usage function inside the main application loop. e.g.:

```c++
// Create a button in previously created window "my_window1"
SDLUI_Control_Button *btn1 = SDLUI_CreateButton(my_window1, 10, 40, "PushButton");
...
...
...
// In main application loop
if(SDLUI_Button(btn1))
{
	// Do something on button click...
}
```

Every control resides in a window, and its placement is relative to its parent window position.

## List of controls
* Window
* Button
* SliderInt
* CheckBox
* Text
* ToggleButton
* RadioButton
* TabContainer
* ScrollArea / List
* TextBox

## Known issues/incomplete features
* TextBox not fully implemented. Need to handle scrolling past the textbox capacity, selection, copy/paste, etc.
* More work needed on scrollbars and detection of TextArea's client rect.
* Enable/disable of controls not implemented yet. This should resemble Windows' greyed-out appearance.
* Windows changes in scaling are not detected. Everything is currently drawn in the same size.
* Doesn't probably handle scaling when dragging the main window between monitors (currently untested)
* Apart from the included skeleton application everything is completely untested!

![screenshot](https://github.com/immortalx74/SDLUI/blob/master/screenshot.jpg)
