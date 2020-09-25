// Create some windows.
SDLUI_Control_Window *wnd1 = SDLUI_CreateWindow(20, 100, 350, 400, "Window1");
SDLUI_Control_Window *wnd2 = SDLUI_CreateWindow(460, 30, 450, 350, "Window2");
SDLUI_Control_Window *wnd3 = SDLUI_CreateWindow(410, 400, 370, 200, "Window3");
SDLUI_Control_Window *wnd4 = SDLUI_CreateWindow(120, 620, 820, 170, "Colors");

// Add some common controls. Simple controls such as these do not need much explaining.
SDLUI_Control_Button *btn1 = SDLUI_CreateButton(wnd1, 10, 40, "PushButton");
SDLUI_Control_TextBox *tbx1 = SDLUI_CreateTextBox(wnd1, 160, 40, 150);
SDLUI_Control_SliderInt *si1 = SDLUI_CreateSliderInt(wnd1, 10, 90, 0, 100, 20);
SDLUI_Control_SliderInt *si2 = SDLUI_CreateSliderInt(wnd1, 140, 90, 0, 100, 60, SDLUI_ORIENTATION_VERTICAL);
SDLUI_Control_CheckBox *chk1 = SDLUI_CreateCheckBox(wnd1, 10, 130, "Checkbox", false);

// A ScrollArea is a dual purpose control. It can either host an image (in the form of an SDL_Texture),
// or a list, where it effectively replicates the functionality of a listbox.
// In both cases it draws and handles scrollbars when the content is bigger than the ScrollArea's size.
// In the example bellow we create a ScrollArea that will be used as a listbox.
// We pass NULL as the last parameter since we don't have an image texture.
SDLUI_Control_ScrollArea *sa1 = SDLUI_CreateScrollArea(wnd1, 10, 220, 320, 160, NULL);

// A list needs an array of strings. It doesn't matter what kind of container it is (std::vector, C array of strings, etc),
// as long as it can feed the list with a char* representing the caption for each element. This will make more sense
// when we'll later call the control's usage function inside the application loop.
std::vector<std::string> list_items;
for (int i = 0; i < 100; ++i)
{
	std::string cur_item = "List Item: ";
	cur_item += std::to_string(i);
	list_items.push_back(cur_item);
}

// We can now create a list and bind it to the ScrollArea above.
// If the size of the data container isn't known at compile time we can simply pass zero to the last function parameter.
// In this case we pass the size of the list_items vector.
SDLUI_Control_List *lst1 = SDLUI_CreateList(wnd1, sa1, list_items.size());

// Some more controls...
SDLUI_Control_Button *btn2 = SDLUI_CreateButton(wnd2, 10, 40, "ClickMe");
SDLUI_Control_CheckBox *chk2 = SDLUI_CreateCheckBox(wnd2, 10, 90, "Another checkbox", true);

// Here's another ScrollArea. This time it hosts an image. If the image texture isn't availlable at compile time,
// we can pass NULL as the last parameter. Here we first create a texture and pass it on to the ScrollArea.
SDL_Surface *surf = IMG_Load("test_image.png");
SDL_Texture *tex = SDL_CreateTextureFromSurface(SDLUI_Core.renderer, surf);
SDL_FreeSurface(surf);
SDLUI_Control_ScrollArea *sa2 = SDLUI_CreateScrollArea(wnd2, 10, 120, 430, 220, tex);

// RadioButtons are handled in groups. A special kind of array(SDLUI_ArrayOfControls) is used
// to store pointers to RadioButtons which belong in the same group. The SDLUI_CreateRadioButtonGroup() function
// is just a helper to create and initialize such an array.
SDLUI_ArrayOfControls rb_group1 = SDLUI_CreateRadioButtonGroup();
SDLUI_ArrayOfControls rb_group2 = SDLUI_CreateRadioButtonGroup();

// Create a bunch of RadioButtons, assigning them to their respective group.
SDLUI_Control_RadioButton *rb1 = SDLUI_CreateRadioButton(wnd3, rb_group1, 30, 80, "RadioButton1", true);
SDLUI_Control_RadioButton *rb2 = SDLUI_CreateRadioButton(wnd3, rb_group1, 30, 110, "RadioButton2", false);
SDLUI_Control_RadioButton *rb3 = SDLUI_CreateRadioButton(wnd3, rb_group1, 30, 140, "RadioButton3", false);

SDLUI_Control_RadioButton *rb4 = SDLUI_CreateRadioButton(wnd3, rb_group2, 220, 80, "RadioButton4", false);
SDLUI_Control_RadioButton *rb5 = SDLUI_CreateRadioButton(wnd3, rb_group2, 220, 110, "RadioButton5", true);
SDLUI_Control_RadioButton *rb6 = SDLUI_CreateRadioButton(wnd3, rb_group2, 220, 140, "RadioButton6", false);

// More controls...
SDLUI_Control_ToggleButton *tb1 = SDLUI_CreateToggleButton(wnd3, 30, 80, "Toggle Button", true);
SDLUI_Control_Button *btn3 = SDLUI_CreateButton(wnd3, 30, 110, "Test");

// A TabContainer is a container for child controls, and all it does is manage their visibility.
// It can be created with either a horizontal (default) or vertical strip of tabs.
// It's a 3-step proccess: Create a TabContainer, add some tabs, and finally add previously created controls to each tab.
// Note that positioning of controls is relative to the window and not to the position of the TabContainer inside the window.
SDLUI_Control_TabContainer *tbc1 = SDLUI_CreateTabContainer(wnd3, 10, 40, 350, 140);

tbc1->add_tab("First");
tbc1->add_tab("Second");
tbc1->add_tab("Third");

