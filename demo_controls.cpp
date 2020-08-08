SDLUI_Control_Window *wnd1 = SDLUI_CreateWindow(10, 10, 350, 400, "test window1");
SDLUI_Control_Window *wnd2 = SDLUI_CreateWindow(380, 10, 450, 350, "test window2");
SDLUI_Control_Window *wnd3 = SDLUI_CreateWindow(250, 350, 450, 300, "test window3");

SDLUI_Control_Button *btn1 = SDLUI_CreateButton(wnd1, 10, 40, "PushButton");
SDLUI_Control_SliderInt *si = SDLUI_CreateSliderInt(wnd1, 10, 90, 0, 100, 20);
SDLUI_Control_CheckBox *chk1 = SDLUI_CreateCheckBox(wnd1, 10, 130, false);

SDLUI_Control_Button *btn2 = SDLUI_CreateButton(wnd2, 10, 40, "ClickMe");
SDLUI_Control_CheckBox *chk2 = SDLUI_CreateCheckBox(wnd2, 10, 90, false);

SDLUI_Array rb_group1 = SDLUI_Create_RadioButtonGroup();
SDLUI_Array rb_group2 = SDLUI_Create_RadioButtonGroup();

SDLUI_Control_RadioButton *rb1 = SDLUI_CreateRadioButton(wnd3, rb_group1, 10, 40, true);
SDLUI_Control_RadioButton *rb2 = SDLUI_CreateRadioButton(wnd3, rb_group1, 10, 70, false);
SDLUI_Control_RadioButton *rb3 = SDLUI_CreateRadioButton(wnd3, rb_group1, 10, 100, false);

SDLUI_Control_RadioButton *rb4 = SDLUI_CreateRadioButton(wnd3, rb_group2, 70, 40, false);
SDLUI_Control_RadioButton *rb5 = SDLUI_CreateRadioButton(wnd3, rb_group2, 70, 70, true);
SDLUI_Control_RadioButton *rb6 = SDLUI_CreateRadioButton(wnd3, rb_group2, 70, 100, false);