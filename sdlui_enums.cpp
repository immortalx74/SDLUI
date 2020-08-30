// Enums ----------------------------------------------

enum SDLUI_BUTTON_STATE
{
	SDLUI_BUTTON_STATE_NORMAL,
	SDLUI_BUTTON_STATE_HOVER,
	SDLUI_BUTTON_STATE_CLICK
};

enum SDLUI_ALIGN
{
	SDLUI_ALIGN_LEFT = 0,
	SDLUI_ALIGN_CENTER = 1,
	SDLUI_ALIGN_RIGHT = 2
};

enum SDLUI_ORIENTATION
{
	SDLUI_ORIENTATION_HORIZONTAL,
	SDLUI_ORIENTATION_VERTICAL
};

enum SDLUI_MOUSEBUTTON
{
	SDLUI_MOUSEBUTTON_NONE,
	SDLUI_MOUSEBUTTON_PRESSED,
	SDLUI_MOUSEBUTTON_HELD,
	SDLUI_MOUSEBUTTON_RELEASED
};

enum SDLUI_CONTROL_TYPE
{
	SDLUI_CONTROL_TYPE_WINDOW,
	SDLUI_CONTROL_TYPE_BUTTON,
	SDLUI_CONTROL_TYPE_SLIDER_INT,
	SDLUI_CONTROL_TYPE_CHECKBOX,
	SDLUI_CONTROL_TYPE_TOGGLE_BUTTON,
	SDLUI_CONTROL_TYPE_TAB_CONTAINER,
	SDLUI_CONTROL_TYPE_TAB,
	SDLUI_CONTROL_TYPE_LABEL,
	SDLUI_CONTROL_TYPE_TEXT,
	SDLUI_CONTROL_TYPE_RADIO_BUTTON,
	SDLUI_CONTROL_TYPE_SCROLL_AREA,
	SDLUI_CONTROL_TYPE_LIST
};

enum SDLUI_RESIZE_DIRECTION
{
	SDLUI_RESIZE_NONE,
	SDLUI_RESIZE_LEFT,
	SDLUI_RESIZE_TOP,
	SDLUI_RESIZE_RIGHT,
	SDLUI_RESIZE_BOTTOM,

	SDLUI_RESIZE_LEFT_TOP,
	SDLUI_RESIZE_RIGHT_TOP,
	SDLUI_RESIZE_LEFT_BOTTOM,
	SDLUI_RESIZE_RIGHT_BOTTOM
};