tbc1->set_active_tab(0);

tbc1->add_child(0, rb1);
tbc1->add_child(0, rb2);
tbc1->add_child(0, rb3);
tbc1->add_child(0, rb4);
tbc1->add_child(0, rb5);
tbc1->add_child(0, rb6);

tbc1->add_child(1, tb1);
tbc1->add_child(1, btn3);

// Yet more controls...
SDLUI_Control_Button *btn_copy = SDLUI_CreateButton(wnd4, 680, 40, "Copy");
SDLUI_Control_Text *txt01 = SDLUI_CreateText(wnd4, 10, 40, "Active window bar");
SDLUI_Control_Text *txt02 = SDLUI_CreateText(wnd4, 10, 70, "Inactive window bar");;
SDLUI_Control_Text *txt03 = SDLUI_CreateText(wnd4, 10, 100, "Window bg");
SDLUI_Control_Text *txt04 = SDLUI_CreateText(wnd4, 10, 130, "Highlight");

// Here's an example creating controls in batch and storing their pointers in an SDLUI_ArrayOfControls.
SDLUI_ArrayOfControls color_sliders;
color_sliders.create();

i32 x = 200, y = 40;
const int num_sliders = 12;

for (int i = 0; i < num_sliders; ++i)
{
	SDLUI_Control_SliderInt *si = SDLUI_CreateSliderInt(wnd4, x, y, 0, 255, 0);
	si->w = 128;
	color_sliders.push(si);
	x += 150;

	if((i + 1) % 3 == 0 && i > 0)
	{
		x = 200;
		y += 30;
	}
}

// We modify the 'value' field of each slider after creation. Most control fields are safe to modify directly,
// but not all of them (currently undocumented).
SDLUI_Control_SliderInt *col_slider;
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[0]; col_slider->value = SDLUI_Core.theme.col_active_window_bar.r;
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[1]; col_slider->value = SDLUI_Core.theme.col_active_window_bar.g;
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[2]; col_slider->value = SDLUI_Core.theme.col_active_window_bar.b;

col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[3]; col_slider->value = SDLUI_Core.theme.col_inactive_window_bar.r;
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[4]; col_slider->value = SDLUI_Core.theme.col_inactive_window_bar.g;
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[5]; col_slider->value = SDLUI_Core.theme.col_inactive_window_bar.b;

col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[6]; col_slider->value = SDLUI_Core.theme.col_window_bg.r;
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[7]; col_slider->value = SDLUI_Core.theme.col_window_bg.g;
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[8]; col_slider->value = SDLUI_Core.theme.col_window_bg.b;

col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[9]; col_slider->value = SDLUI_Core.theme.col_highlight.r;
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[10]; col_slider->value = SDLUI_Core.theme.col_highlight.g;
col_slider = (SDLUI_Control_SliderInt*)color_sliders.data[11]; col_slider->value = SDLUI_Core.theme.col_highlight.b;


SDLUI_Control_Window *calc = SDLUI_CreateWindow(980, 280, 170, 280, "Calc");
calc->can_be_resized = false;
SDLUI_Control_Text *display = SDLUI_CreateText(calc, 150, 50, "0");

SDLUI_Control_Button *calc_btnc = SDLUI_CreateButton(calc, 130, 80, "C"); calc_btnc->w = 30;

SDLUI_Control_Button *calc_btn7 = SDLUI_CreateButton(calc, 10, 120, "7"); calc_btn7->w = 30;
SDLUI_Control_Button *calc_btn8 = SDLUI_CreateButton(calc, 50, 120, "8"); calc_btn8->w = 30;
SDLUI_Control_Button *calc_btn9 = SDLUI_CreateButton(calc, 90, 120, "9"); calc_btn9->w = 30;
SDLUI_Control_Button *calc_btndiv = SDLUI_CreateButton(calc, 130, 120, "/"); calc_btndiv->w = 30;

SDLUI_Control_Button *calc_btn4 = SDLUI_CreateButton(calc, 10, 160, "4"); calc_btn4->w = 30;
SDLUI_Control_Button *calc_btn5 = SDLUI_CreateButton(calc, 50, 160, "5"); calc_btn5->w = 30;
SDLUI_Control_Button *calc_btn6 = SDLUI_CreateButton(calc, 90, 160, "6"); calc_btn6->w = 30;
SDLUI_Control_Button *calc_btnmul = SDLUI_CreateButton(calc, 130, 160, "x"); calc_btnmul->w = 30;

SDLUI_Control_Button *calc_btn1 = SDLUI_CreateButton(calc, 10, 200, "1"); calc_btn1->w = 30;
SDLUI_Control_Button *calc_btn2 = SDLUI_CreateButton(calc, 50, 200, "2"); calc_btn2->w = 30;
SDLUI_Control_Button *calc_btn3 = SDLUI_CreateButton(calc, 90, 200, "3"); calc_btn3->w = 30;
SDLUI_Control_Button *calc_btnsub = SDLUI_CreateButton(calc, 130, 200, "-"); calc_btnsub->w = 30;

SDLUI_Control_Button *calc_btn0 = SDLUI_CreateButton(calc, 10, 240, "0"); calc_btn0->w = 30;
SDLUI_Control_Button *calc_btndot = SDLUI_CreateButton(calc, 50, 240, "."); calc_btndot->w = 30;
SDLUI_Control_Button *calc_btneq = SDLUI_CreateButton(calc, 90, 240, "="); calc_btneq->w = 30;
SDLUI_Control_Button *calc_btnadd = SDLUI_CreateButton(calc, 130, 240, "+"); calc_btnadd->w = 30;