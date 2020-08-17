SDLUI_Control_Window *wnd1 = SDLUI_CreateWindow(10, 10, 350, 400, "test window1");
SDLUI_Control_Window *wnd2 = SDLUI_CreateWindow(380, 10, 450, 350, "test window2");
SDLUI_Control_Window *wnd3 = SDLUI_CreateWindow(250, 300, 550, 400, "test window3");

SDLUI_Control_Button *btn1 = SDLUI_CreateButton(wnd1, 10, 40, "PushButton");
SDLUI_Control_SliderInt *si = SDLUI_CreateSliderInt(wnd1, 10, 90, 0, 100, 20);
SDLUI_Control_SliderInt *si2 = SDLUI_CreateSliderInt(wnd1, 140, 90, 0, 100, 60, SDLUI_ORIENTATION_VERTICAL);
SDLUI_Control_CheckBox *chk1 = SDLUI_CreateCheckBox(wnd1, 10, 130, false);

SDLUI_Control_Button *btn2 = SDLUI_CreateButton(wnd2, 10, 40, "ClickMe");
SDLUI_Control_CheckBox *chk2 = SDLUI_CreateCheckBox(wnd2, 10, 90, false);
SDLUI_Control_Text *txt1 = SDLUI_CreateText(wnd2, 50, 90, "Some text here!");

SDLUI_Array rb_group1 = SDLUI_Create_RadioButtonGroup();
SDLUI_Array rb_group2 = SDLUI_Create_RadioButtonGroup();

SDLUI_Control_RadioButton *rb1 = SDLUI_CreateRadioButton(wnd3, rb_group1, 10, 40, true);
SDLUI_Control_RadioButton *rb2 = SDLUI_CreateRadioButton(wnd3, rb_group1, 10, 70, false);
SDLUI_Control_RadioButton *rb3 = SDLUI_CreateRadioButton(wnd3, rb_group1, 10, 100, false);

SDLUI_Control_RadioButton *rb4 = SDLUI_CreateRadioButton(wnd3, rb_group2, 70, 40, false);
SDLUI_Control_RadioButton *rb5 = SDLUI_CreateRadioButton(wnd3, rb_group2, 70, 70, true);
SDLUI_Control_RadioButton *rb6 = SDLUI_CreateRadioButton(wnd3, rb_group2, 70, 100, false);

SDLUI_Control_ToggleButton *tb1 = SDLUI_CreateToggleButton(wnd3, 140, 140, true);
SDLUI_Control_Button *btn3 = SDLUI_CreateButton(wnd3, 140, 180, "Test");

SDLUI_Control_TabContainer *tbc1 = SDLUI_CreateTabContainer(wnd3, 100, 40, 350, 200);

tbc1->add_tab("First");
tbc1->add_tab("Second");
tbc1->add_tab("Third");

tbc1->add_child(0, tb1);
tbc1->add_child(0, btn3);