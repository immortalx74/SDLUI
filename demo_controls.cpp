SDLUI_Control_Window *wnd = SDLUI_CreateWindow(10, 10, 400, 400, "Demo Window (Enter to toggle visibility)");
SDLUI_Control_Button *btn = SDLUI_CreateButton(30, 60, "PushButton");
SDLUI_Control_SliderInt *si = SDLUI_CreateSliderInt(30, 110, 0, 100, 50);
SDLUI_Control_CheckBox *chk = SDLUI_CreateCheckBox(30, 140, true);
SDLUI_Control_ToggleButton *tb = SDLUI_CreateToggleButton(30, 180, false);
SDLUI_Control_RadioButton *rb1 = SDLUI_CreateRadioButton(30, 220, true);
SDLUI_Control_RadioButton *rb2 = SDLUI_CreateRadioButton(30, 250, false);
SDLUI_Control_RadioButton *rb3 = SDLUI_CreateRadioButton(30, 280, false);

SDLUI_Control_RadioButton *rb4 = SDLUI_CreateRadioButton(90, 220, false, 1);
SDLUI_Control_RadioButton *rb5 = SDLUI_CreateRadioButton(90, 250, true, 1);
SDLUI_Control_RadioButton *rb6 = SDLUI_CreateRadioButton(90, 280, false, 1);

btn->parent_to(wnd);
si->parent_to(wnd);
chk->parent_to(wnd);
tb->parent_to(wnd);
rb1->parent_to(wnd);
rb2->parent_to(wnd);
rb3->parent_to(wnd);

rb4->parent_to(wnd);
rb5->parent_to(wnd);
rb6->parent_to(wnd);


SDLUI_Control_Window *wnd2 = SDLUI_CreateWindow(10, 300, 400, 400, "Another Window");